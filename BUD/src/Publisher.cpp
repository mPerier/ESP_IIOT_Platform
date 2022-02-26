/*********************************
* Simple MQTT Publish Client 
* With Serial link for output message
* Button on GPIO_0 
* Led GPIO_2 is ON when button pushed
* Sensor SHT30
*
* Connect to WiFi hub and MQTT Broker.  
*  
*  
* WiFi Hub :
*    - ssid = "robotique";
*    - password = "robotiqueS3";
* The IP address of the ESP8266 module, will be 
* printed to Serial when the module is connected. 
*  
* Station IP is 192.168.1.28
*  
* MQTT Broker Configuration
*   mqtt_server => 192.168.1.2
*   mqtt_topic => Temp
*   mqtt_topic2 => Humi
*   mqtt_username => usr8
*   mqtt_password => usr8
*   clientID => ESP8266_Pub
*   
* Message received => Led GPIO_0 set to OFF for 1s
*   
* Board : ESP-D1    
* Author : B. BIAIS ESTIA
* Date : 07/12/2021
*********************************/
#include <Arduino.h>
#include <ESP8266WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#include <WEMOS_SHT3X.h> // Used for "Temp&Hydr" the SHT30
#include <WS.h> // Allows us to connect to, and publish to the MQTT broker
#include <MQTT.h> // Allows us to connect to, and publish to the MQTT broker
#include <GPIO.h>
#include <AP.h>

#define triggerTime 2

/// Init of SHT30 Sensor for temp & hydr
SHT3X sht30(0x45); 

/// MQTT Server Adress
const char* mqtt_server = "192.168.1.2";
/// MQTT Topic for Temperature
const char* mqtt_topic = "Temp";
/// MQTT Topic for Humidity
const char* mqtt_topic2 = "Humi";
/// MQTT Username
const char* mqtt_username = "usr9";
/// MQTT Password
const char* mqtt_password = "usr9";

/// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "ESP8266_Puberty";

/// Timer of the last timer registration
unsigned long last = millis();
/// Timer of current time
unsigned long now = millis();

/// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
/// Initialisation of 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient); 
/// Initialisation of Wifi Client
WS ws;
/// Initialisation of MQTT Server
MQTT mqtt;

/**
 * @brief Setup for the ESP8266
 * 
 */
void setup() {
  // initialize digital esp8266 gpio 0 as an input.
  pinMode(D3, INPUT);
  // initialize digital esp8266 gpio 2 as an output.
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  delay(1000);

  // Begin Serial on 115200
  // Remember to choose the correct Baudrate on the Serial monitor!
  // This is just for debugging purposes
  Serial.begin(9600);

  ws.InitWifiWsMode();
  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  mqtt.Connect(&client, clientID, mqtt_username, mqtt_password);
}
 /**
  * @brief Loop for the ESP8266
  * 
  */
void loop() {
  // Update button state
  // This needs to be called so that the Bouncer object can check if the button has been pressed 
  ws.getServer();
  now = millis();
  if(now - last >= (triggerTime*1000)){
    if(sht30.get()==0){
      setTemp(sht30.cTemp);
      mqtt.publishTopic(client, mqtt_topic, sht30.cTemp, clientID, mqtt_username, mqtt_password);
      setHumi(sht30.humidity);
      mqtt.publishTopic(client, mqtt_topic2, sht30.humidity, clientID, mqtt_username, mqtt_password);
    }
    last = millis();
  }
  
}
