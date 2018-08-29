#include "ESPEnergy.h"
#define DEBUGMIO
char[] myvolt[100],myamp[100],mycosfact[100];
void setup()
{
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
	prendi_dati();
	smartDelay(1000);
}
// called by interrupt service routine when incoming data arrives
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
void loop(){

	for (uint16_t i = 0; i < 10; i++) {
		uint8_t check = prendi_dati();
		myvolt[i]=myener.supplyVoltage;
		myamp[i]=myener.Irms;
		mycosfact[i]=myener.powerFactor;
		smartDelay(5000);
	}
	//thisvolt/=50;
	//thisamp/=50;
	//mypow=thisamp * thisvolt;
	DEBUG_PRINT("verifico connessione...");
	uint8_t check = connLAN(); 		//check == 1 -> connected to local WIFI
	if(check==0){
		DEBUG_PRINT("NO LAN -> chiudo");
		delay(50000);
		return;
	}
	DEBUG_PRINT("ok connesso.");
	//reconnect();
	//printMqtt();
	sendWeb();
	client.disconnect();
	WiFi.mode(WIFI_OFF); //energy saving mode if local WIFI isn't connected
	WiFi.forceSleepBegin();
	smartDelay(10);
}
void sendWeb(){
	String s ="volt[]=" + String(myvolt) +
	+"&pwd=" + webpass +
	+"&ampere=" + String(myener.Irms) +
	+"&power=" + String(mypow);
	http.begin(post_server);
	http.addHeader("Content-Type", "application/x-www-form-urlencoded");
	int httpResponseCode = http.POST(s);
	if(httpResponseCode>0){

		String response = http.getString();                       //Get the response to the request

		DEBUG_PRINT(httpResponseCode);   //Print return code
		DEBUG_PRINT(response);           //Print request answer

	 }else{

		DEBUG_PRINT("Error on sending POST: ");
		DEBUG_PRINT(httpResponseCode);

	 }
	http.end();  //Free resources
	//return true;
}
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
uint8_t prendi_dati(){
	DEBUG_PRINT("richiedo dati...");
	Wire.requestFrom (SLAVE_ADDRESS, sizeof(myener));
	delay(1);
	uint8_t i=I2C_readAnything(myener);
	//mypow = myener.supplyVoltage * myener.Irms;
	DEBUG_PRINT("volt " + String(myener.supplyVoltage));
	DEBUG_PRINT("amp " + String(myener.Irms));
	DEBUG_PRINT("pfact " + String(myener.powerFactor));
	DEBUG_PRINT("i " + String(i));
	return i;

	//return 1;
}
