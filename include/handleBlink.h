#ifndef HANDLEBLINK_H
#define HANDLEBLINK_H

#include <Arduino.h>

void handleBlink(int port, unsigned long *previousMillis, unsigned long delayMillis);

#endif /* HANDLEBLINK_H */