#ifndef AP_H
#define AP_H

    #include <WiFi.h>
    #include <WebServer.h>
    #include "sharedFunctions.h"

    void setTemp(float temperatureReal);
    float getTemp(void);
    void setHumi(float humidityReal);
    float getHumi(void);

#endif /* AP_H */