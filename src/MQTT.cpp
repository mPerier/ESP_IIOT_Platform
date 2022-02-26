#include <MQTT.h>

// ----------------------------- CONNECT -----------------------------
int MQTT_Connect() {

  int mqtt_returnCode;

  char paramBuf[32];
  char mqtt_username[32];
  char mqtt_password[32];
  char mqtt_clientID[32];
  char mqtt_server[32];

  // Gets MQTT parameters from EEPROM
  getEEPROMParam(e_mqtt_usr).toCharArray(paramBuf,32);
  memcpy(&mqtt_username,paramBuf,32);
  getEEPROMParam(e_mqtt_passwd).toCharArray(paramBuf,32);
  memcpy(&mqtt_password,paramBuf,32);
  getEEPROMParam(e_mqtt_server).toCharArray(paramBuf,32);
  memcpy(&mqtt_server,paramBuf,32);
  getEEPROMParam(e_mqtt_client_id).toCharArray(paramBuf,32);
  memcpy(&mqtt_clientID,paramBuf,32);

  // Print MQTT parameters
  Serial.println("MQTT Parameters:");
  Serial.println(mqtt_server);
  Serial.println(mqtt_clientID);
  Serial.println(mqtt_username);
  Serial.println(mqtt_password);

  MQTTclient.setServer(mqtt_server, 1883);
  MQTTclient.setClient(wifiClient);
  // setCallback sets the function to be called when a message is received.
  MQTTclient.setCallback(MQTT_ReceiveMessageEvent);

  // Connect to MQTT Server
  mqtt_returnCode = MQTTclient.connect(mqtt_clientID, mqtt_username, mqtt_password);
  if (mqtt_returnCode) {
    Serial.println("Connected Successfully to MQTT Broker!"); 
    Serial.println("As user : " + String(mqtt_username));
  }
  else {
    Serial.print("Connection Failed! ");
    Serial.print("As user : " + String(mqtt_username));
    Serial.print(" Error Code: " + String(mqtt_returnCode));
   }
  return mqtt_returnCode;
}
// -------------------------- END CONNECT ---------------------------

// -------------------- MQTT_subscribeTopicsList --------------------
void MQTT_subscribeTopicsList(){
  int i;
  char buf[32];

  Serial.println("Subscribing to topics:");
  // Iterate through each of the topics
  for(i=FIRST_MQTT_TOPIC_ID_EEPROM;i<FIRST_MQTT_TOPIC_ID_EEPROM+NUMBER_OF_MQTT_TOPIC_SUBS;i++){
    getEEPROMParam(i).toCharArray(buf,32);
    // If the first character readed in EEPROM is a correct value, and not a "NULL TERMINATED CHARACTER"
    if (isAscii(buf[0]) && buf[0] != '\0'){
      Serial.println(buf);
      // Create subscribtion
      MQTTclient.subscribe(buf);
    }
  }
}
// -------------------- MQTT_subscribeTopicsList --------------------


// ------------------------- MQTT_subscribe -------------------------
int  MQTT_subscribe(char *topic){
  return MQTTclient.subscribe(topic);
}
// ----------------------- END MQTT_subscribe -----------------------

// --------------------- RECEIVED MESSAGE EVENT ---------------------
void MQTT_ReceiveMessageEvent(char* topic, byte* payload, unsigned int length){
  char receivedMsg[length+1];

  // Float check variables
  float ignore;

  memset(receivedMsg,'\0',length+1);
  // Resets, than writes into receivedMsg
  memcpy(&receivedMsg, payload, length);

  // sscanf returns the size of the string, or the size of any non-alphanumeric element
  // Is sscanf returned the size of the string, receivedMsg[len] will get the last character value: '\0' that equals to 0, or TRUE
  if (sscanf(receivedMsg, "%f", &ignore)) {
    Serial.print("Received float on topic: ");
    Serial.println(topic);
    Serial.println(atof(receivedMsg));
    memcpy(param1,receivedMsg,32);
    memcpy(param2,topic,32);
  }
  else {
    Serial.print("Received string on topic ");
    Serial.println(topic);
    Serial.println(receivedMsg);
  }
}
// ------------------- END RECEIVED MESSAGE EVENT --------------------

// ------------------------ MQTT_sendMessage -------------------------
int MQTT_sendMessage(char *msg, char *topic){
  return MQTTclient.publish(topic,msg);
}
// ---------------------- END MQTT_sendMessage -----------------------


