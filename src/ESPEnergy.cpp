#include "ESPEnergy.h"
//#define DEBUGMIO
void setup()
{
	#ifdef DEBUGMIO
	Serial.begin(9600);
  delay(5000);
	#endif

	DEBUG_PRINT("Booting!");
	//APPENA ACCESO CONTROLLA LA CONNESSIONE WiFi
	uint8_t check = connLAN(); 		//check == 1 -> connected to local WIFI
	if(check==0){
		DEBUG_PRINT("NO LAN -> chiudo");
		delay(500);
		shutDownNow();
		//FINE PROGRAMMA
	}


}
void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do
  {
    client.loop();
		delay(1);
	} while (millis() - start < ms);
}
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}
void shutDownNow(){
	for (byte i = 0; i < 5; i++) {
		DEBUG_PRINT("spegniti");
	  delay(500);
		Wire.begin(default_sda_pin, default_scl_pin);
		delay(500);
		Wire.beginTransmission (2);
	  Wire.write (20);
	  Wire.endTransmission(true);
	}

}
void loop(){

		Wire.begin(default_sda_pin, default_scl_pin);
	  Wire.requestFrom (SLAVE_ADDRESS, sizeof(ener));
		I2C_readAnything (ener);
		reconnect();
		printMqtt();
		smartDelay(3000);
}
//MQTT//////////////////////////////////////////////////////////////
void printMqtt(){
	StaticJsonBuffer<300> JSONbuffer;
	JsonObject& JSONencoder = JSONbuffer.createObject();
	JSONencoder["topic"] = "Energy";
	JSONencoder["realPower"] = ener.realPower;
	JSONencoder["apparentPower"] = ener.apparentPower;
	JSONencoder["Irms"] = ener.Irms;
	JSONencoder["supplyVoltage"] = ener.supplyVoltage;
	JSONencoder["powerFactor"] = ener.powerFactor;
	char JSONmessageBuffer[100];
	JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
	client.publish(sensorsTopic, JSONmessageBuffer,true);
	DEBUG_PRINT("mandato dati a mqtt");

}
void reconnect() {
	for (char i = 0; i < 10; i++){
		DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(nodeID,mqttUser,mqttPass)) {
      DEBUG_PRINT("MQTT connected");
			// Once connected, publish an announcement...
      client.publish(logTopic, "ESP-01 Energy connesso");
			break;
			}else {
				switch (client.state()){
	        case -4:
	          DEBUG_PRINT("MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time");
	          break;
	        case -3:
	          DEBUG_PRINT("MQTT_CONNECTION_LOST - the network connection was broken");
	          break;
	        case -2:
	          DEBUG_PRINT("MQTT_CONNECT_FAILED - the network connection failed");
	          break;
	        case -1:
	          DEBUG_PRINT("MQTT_DISCONNECTED - the client is disconnected cleanly");
	          break;
	        case 0:
	          break;
	        case 1:
	          DEBUG_PRINT("MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT");
	          break;
	        case 2:
	          DEBUG_PRINT("MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier");
	          break;
	        case 3:
	          DEBUG_PRINT("MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection");
	          break;
	        case 4:
	          DEBUG_PRINT("MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected");
	          break;
	        case 5:
	          DEBUG_PRINT("MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect");
	          break;
	        default:
	          Serial.print("failed, rc=");
	          Serial.println(client.state());
	          break;
	     }
			}
		}
	}
