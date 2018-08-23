#ifndef cxconfig_h
#define cxconfig_h
#include <Arduino.h>
struct energyData{
	float realPower;
	float apparentPower;
	float powerFactor;
  float supplyVoltage;
  float Irms;
};
typedef struct energyData EnergyData;
volatile boolean haveData = false;
//ESP-01 SDA - SCL pin
const int SLAVE_ADDRESS = 0X02; //attiny
//WIFI stuff
#ifdef ARDUINO_AVR_ATTINYX5
volatile EnergyData myener;
#else
//ESP-01 SDA - SCL pin
static int default_sda_pin = 0;
static int default_scl_pin = 2;
volatile EnergyData myener;
const char* ssid     = "**********";
const char* password = "*******";
const char* webpass ="*****";
#include <ESP8266WiFi.h>
IPAddress ip(192, 168, 1, 218); //Node static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
// data web host Settings
const char* host = "www.********.com";
const int httpPort = 80;
//voltage stuff
//mqtt
const char* nodeID="mainPower";
const char* mqtt_server = "192.168.1.100";
const uint16_t mqtt_port = 8883;
const char* mqttUser ="*******";
const char* mqttPass ="******";
const char* sensorsTopic = "*****";
const char* logTopic ="*****"; // log generico per lo piu non usato
#endif




#endif
