#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h> //https://github.com/Links2004/arduinoWebSockets
WebSocketsClient webSocket;
String initText="";
const char* ssid = "Vu Bich Ngoc"; //Đổi thành wifi của bạn //realme 2//Vu Bich Ngoc
const char* password = "18121961"; //Đổi pass luôn //12345678//18121961
const char* ip_host = "192.168.1.188"; //Đổi luôn IP host của PC nha//192.168.1.187
const uint16_t port = 3000; //Port thích đổi thì phải đổi ở server nữa
float val1;
float val2;
byte wireArray[4];
String val;
String vall;
char myBuffer[33];
void getData(){
  byte index = 0;
  Wire.requestFrom(8,2);
  while (Wire.available() > 0)
  {
    wireArray[index] = Wire.read();
    index++;
  }
  val1 = wireArray[0]; 
  val2 = wireArray[1]                                                                                                                                                                                                                      +(val1/3); 
}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
      }
      break;
    case WStype_BIN:
      Wire.beginTransmission(8);
      Wire.write((char *)payload);
      Wire.endTransmission();    /* stop transmitting */

      Serial.printf("[WSc] get text:%s\n", payload);
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}
void setup() {
  Serial.begin(57600); /* begin serial for debug */
 Wire.begin(D1, D2); 
  Serial.println("ESP8266 Websocket Client");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  webSocket.begin(ip_host, port);
  webSocket.onEvent(webSocketEvent);
}
void loop() {
  webSocket.loop();
  getData();
  val = itoa(val1,myBuffer,10);
  vall = itoa(val2,myBuffer,10);
  String ms=vall+"C  "+val+"%";
//  Serial.println(val);
  webSocket.sendTXT(ms);
}
