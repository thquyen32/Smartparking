#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include<WiFi.h>
#include<HTTPClient.h>
#include<SPI.h>
#include<ESP32Servo.h>
#include<MFRC522.h>
#include<Ticker.h>
#include<Wire.h>
#define buzzer_pin 16
#define servo_pin 25
#define cambien1_pin 33
#define cambien2_pin 32
#define cambien3_pin 35
#define SS_PIN 5
#define RST_PIN 4
#define SCREEN_WITDH 128
#define SCREEN_HEIGHT 64
#define OLED_ADD 0x3C

Adafruit_SSD1306 display(SCREEN_WITDH,SCREEN_HEIGHT,&Wire,-1);

Servo myServo;
const char* ssid = "TuGiang";
const char* pass = "0986712793";
const char* serverUrl = "http://192.168.1.6:8483/";
MFRC522 Card_reader(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key;
String tag;
String slot1_id;
String slot2_id;
String slot3_id;
int k=1;
int k1=1;
int k2=1;
int k3=1;
void setup()
{
Serial.begin(9600);
SPI.begin();
Card_reader.PCD_Init();
pinMode(cambien1_pin,INPUT);
pinMode(cambien2_pin,INPUT);
pinMode(cambien3_pin,INPUT);
pinMode(16,OUTPUT);
pinMode(servo_pin,OUTPUT);
digitalWrite(buzzer_pin,LOW);
myServo.attach(servo_pin);
WiFi.begin(ssid,pass);
Serial.print("Đang kết nối ");
while(WiFi.status() != WL_CONNECTED)
{
  delay(1000);
  Serial.print(".");
}
Serial.print("\n");
Serial.print("IP WIFI: ");
Serial.println(WiFi.localIP());
if(!display.begin(SSD1306_SWITCHCAPVCC,OLED_ADD))
{
  Serial.println("Fail generate OLED");
  for(;;);
}
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.drawFastHLine(0,20,128,WHITE);
display.drawFastHLine(0,40,128,WHITE);
display.drawFastVLine(45,0,64,WHITE);
display.setCursor(5,5);
display.print("Slot 1");
display.setCursor(5,27);
display.print("Slot 2");
display.setCursor(5,50);
display.print("Slot 3");
display.display();
myServo.write(0);

}
void loop()
{
RFID();
}
void RFID()
{
  tag="";
  if(! Card_reader.PICC_IsNewCardPresent())
  return;
  if(Card_reader.PICC_ReadCardSerial())
  {
    for(int i=0; i<Card_reader.uid.size; i++)
    {
      tag+= Card_reader.uid.uidByte[i];
    }
      HTTP(tag);  
      Card_reader.PICC_HaltA();
      Card_reader.PCD_StopCrypto1();
  }
}
void HTTP(String tag)
{
  if(WiFi.status() == WL_CONNECTED)
{
  HTTPClient http;
  http.begin(serverUrl);
  int response = http.GET();
  if(response > 0)
  {
    k=1;
  String payload = http.getString();
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + 60;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, payload);     
  if (error)
  {
    Serial.println("Lỗi phân tích JSON");
    Serial.println(error.f_str());
    return;
  }
  for(int i = 0; i < doc.size();i++)
  {
    String id = doc[i]["id"];
    String name =doc[i]["name"];
    if (id == tag)
    {
      if(tag == slot1_id)
      {
       if(digitalRead(cambien1_pin)==1 && k1==0) 
      {
        buzzer_access();
        cambien1();
        k1=1;
        slot1_id = "";
        servo();
      }
      else
      {
        buzzer_denied();
      }
      }
      else if(tag == slot2_id)
      {
        if(digitalRead(cambien2_pin)==1 && k2==0) 
      {
        buzzer_access();
        cambien2();
        k2=1;
        slot2_id = "";
        servo();
      }
      else
      {
        buzzer_denied();
      }
      }
      else if(tag == slot3_id)
      {
          if(digitalRead(cambien3_pin)==1 && k3==0) 
      {
        buzzer_access();
        cambien3();
        k3=1;
        slot3_id = "";
        servo();
      }
      else
      {
        buzzer_denied();
      }
      }
      else
      {
        buzzer_access();
        servo();
      if(digitalRead(cambien1_pin)==0 && k1==1)
      {
        slot1(name);
        k1=0;
        slot1_id = id;
      }
      else if(digitalRead(cambien2_pin)==0 && k2==1)
      {
        slot2(name);
        k2=0;
        slot2_id = id;
      }
      else if(digitalRead(cambien3_pin)==0 && k3==1) 
      {
        slot3(name);
        k3=0;
        slot3_id = id;
      }
      }
      k=0;
      break;
    }
    Serial.println(k);
  }
  if(k==1)
  {
    buzzer_denied();
  }
}
  else{
    Serial.println(response);
  }
  http.end();
}
else{
  Serial.println("Mất kết nối WiFi");
}
}
void buzzer_access ()
{
digitalWrite(buzzer_pin,HIGH);
delay(300);
digitalWrite(buzzer_pin,LOW);
delay(300);
digitalWrite(buzzer_pin,HIGH);
delay(300);
digitalWrite(buzzer_pin,LOW);
delay(300);
}
void buzzer_denied()
{
  digitalWrite(buzzer_pin,HIGH);
  delay(1000);
  digitalWrite(buzzer_pin,LOW);
  delay(1000);
}
void servo()
{
myServo.write(90);
delay(2500);
myServo.write(0);
delay(2500);
}
void cambien1()
{
  if(digitalRead(cambien1_pin)==1)
  {
  display.fillRect(50,5,128,8,BLACK);
  display.display();
}
}
void cambien2()
{
  if(digitalRead(cambien2_pin)==1)
  {
  display.fillRect(50,27,128,8,BLACK);
  display.display();
  k2=1;
}
}
void cambien3()
{
  if(digitalRead(cambien3_pin)==1)
  {
  display.fillRect(50,50,128,8,BLACK);
  display.display();
  k3=1;
}
}
void slot1(String name)
{
    display.setCursor(55,5);
    display.print(name);
    display.display();
    k1=0;
}
void slot2(String name)
{
    display.setCursor(55,27);
    display.print(name);
    display.display();
    k2=0;
}
void slot3(String name)
{
    display.setCursor(55,50);
    display.print(name);
    display.display();
    k3=0;
}

