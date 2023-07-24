#include <SerialComs.h>
#include <SoftwareSerial.h>

//dong co trai
#define IN1 4
#define IN2 5
//#define textReceived getTextReceived()
//dong co phai
#define IN3 6
#define IN4 7

//dieu khien toc do dong co
#define ENA 8
#define ENB 9

const int RX = 10;
const int TX = 11;
SoftwareSerial espSerial(RX, TX);

////cảm biến siêu âm
const int trig = 3;
const int echo = 2;

SerialComs coms;

String x_data, y_data, v_data, data;
String str;
int khoang_cach;
int x, y, v;
char inChar, count;
int ck;
boolean gotData;
int minRange = 312;
int maxRange = 712;
int max_tocdo = 800;
byte moc;

void setup(){
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  setup_motor(); 
  Serial.begin(115200);
  for (int i = 10; i>0; i--){
    Serial.print(' '); Serial.print(i);
    delay(500);
  }
  Serial.println();
  Serial.println("UNOSoftSerial");

  SafeString::setOutput(Serial);
  espSerial.begin(9600);
  if(!coms.connect(espSerial)){
    while(1){
      Serial.println(F("out of memory"));
      delay(3000);
    }
  }
  Serial.println(F("Uno setup finished"));
}

void setup_motor(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

int tinh_khoang_cach(){
  unsigned long duration;
  //Phat xung tu chan trig, chu ky la 7s, trong do 2s Low, 5s High
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(5);
  digitalWrite(trig, 0);

  //Tinh toan thoi gian phan hoi
  duration = pulseIn(echo, HIGH);
  //khoang cach = thoi_gian/2 *v, v = 340m/s = 0,034 cm/us
  khoang_cach = int(duration/2*0.034);  
}

void re_trai(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, v);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, v);
}
void re_phai(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, v);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, v);
}
void tien(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, v);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, v);
}
void lui(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, v);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, v);
}
void dung_xe(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void giaimaData(){
  x = x_data.toInt();
  x_data = "";
  //Serial.println(x);
  y = y_data.toInt();
  y_data = "";
  //Serial.println(y);
  v = v_data.toInt();
  v_data = "";
  //Serial.println(v);  
}

void serialEvent(){
  while (!coms.textReceived.isEmpty()){
    //get the new byte
    inChar = (char)Serial.read();
    if (inChar == '!'){
      ck = 1;
    }
    else if (ck == 1 && inChar == '@'){
      ck = 2;
    }
    else if (ck == 2 && inChar == '#'){
      ck = 3;
    }
    else if (ck == 3 && inChar == '$'){
      ck = 4;
      count = 0;
    }
    else{
      if ((ck == 4) && (inChar != '#')){
        x_data += inChar;
      }
      else if ((ck == 5) && (inChar != '#')){
        y_data += inChar;
      }
      else if ((ck == 6) && (inChar != '#')){
        v_data += inChar;
      }
      if (inChar == '#'){
        ck += 1;
        if (ck == 7){
          gotData = true;
          ck = 0;
        }
      }
    }
  }
}

void loop(){
  coms.sendAndReceive();
  if(coms.textToSend.isEmpty()){
    tinh_khoang_cach();
    //coms.textToSend.print("Khoang cach = ");
    coms.textToSend.print(khoang_cach);
  
    Serial.println(coms.textToSend);
  }
  
  if (!coms.textReceived.isEmpty()){
//    String data = coms.textReceived;
    Serial.println(coms.textReceived);
//    if (gotData){
//      giaimaData();

//    for (int i = 0; i < data.length(); i++){
//              if (data.charAt(i) == ',') {
//            moc = i; //Tìm vị trí của dấu ","
//        }
//    }
//    }
//    if (y < maxRange && y > minRange && x < maxRange && x > minRange) // dừng
//    {
//      dung_xe();
//    }
//    else if (y >= maxRange && x >= minRange && x <= maxRange) // tới
//    {
//      tien();if (khoang_cach < 50) {dung_xe();}
//    }
//    else if (x >= maxRange && y >= minRange && y <= maxRange) // rẽ phải
//    {
//      re_phai();
//    }
//    else if (x <= minRange && y >= minRange && y <= maxRange) // rẽ trái
//    {
//      re_trai();
//    }
//    else if (y <= minRange && x >= minRange && x <= maxRange) // Lui
//    {
//      lui();
//    }
//     gotData = false; 
    }
}
