#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SerialComs.h>

const int RX = 13; //D7
const int TX = 12; //D6

SoftwareSerial softSerial(RX, TX); 

SerialComs coms;

char auth[] = "7Db5tznFnkSki8FysXX5cAVVvWDD7rnc";
char ssid[] = "HUAWEI-8093";
char pass[] = "75279974";
BlynkTimer timer;

int x;
int y;
int v;
String data_str;
int pinValue;
int kc;
char tds[10];
char tdsEC[10];
bool joystickIspush = false;
bool sliderIspush = false;

BLYNK_WRITE(V0){
  x = param[0].asInt();
  y = param[1].asInt();
  joystickIspush = true;  
}

BLYNK_WRITE(V1){
  v = param.asInt();
  sliderIspush = true;
}

BLYNK_WRITE(V3) {
  pinValue = param.asInt();
}

void setup()
{
  // Debug console
  pinMode(D7, INPUT);
  pinMode(D8, OUTPUT);
  Serial.begin(115200);
  for (int i = 10; i > 0; i--){
    Serial.print(' ');
    Serial.print(i);
    delay(500);
  }
  Serial.println();
  Serial.println("ESP_from_UNO");

  SafeString::setOutput(Serial);
  
  softSerial.begin(9600);
  coms.setAsController();
  if(!coms.connect(softSerial)){
    while(1){
      Serial.println(F("out of memory"));
    }
  }
  Serial.println(F("ESP setup finished"));
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  timer.run();
  coms.sendAndReceive();  
    if (!coms.textReceived.isEmpty()){
      
      Serial.println(coms.textReceived);
      if (pinValue == 1){
        Blynk.virtualWrite(V2, coms.textReceived.c_str()); 
      }
    }
    if(coms.textToSend.isEmpty()){
      //coms.textToSend.print("!@#$");
      coms.textToSend.print("#");
      coms.textToSend.print(x);
      coms.textToSend.print("#");
      coms.textToSend.print(y);
      coms.textToSend.print("#");
      coms.textToSend.print(v);
      coms.textToSend.print("#");
      Serial.println(coms.textToSend);
    }
}
