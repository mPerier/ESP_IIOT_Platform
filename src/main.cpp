/*********************************
* MAIN function of the ESP8266 IIOT Platform
*
* Handles:
*  - RS232 Server
*  - Acess Point handling
*  - WebServer Handling
*  - Flash writing parameters
*  - MQTT Server
*
* Board : ESP-D1    
*
* Authors:
*   - Base sample:  | O. Patrouix | ESTIA
*   - Sensor file:  | B.  BIAIS   | ESTIA
*   - Actuator file | M. PERIER   | ESTIA 
*
* Changelog:
*    V1.0 Initialising the project with WIFI Workstation
*    V1.2 Intialising MQTT communication
*    V1.3 Integration of temperature sensor MQTT messages: opens or closes the relay
*    V1.4 Changes on MQTT messages treatment: topic specific treatments
*    V1.5 Decorrelation of "hardware functions" (WS.c, MQTT.c, AP.c) from "main.c" 
*    V1.6 Initialising of the "ShellDon" Command Line Interface: RS232 user input, parsed and executed in serial ISR
*    V1.7 Initialisation of EEPROM get & set functions
*    V1.8 Major debug version, and ShellDon command implementation (RELAY, WIFI_INIT, MemoryDisplay, MemoryModify)
*    V1.9 Functionnal MQTT Server, implementation of "Module specific" initialisation and loop actions
*    ----------------------------------------------------------------------------------------------------------------
*    V10.0 Modification of firmware to implement the server on ESP32 modules.

* Date : 26/02/2022
*********************************/
#define FIRMWARE_VERSION 10.0

// ##################################################################
// ###                          INCLUDES                          ###
// ##################################################################
#include "GLOBALS.h"
#include "WEMOS_SHT3X.h" // Used for "Temp&Hydr" the SHT30
#include "handleBlink.h"
#include "sharedFunctions.h"
#include "RS.h"
#include "AP.h"
#include "WS.h"

// ##################################################################
// ###                SHARED VARIABLES & CONSTANTS                ###
// ##################################################################
// ---- CONSTANTS ---- 
#define EEPROM_SIZE 512
#define triggerTime 2
const long intervalMillis = 1000;           // interval at which to blink (milliseconds)
// -- END CONSTANTS --

// ---- WiFi CONFIGURATION ----
const char* ssid;
const char* password;

// --- END WiFi CONFIGURATION --

// ---- MQTT CONFIGURATION ----
const char* mqtt_topicTemp;
const char* mqtt_topicHumi;
// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* mqtt_clientID;
// -- END MQTT CONFIGURATION --

// ---- DYNAMIC VARIABLES ----
unsigned long previousMillis;        // will store last time LED was updated
char param1[32], param2[32];
// -- END DYNAMIC VARIABLES --

// ---- TYPE VARIABLES ----
WiFiClient wifiClient;
PubSubClient MQTTclient; // 1883 is the listener port for the Broker
SHT3X sht30(0x45); /// Init of SHT30 Sensor for temp & hydr
String inStr = "";
String srlBuffer;
WebServer server(80);
//WS ws;

// -- END TYPE VARIABLES --

// ---- SHARED VARIABLES ----
char outputPars[MAX_PARAM_NUMBER][MAX_PARAM_SIZE];
char *p_outputPars = &outputPars[0][0];
int moduleType;
// -- END SHARED VARIABLES --

// ##################################################################
// ##################### FUNCTIONS DEFINITION #######################
// ##################################################################

// ############################ SETUP ###############################
void setup() {

  // TODELETE
    previousMillis = 0;

  // END TODELETE

  // initialize LED and relay Pins as outputs 
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  EEPROM.begin(EEPROM_SIZE);

  // Security delay 
  delay(1000);

  // Start Serial Link
  initSrl();
  Serial.print("\n#############################################################\n             [INIT] ESP8266 IIOT Platform V");
  Serial.print(FIRMWARE_VERSION);
  Serial.println("\n#############################################################\n");

  // Get module type
  moduleType = getEEPROMParam(e_typeModule).toInt();
  Serial.println("[INIT] If hardware module configuration is incoherent, change it's value via the Loading Server: MM 0 {VALUE}");

  // -------------------- MODULE SPECIFIC INITIALISATION ---------------------
  switch (moduleType){

    case m_UNDEFINED:
      Serial.println("/!\\ UNDEFINED Module type! Please reconfigure module type by modifying adress 0 of the EEPROM (MM Command) CLI");
    break;

    case m_relay:
      Serial.println("[INIT] Relay module configuration.");
      if (InitWifiWsMode(0)) {
        if (MQTT_Connect()){
          MQTT_subscribeTopicsList();
        }
        else {
          Serial.println("[INIT] Can't access MQTT Server, starting Access Point");
          
        }
      }
    break;

    case m_tempHumi:
      Serial.println("[INIT] Temperature & Humidity module configuration.");
      if (InitWifiWsMode(1)){
        if (MQTT_Connect()){
        }
        else {
          Serial.println("[INIT] Can't access MQTT Server, starting Access Point");

        }
      }
    break;

    default:
      Serial.print("[INIT] Unknown module type: [");
      Serial.print(moduleType);
      Serial.println("]. Modify the value of address 0 via the Loading Server: MM 0 {VALUE}");
      Serial.println("If you are introducing a new module type, you will need to modify the µc's firmware.");
    break;
  }
  // ----------------- END MODULE SPECIFIC INITIALISATION -------------------
  addToSrl("\n[INIT done. Loading Server ready to receive commands.]");
}
// ########################## END SETUP ##############################


// ############################# LOOP ################################
void loop() {
  double now=0, last=0;

  // ----------------------- MODULE SPECIFIC ACTIONS ------------------------
  switch (moduleType){
    case m_relay:
    now = millis();
      if(now - last >= (triggerTime*1000)){
        if (!strcmp(param2,"Temp")){
          if (atof(param1) > 26) {
            setRelayState(HIGH);
          }
          else{
            setRelayState(LOW);
          }
        }
      }
      last = millis();
    break;

    case m_tempHumi:
      getServer();
now = millis();
      if(now - last >= (triggerTime*1000)){
        if(sht30.get()==0){
          setTemp(sht30.cTemp);
          String(getTemp()).toCharArray(param1,32);
          getEEPROMParam(e_mqtt_topic1).toCharArray(param2,32);
          MQTT_sendMessage(param1,param2);
          setHumi(sht30.humidity);
          String(getHumi()).toCharArray(param1,32);
          getEEPROMParam(e_mqtt_topic2).toCharArray(param2,32);
          MQTT_sendMessage(param1,param2);
        }
        last = millis();
      }
    break;
  }
  // --------------------- END MODULE SPECIFIC ACTIONS ----------------------

  if (MQTTclient.connected()) {
    MQTTclient.loop();
  }

  // Print the serial buffer, and execute eventual commands from the Shell
  printSrlBuffer();
  execCommands(p_outputPars);

  // Once it has done all it needs to do for this cycle, go back to checking if we are still connected.
  handleBlink(LED_PIN, &previousMillis, 1000);
}
// ########################### END LOOP ##############################