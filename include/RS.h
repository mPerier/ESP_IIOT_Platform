#ifndef RS_H
#define RS_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <esp32-hal-uart.h>
#include "sharedFunctions.h"
#include "WS.h"
#include "MQTT.h"

#define MAX_PARAM_SIZE 20 
#define MAX_PARAM_NUMBER 10
#define MAX_OUTPUT_SIZE 1024

const uart_port_t uart_num = UART_NUM_0;

void initSrl();
void addToSrl(String inStr);
void printSrlBuffer();
void ICACHE_RAM_ATTR UART_ISR_ROUTINE(void *pvParameters);
void parseInput(String rawStr, char *outputParams);
void execCommands(char *paramList);


// External variables (global in main.cpp)
extern String srlBuffer;
extern String inStr;
extern char *p_outputPars;



#endif /* RS_H */