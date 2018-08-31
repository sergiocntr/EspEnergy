#ifndef espmeteo_h
#define espmeteo_h
#define DEBUGMIO
#include "EnergyMUXconfig.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "I2C_Anything.h"
#include <PubSubClient.h> //mqtt library
#include <ArduinoJson.h>
#include "wifimio.h"
#include "debugutils.h"
#include <Wire.h>
HTTPClient http;
WiFiClient c;
PubSubClient client(mqtt_server, mqtt_port, c);

//uint16_t volt=0;
void setup();
void loop();
//WIFI
uint8_t connLAN();
bool printWEB(bool timeAvailable);
void smartDelay(unsigned long ms);
void callback(char* topic, byte* payload, unsigned int length);
void printMqtt();
void reconnect();
uint8_t prendi_dati();
void sendWeb(char* );
#endif
