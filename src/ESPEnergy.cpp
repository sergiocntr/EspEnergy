#include "ESPEnergy.h"
#define DEBUGMIO
void setup(){
	#if defined DEBUGMIO
	Serial.begin(9600);
  delay(1000);
	#endif
	WiFi.mode(WIFI_OFF); //energy saving mode if local WIFI isn't connected
	WiFi.forceSleepBegin();
	smartDelay(10);
	delay(5000);
	DEBUG_PRINT("Booting!");
	DEBUG_PRINT("lunghezza " + String(sizeof(myener)));
	Wire.begin(default_sda_pin, default_scl_pin);
}
void loop(){
	uint8_t iteration = 40;
	const size_t bufferSize = 3*JSON_ARRAY_SIZE(iteration) + JSON_OBJECT_SIZE(iteration);
	DynamicJsonBuffer jsonBuffer(bufferSize);

	JsonObject& root = jsonBuffer.createObject();

	JsonArray& volt = root.createNestedArray("volt");
	JsonArray& amp = root.createNestedArray("amp");
	JsonArray& cosfi = root.createNestedArray("cosfi");
	//JsonArray& aP = root.createNestedArray("aP");
	//JsonArray& rP = root.createNestedArray("rP");
	for (uint16_t i = 0; i < iteration; i++) {
		uint8_t check = prendi_dati();
		mfCosfi.in(myener.powerFactor);
		mfVolt.in(myener.supplyVoltage);
		mfAmp.in(myener.Irms);
		//
		volt.add(mfVolt.out());
		amp.add(mfAmp.out());
		cosfi.add(mfCosfi.out());
		DEBUG_PRINT(i);
		smartDelay(5000);
	}
	DEBUG_PRINT("verifico connessione...");
	uint8_t check = connLAN(); 		//check == 1 -> connected to local WIFI
	if(check==0){
		DEBUG_PRINT("NO LAN -> chiudo");
		delay(900000);
		ESP.restart();
	}
	DEBUG_PRINT("ok connesso.");
	//reconnect();
	//printMqtt();
	root["sensorType"] = "Energy";
	char JSONmessageBuffer[3000];
	root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  DEBUG_PRINT(sizeof(JSONmessageBuffer));
	DEBUG_PRINT(JSONmessageBuffer);
	smartDelay(1);
	ESP.wdtDisable();
	sendWeb(JSONmessageBuffer);
	ESP.wdtEnable(1000);
	//client.disconnect();
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF); //energy saving mode if local WIFI isn't connected
	WiFi.forceSleepBegin();
	smartDelay(10);
}
void sendWeb(char* _JSONMessage){

	http.begin(post_server);
	http.addHeader("Content-Type", "application/json");
	int httpResponseCode = http.PUT(_JSONMessage);   //Send the request
	smartDelay(1);
  if(httpResponseCode>0){

		String response = http.getString();                       //Get the response to the request
		DEBUG_PRINT(httpResponseCode);   //Print return code
		DEBUG_PRINT(response);           //Print request answer

	 }else{

		DEBUG_PRINT("Error on sending POST: ");
		DEBUG_PRINT(httpResponseCode);

	 }
	http.end();  //Free resources

}
uint8_t prendi_dati(){
	Wire.requestFrom (SLAVE_ADDRESS, sizeof(myener));
	smartDelay(1);
	uint8_t i=I2C_readAnything(myener);
	return i;
}
void smartDelay(unsigned long ms){
	unsigned long start = millis();
	do
	{
		//client.loop();
		delay(1);
	} while (millis() - start < ms);
}
/*
//MQTT//////////////////////////////////////////////////////////////
void printMqtt(){
	StaticJsonBuffer<300> JSONbuffer;
	JsonObject& JSONencoder = JSONbuffer.createObject();
	JSONencoder["topic"] = "Energy";
	JSONencoder["realPower"] = mypow;
	//JSONencoder["apparentPower"] = myener.apparentPower;
	JSONencoder["Irms"] = myener.Irms;
	JSONencoder["supplyVoltage"] = myener.supplyVoltage;
	JSONencoder["powerFactor"] = myener.powerFactor;
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
      //DEBUG_PRINT("MQTT connected");
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

	void callback(char* topic, byte* payload, unsigned int length) {
	  // handle message arrived
	}
*/
