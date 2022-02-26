#ifndef SHAREDFUNCTIONS_H
#define SHAREDFUNCTIONS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "GPIO.h"
#include "GLOBALS.h"

#define FIRST_MQTT_TOPIC_ID_EEPROM 10
#define NUMBER_OF_MQTT_TOPIC_SUBS 5

// -- EEPROM NAME DEFINITIONS --
enum EEPROMNAMES{
    e_typeModule,
    e_IP_Local,
    e_IP_Gateway,
    e_IP_Subnet,
    e_ssid,
    e_password,
    e_mqtt_usr,
    e_mqtt_passwd,
    e_mqtt_client_id,
    e_mqtt_server,
    e_mqtt_topic1,
    e_mqtt_topic2,
    e_mqtt_topic3,
    e_mqtt_topic4,
    e_mqtt_topic5,
    e_IP_AP,
    e_physicsValue1,
    e_physicsValue2
};

enum MODULETYPES{
    m_relay,
    m_tempHumi,
};

void setRelayState(bool state);
bool getRelayState();
int setEEPROMParam(int name, String inputVal);
String getEEPROMParam(int name);
void getParameterCaracteristics(int name, unsigned int *address, unsigned int *length);


extern char* mqtt_username;
extern const char* mqtt_password;
extern const char* mqtt_clientID;

#endif /* SHAREDFUNCTIONS_H */
