#ifndef espmeteo_h
#define espmeteo_h
#define DEBUGMIO
#include "espEnergyconfig.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "I2C_Anything.h"
#include <PubSubClient.h> //mqtt library
#include <ArduinoJson.h>
#include "wifimio.h"
#include "debugutils.h"
//#include <ArduinoOTA.h>
WiFiClient c;
PubSubClient client(mqtt_server, mqtt_port, c);
void setup();
void loop();
//WIFI
uint8_t connLAN();
bool printWEB(bool timeAvailable);
void requestSensorsValues();
void sendData(uint8_t nrRecords);
void smartDelay(unsigned long ms);
void callback(char* topic, byte* payload, unsigned int length);
void printMqtt();
void printMqttLog(String message);
void storeData(uint8_t nrRecords);
void shutDownNow();
void reconnect();
#endif
