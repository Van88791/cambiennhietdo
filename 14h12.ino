#include <Wire.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include<DHT.h> 

const int DHTPIN = A0;       //Đọc dữ liệu từ DHT11 ở chân A0 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE);

int chosen=1;
int myData[2]; // có 2 giá trị thôi (nhiệt ,độ độ ẩm)
int speed_scroll;
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

uint8_t scrollSpeed = 25;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 2000; // in milliseconds
#define  BUF_SIZE  75
char curMessage[BUF_SIZE] = { "Nhom15" };
char newMessage[BUF_SIZE] = { "Hello" };

void setup() {

  Wire.begin(8);  
    Wire.onReceive(receiveEvent); /* register receive event */
    Wire.onRequest(requestEvent);
 Serial.begin(57600);
 dht.begin();
  // put your setup code here, to run once:
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect,scrollEffect);
}
void receiveEvent(int howMany) {
  String str ="";
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    str+=c;           /* print the character */
  }
  Serial.println(str);
  if(str.length()==2){
    if(str[0]=='1'){
      chosen=1;
    }
    else if(str[0]=='2'){
      chosen=2;
    }
    else if(str[0]=='3'){
      chosen=3;
    }
    else if(str[0]=='4'){
      chosen=4;
    }
  }
  else{
    strcpy(curMessage, str.c_str());
  } 
 Serial.println();             /* to newline */
}
void loop() {  
    float h = dht.readHumidity();    //nhiệt độ
    float t = dht.readTemperature(); //độ ẩm
    myData[0]=h;
    myData[1]=t; 

  if(chosen==1){
    scrollEffect = PA_SCROLL_LEFT;
    P.setTextEffect(scrollEffect,scrollEffect);
    P.setSpeed(50);
  }
  else if(chosen==2){
    scrollEffect = PA_SCROLL_RIGHT;
    P.setTextEffect(scrollEffect,scrollEffect);
    P.setSpeed(50);
  }
    else if(chosen==3){
      if(strlen(curMessage)>5)
        chosen=1;
      else{
    scrollEffect = PA_SCROLL_UP;
    P.setTextEffect(scrollEffect,scrollEffect);
    P.setSpeed(50);
      }

  }
    else if(chosen==4){
      if(strlen(curMessage)>5)
        chosen=1;
      else{
    scrollEffect = PA_SCROLL_DOWN;
    P.setTextEffect(scrollEffect,scrollEffect);
    P.setSpeed(50);
      }
    
  }
  if (P.displayAnimate())
  {
    P.displayReset();
  }
}
 void requestEvent() {
  Wire.write((byte *) myData, sizeof myData);
}
