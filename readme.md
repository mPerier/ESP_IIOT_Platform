# ESP_IIOT_Platform

A software platform for ESP_like microcontrollers

## Description

The main goal of the platform is to simplify the implementation and usage of IIOT ESP modules (for instance, temperature sensors, relays...).
In it's current state, this platform is written for ESP8266 NodeMCU, and implements some basic sensors.

>**This software have been developped by students only with a pedagocial objective in mind.**

## Getting Started

### Skills and knowledge

Because this software was developped by engineering students, it is inheritely made simple, using only the "Procedural" aspects of C/C++, without large complex classes or Object-oriented paradigms.

Anyone who have at least programmed an arduino-like developpement kit, and played a bit with registers, EEPROM, timers and some C principles like pointers and compiling routines should be capable of at least understanding the code.

**Note that because this project is neither professional nor released, it's substance and form could hurt the eyes of experienced programmers or embedded systems specialists.** Sorry for the incovenience, but everyone must start learning somewhere; And hey, it's free!

### Hardware

To implement this software, you will need at least:
- A computer capable of running the [PlatfomIo](https://platformio.org/) platform on [VsCode](https://code.visualstudio.com/).
*Note that the C++ source files can totaly be compiled the "old-fashionned" way with a simple g++ compiler and the coherent libraries and sources, so unsing basically any IT equipment capable of running ".exe" files*.
- A microcontroller from the "ESP" family with RISC or Xstensa CPU.
*Currently, only ESP8266 boards are supported. The implementation on other hardware should only be a matter or config files and libraries replacement.*

### Making your own

The simplest method to implement your own ESP_IIOT_Platform:
* Create a new project with PlatformIO, correctly configured for your own hardware. You will need your "platfomio.ini" to have the "Arduino" framework  and a correct "espressif" platform.
* Clone the contents of the repository's folders "include" and "src" to your project.
* Search for hardware-adherent functions, includes and constants (pinout for example), you can CTRL+F for "8266" for example. You will need to replace the included libraries by the ones used by your specific hardware (check the [platformIO Package Browser](https://registry.platformio.org/search) to find the appropriate ones)
* Once all the obvious hardware adherences fixed, you can try a compilation and adress the compiler's issues one by one ;) Be brave!

## Help section of the CLI

Once the code succesfully compiled and uploaded on your ESP module, run the "Help" command via your favorite RS-232 connection (115200 baud by default)
```
DON> HELP
---------- HELP SECTION -----------
[RST] Resets the µc
[IP LOCAL|GATEWAY|SUBNET  www xxx yyy zzz]: Changes the ip configuration to www.xxx.yyy.zzz
[RELAY xxx] Changes the Relay state: ON or OFF
[MD ID] Displays the value in EEPROM of the parameter ID 
[MM ID VALUE] Modifies the value in EEPROM of the parameter ID to VALUE
[WIFI_INIT] Forces the initialisation of WIFI client
[MQTT_INIT] Forces the initialisation of MQTT client
[MQTT_SUB xxx] Subsribes to MQTT topic xxx. If xxx is nothing, the subscription will be made acording to the list of topics in EEPROM (see sharedFunctions.c)
[MQTT_SENDMSG xxx yyy] Sends the message xxx on the topic yyy
-------- END HELP SECTION ---------
```

## EEPROM 

The EEPROM is separated into multiple blocks of 2 to 32 bits, you can easily understand the access plan by reading the getParameterCaracteristics() function of the "sharedFunctions.cpp" source file:

```
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
```

*Once again, note that this project is not intented for commercial or professional use, so the EEPROM addressing plan is far from optimized, however it is made so that you can easily optimize it!*

## Authors

The two original contributors:

PERIER Marc: [LinkedIn](https://www.linkedin.com/in/marc-perier-6079b4161)
BIAIS Budwael: [LinkedIn](https://www.linkedin.com/in/budwael-biais-677225187)


## License

This project is licensed under the MIT License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [random tutorials](https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/https://github.com/matiassingers/awesome-readme)
* [EQMX](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2https://www.emqx.com/en/blog/esp32-connects-to-the-free-public-mqtt-broker)

**For more informations about the roots of the project and it's first implementations, please contact PERIER Marc.**

