#include "handleBlink.h"

void handleBlink(int port, unsigned long *previousMillis, unsigned long delayMillis){
    unsigned long currentMillis = millis();

    if (currentMillis - *previousMillis > delayMillis){
        digitalWrite(port, !digitalRead(port));
        *previousMillis = currentMillis;
    }

}