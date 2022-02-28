#ifndef WS_H
#define WS_H

    #include <Arduino.h>
    #include <Wifi.h>
    #include <AP.h>
    #include <WebServer.h>
    #include "sharedFunctions.h"

    #define WIFI_CONNEXION_TIMEOUT_MS 10000

    int InitWifiWsMode(int mode);
    void handleRoot(void);
    void handleGetTemp(void);
    String getPage(void);
    String getPageTemp(void);
    void getServer(void);

    class WS{
        public:
            WS(void);
            int InitWifiWsMode (void);
    };

    extern WebServer server;
#endif /* WS_H */