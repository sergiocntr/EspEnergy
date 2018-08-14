#ifndef espEnergyconfig_h
#define espEnergyconfig_h
//ESP-01 SDA - SCL pin
static int default_sda_pin = 0;
static int default_scl_pin = 2;
const int SLAVE_ADDRESS = 0X02; //attiny
//WIFI stuff
const char* ssid     = "****";
const char* password = "****";
#include <ESP8266WiFi.h>
IPAddress ip(192, 168, 1, 217); //Node static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
// data web host Settings
const char* host = "****";
const int httpPort = 80;
//voltage stuff
//mqtt
const char* nodeID="****";
const char* mqtt_server = "192.168.1.100";
const uint16_t mqtt_port = 8883;
const char* mqttUser ="****";
const char* mqttPass ="****";
const char* sensorsTopic = "****";
const char* logTopic ="****"; // log generico per lo piu non usato
struct energyData{
	float realPower;
	float apparentPower;
	float powerFactor;
  float supplyVoltage;
  float Irms;
};
typedef struct energyData EnergyData;
EnergyData ener;
#endif
