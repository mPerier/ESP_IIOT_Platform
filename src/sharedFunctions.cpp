#include "sharedFunctions.h"

void setRelayState(bool state){
    // Changes the relay state in accord with given boolean
    if (digitalRead(RELAY_PIN) != state) {
        digitalWrite(RELAY_PIN,state);
    }
}
bool getRelayState(){
    // Ouputs a boolean representing the relay state
    return digitalRead(RELAY_PIN);
}

int setEEPROMParam(int name, String inputVal){
    // Writes in EEPROM the given parameter value
    unsigned int i;
    unsigned int address = 0;
    unsigned int length = 1;
    
    char inputCharArray[inputVal.length()];

    // Transform into char array
    inputVal.toCharArray(inputCharArray,inputVal.length()+1);

    getParameterCaracteristics(name,&address,&length); 

    // If the given parameter fits in the allocated memory
    if (inputVal.length() <= length) {
        // Writes in EEPROM byte by byte
        for (i=0;i<=inputVal.length();i++) {
            EEPROM.put(address+i,inputCharArray[i]);
        }
        // Applies the EEPROM changes
        EEPROM.commit();
        return 0;
    }
    else {
        return length;
    }
}

String getEEPROMParam(int name){
    // Outputs the stored parameter value
    char inChar = 0;
    String outputString = "";
    unsigned int i;
    unsigned int address = 0;
    unsigned int length = 1;

    
    getParameterCaracteristics(name, &address, &length);

    // Gets the value byte by byte, than adds the NULL TERMINATED CHARACTER
    for(i=0;i<length;i++){
        EEPROM.get(address+i,inChar);
        // Ends the reading at the first NULL TERMINATED CHARACTER
        if (inChar == '\0'){
            break;
        }
        outputString+=inChar;
    }
    return outputString;
}

void getParameterCaracteristics(int name, unsigned int *address, unsigned int *length){
    switch (name) {
        case e_typeModule:
            *address = 0;
            *length = 2;
        break;
        case e_IP_Local:
            *address = 2;
            *length=16;
        break;
        case e_IP_Gateway:
            *address = 18;
            *length=16;
        break;
        case e_IP_Subnet:
            *address = 34;
            *length=16;
        break;
        case e_ssid:
            *address = 50;
            *length=32;
        break;
        case e_password:
            *address = 82;
            *length=32;
        break;
        case e_mqtt_usr:
            *address = 114;
            *length = 32;
        break;
        case e_mqtt_passwd:
            *address = 146;
            *length = 32;
        break;
        case e_mqtt_client_id:
            *address = 178;
            *length = 32;
        break;
        case e_mqtt_server:
            *address = 210;
            *length = 32;
        break;
        case e_mqtt_topic1:
            *address = 242;
            *length = 32;
        break;
        case e_mqtt_topic2:
            *address = 274;
            *length = 32;
        break;
        case e_mqtt_topic3:
            *address = 306;
            *length = 32;
        break;
        case e_mqtt_topic4:
            *address = 338;
            *length = 32;
        break;
        case e_mqtt_topic5:
            *address = 370;
            *length = 32;
        break;
        case e_IP_AP:
            *address = 402;
            *length = 32;
        break;
        case e_physicsValue1:
            *address = 434;
            *length = 32;
        break;
        case e_physicsValue2:
            *address = 466;
            *length = 32;
        break;
                
        default:
            Serial.print("[FATAL ERROR]: Unkown EEPROM parameter ID: ");
            Serial.println(name);
            *address = 0;
            *length = 0;
        break;
    }   
}