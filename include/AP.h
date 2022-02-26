#ifndef AP_H
#define AP_H

    #include <ESP8266Wifi.h>
    #include <ESP8266WebServer.h>
    #include "sharedFunctions.h"

    void setTemp(float temperatureReal);
    float getTemp(void);
    void setHumi(float humidityReal);
    float getHumi(void);

#endif /* AP_H */