#ifndef MQTT_H
#define MQTT_H

    #include <Arduino.h>
    #include <PubSubClient.h>
    #include "sharedFunctions.h"
    #include <WiFi.h>

    extern PubSubClient MQTTclient;
    extern WiFiClient wifiClient;

    int MQTT_Connect();
    void MQTT_subscribeTopicsList();
    int MQTT_subscribe(char *topic);
    void MQTT_ReceiveMessageEvent(char* topic, byte* payload, unsigned int length);
    int MQTT_sendMessage(char *msg, char *topic);

    extern char param1[32], param2[32];

#endif