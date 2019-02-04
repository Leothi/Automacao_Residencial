// Leonardo de Holanda Bonifácio - leohb2@gmail.com

#define BLYNK_PRINT Serial   
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <DHT.h>
#include <SPI.h>
#include <WidgetRTC.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define DHTPIN D7          
#define DHTTYPE DHT11    
#define relay1 D8
#define relay2 D1
#define relay3 D2
#define button1 D3
#define button2 D4
#define button3 D5
#define IR_LED D0
#define RAWBUF 400

uint16_t ac24[] = {9060,  4564, 548,  1724, 548,  1724, 548,  596, 552,  604, 548,  596, 548,  596, 548,  596, 552,  1732, 548,  596, 548,  1724, 548,  1720, 548,  608, 548,  596, 548,  600, 544,  600, 548,  604, 548,  1724, 548,  1724, 548,  596, 548,  604, 548,  596, 552,  596, 548,  596, 548,  604, 552,  596, 548,  1720, 552,  596, 548,  604, 548,  596, 552,  1720, 548,  1724, 548,  608, 548,  596, 548,  596, 548,  596, 548,  608, 548,  596, 548,  596, 548,  596, 552,  604, 548,  596, 548,  600, 548,  596, 548,  604, 548,  600, 548,  596, 548,  596, 548,  604, 548,  8076, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 544,  604, 544,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 544,  604, 544,  600, 548,  600, 544,  604, 548,  600, 544,  600, 548,  600, 548,  600, 544,  600, 548,  600, 544,  604, 548,  600, 544,  600, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  600, 548,  600, 548,  600, 544,  604, 544,  600, 548,  600, 544,  604, 544,  600, 548,  600, 548,  600, 544,  604, 544,  600, 548,  600, 548,  600, 544,  604, 544,  600, 548,  600, 544,  604, 544,  660, 544,  1728, 544,  600, 544,  600, 548,  608, 544,  600, 548,  1724, 544,  1724, 548,  604, 544};
uint16_t ac23[] = {9060, 4568, 552, 1724, 548, 1724, 544, 600, 548, 608, 548, 596, 548, 596, 548, 600, 548, 1732, 544, 600, 548, 1724, 548, 1724, 548, 608, 548, 600, 544, 600, 548, 596, 548, 608, 548, 1724, 548, 1724, 548, 596, 548, 608, 548, 596, 548, 596, 548, 600, 548, 604, 548, 596, 552, 1720, 552, 596, 548, 604, 552, 1724, 548, 596, 548, 1724, 548, 604, 552, 596, 544, 600, 548, 600, 548, 604, 548, 596, 552, 596, 548, 596, 548, 608, 548, 596, 548, 596, 548, 600, 548, 604, 548, 600, 548, 596, 548, 596, 548, 604, 548, 8080, 548, 600, 548, 600, 548, 600, 548, 600, 548, 596, 552, 596, 548, 600, 548, 600, 548, 600, 548, 596, 548, 600, 552, 596, 548, 600, 548, 600, 548, 600, 548, 600, 548, 600, 548, 596, 548, 600, 552, 596, 548, 600, 544, 604, 548, 600, 548, 596, 552, 596, 548, 600, 548, 600, 548, 600, 548, 600, 548, 596, 552, 596, 552,596, 548, 600, 548, 600, 548, 600, 548, 596, 552, 596, 548, 600, 548, 600, 548, 600, 548, 600, 548, 600, 548, 596, 548, 600, 548, 600, 548, 600, 548, 600, 548, 600, 548, 596, 552, 596, 548, 600, 548, 600, 548, 596, 552, 600, 548, 596, 552, 656, 548, 1724, 548, 596, 548, 596, 552, 604, 548, 1724, 548, 596, 552, 1720, 552, 600, 548}; 
uint16_t ac22[] = {9060, 4564, 548, 1724, 544, 1728, 544, 600, 544, 608, 548, 596, 548, 600, 544, 600, 544, 1736, 544, 600, 544, 1728, 544, 1728, 544, 608, 544, 600, 548, 600, 544, 600, 544, 608, 548, 1724, 544, 1728, 544, 600, 544, 608, 548, 600, 544, 600, 544, 600, 544, 612, 544, 600, 544, 1724, 548, 600, 544, 608, 544, 604, 544, 600, 544, 1728, 544, 608, 544, 600, 544, 600, 548, 600, 544, 608, 544, 600, 548, 600, 544,600, 544, 608, 548, 600, 544, 600, 544, 600, 544, 612, 544, 600, 544, 600, 544, 600, 544, 608, 544, 8080, 544, 604, 544, 604, 544, 600, 548, 600, 544, 604, 544, 600, 548, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 548, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 548, 600, 544, 604, 544, 600, 548, 600, 548, 600, 544, 604, 544, 600, 544, 604, 544, 600, 548, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 544, 604, 544, 604, 544, 600, 548, 600, 544, 660, 544, 1728, 544, 600, 544, 604, 544, 608, 544, 600, 544, 604, 544, 1724, 544, 604, 548};
uint16_t  onac22[] = {9012,  4592, 524,  1752, 520,  1748, 520,  628, 520,  632, 520,  624, 520,  628, 520,  624, 520,  1760, 520,  624, 520,  1752, 520,  1752, 520,  636, 520,  624, 520,  624, 520,  628, 516,  636, 520,  1752, 520,  1752, 520,  1752, 520,  632, 520,  628, 520,  624, 520,  624, 520,  636, 520,  624, 520,  1752, 520,  624, 520,  636, 520,  624, 520,  624, 520,  1752, 520,  632, 524,  624, 520,  624, 520,  624, 520,  636, 520,  624, 520,  624, 520,  628, 516,  636, 520,  624, 520,  628, 516,  628, 520,  632, 520,  624, 520,  628, 520,  624, 520,  632, 516,  8108, 520,  628, 520,  628, 520,  628, 520,  624, 524,  624, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  628, 516,  628, 520,  628, 520,  628, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  628, 520,  628, 516,  628, 520,  628, 520,  628, 520,  624, 520,  628, 520,  628, 520,  628, 520,  628, 520,  624, 520,  632, 516,  688, 516,  1752, 520,  628, 520,  1752, 520,  632, 520,  624, 520,  628, 520,  1752, 520,  628, 520};
uint16_t onac24[] = {9040,  4564, 548,  1724, 548,  1724, 548,  596, 548,  608, 548,  596, 548,  596, 548,  600, 548,  1732, 548,  596, 548,  1724, 548,  1724, 548,  608, 544,  600, 548,  596, 548,  596, 548,  608, 548,  1720, 552,  1724, 544,  1724, 548,  608, 548,  596, 548,  600, 544,  600, 548,  604, 548,  596, 548,  1724, 548,  600, 548,  604, 548,  596, 548,  1724, 548,  1724, 548,  608, 544,  600, 548,  596, 548,  596, 548,  608, 548,  596, 548,  596, 548,  596, 548,  608, 548,  596, 548,  596, 548,  600, 548,  604, 548,  596, 548,  600, 548,  596, 548,  600, 548,  8080, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  596, 552,  596, 548,  600, 548,  596, 552,  596, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  596, 552,  596, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  656, 548,  1724, 548,  596, 548,  1724, 548,  608, 548,  596, 548,  1724, 548,  1724, 548,  600, 548};
uint16_t timer24[]= {9044,  4568, 544,  1728, 544,  1724, 548,  600, 544,  608, 544,  604, 544,  600, 544,  600, 544,  1736, 544,  600, 544,  1728, 544,  1728, 544,  608, 548,  600, 544,  600, 544,  600, 548,  608, 544,  1724, 548,  1724, 548,  596, 548,  608, 544,  600, 548,  600, 544,  600, 544,  608, 548,  600, 544,  1728, 544,  600, 544,  1736, 544,  600, 544,  1728, 544,  1728, 544,  608, 548,  1724, 544,  604, 544,  1724, 548,  608, 544,  600, 548,  600, 544,  600, 544,  608, 544,  604, 544,  600, 548,  596, 544,  608, 548,  600, 544,  600, 544,  600, 548,  604, 544,  8080, 548,  600, 544,  604, 544,  604, 544,  604, 544,  600, 544,  604, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  604, 544,  600, 548,  600, 548,  600, 544,  604, 544,  600, 548,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  604, 544,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  604, 544,  660, 544,  600, 544,  600, 548,  1724, 544,  1736, 548,  600, 544,  1724, 548,  1724, 548,  604, 544};
uint16_t timer23[]= {9064,  4564, 548,  1724, 548,  1724, 544,  604, 544,  608, 544,  600, 548,  596, 548,  600, 544,  1736, 544,  600, 544,  1728, 544,  1728, 544,  608, 544,  600, 548,  596, 548,  600, 544,  608, 544,  1728, 544,  1728, 544,  600, 544,  608, 548,  600, 544,  600, 544,  600, 548,  608, 544,  600, 544,  1728, 544,  600, 544,  1736, 544,  1728, 544,  604, 544,  1724, 548,  608, 544,  1728, 544,  600, 544,  1728, 544,  608, 548,  600, 544,  600, 544,  600, 544,  612, 544,  600, 544,  600, 544,  600, 548,  608, 544,  600, 544,  600, 544,  604, 544,  604, 544,  8080, 548,  600, 548,  600, 544,  604, 544,  604, 544,  600, 544,  604, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  604, 544,  604, 544,  600, 548,  600, 544,  660, 548,  600, 544,  600, 544,  1728, 544,  1736, 544,  1728, 544,  600, 544,  1728, 544,  604, 548};
uint16_t timer22[]= {9064,  4564, 548,  1724, 548,  1724, 548,  596, 548,  608, 548,  596, 548,  596, 548,  600, 548,  1732, 548,  596, 548,  1724, 548,  1724, 548,  604, 548,  596, 552,  596, 548,  596, 548,  604, 552,  1724, 544,  1724, 552,  596, 548,  604, 548,  596, 552,  596, 548,  596, 552,  600, 548,  600, 548,  1724, 544,  600, 548,  1732, 548,  596, 548,  596, 548,  1724, 548,  608, 544,  1724, 548,  596, 552,  1724, 544,  608, 548,  596, 548,  596, 548,  600, 548,  604, 548,  596, 548,  600, 548,  596, 548,  604, 548,  596, 548,  600, 548,  596, 548,  600, 548,  8080, 548,  600, 548,  596, 552,  596, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  596, 552,  596, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  596, 552,  596, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  600, 548,  600, 544,  600, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 548,  596, 548,  600, 548,  600, 548,  600, 548,  600, 544,  600, 548,  660, 548,  596, 548,  596, 548,  1724, 548,  1732, 548,  600, 548,  596, 548,  1724, 548,  600, 548};
int khz = 38;

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);
WidgetRTC rtc;
WidgetLED led1(V20);  
WidgetLED led2(V21); 
WidgetLED led3(V22);
WidgetTerminal terminal(V0);
IRsend irsend(IR_LED); 

char auth[] = "312xxx3aa7688c1b54caef57";

char ssid[] = "Leo";  
char pass[] = "xxxx";  

-----------------------

int relayVButton1 = 0;
int relayVButton2 = 0;
int relayVButton3 = 0;
int isFirstConnect = true;

boolean relayState1 = 1;
boolean relayState2 = 1;
boolean relayState3 = 1;
boolean buttonState1 = 1;
boolean buttonState2 = 1;
boolean buttonState3 = 1;

//------------------msgs conectado e disc-----------
BLYNK_CONNECTED() {
  if (isFirstConnect) {
    Blynk.syncAll();
    isFirstConnect = false;
  }

  Serial.println("\n\n\n\n\n");
  Serial.println("------------------");
  Serial.println("AUTOMACAO 1 - CONECTADO");
  Serial.println("------------------");

  terminal.println("\n\n\n\n\n");
  terminal.println("------------------");
  terminal.println("AUTOMACAO 1 - CONECTADO");
  terminal.println("------------------");
  terminal.flush();
  rtc.begin();
}
  
BLYNK_APP_CONNECTED() {
  Serial.println("App aberto.");
  terminal.println("App aberto.");
  terminal.flush();
}
//----------------------------------------------------

//-------------------------------SETUP---------------------------------------------------
void setup(){
 pinMode(relay1, OUTPUT);
 pinMode(button1,INPUT_PULLUP);
 pinMode(relay2, OUTPUT);
 pinMode(button2,INPUT_PULLUP);
 pinMode(relay3, OUTPUT);
 pinMode(button3,INPUT_PULLUP);
 pinMode(IR_LED, OUTPUT);
 Serial.begin(9600); 
 Blynk.begin(auth, ssid, pass); 
 dht.begin();
 
 timer.setInterval(12000L, sendSensor);    // SENSOR DHT
 timer.setInterval(5000L, clockDisplay);  // CLOCK
 timer.setInterval(300L, somefunction);   // FUNÇAO RELAY

while (Blynk.connect() == false) {
 buttonState1 = digitalRead (button1);
  if (buttonState1 > 0){
    relayState1 = !relayState1;
      } 
      digitalWrite(relay1, relayState1);
       buttonState2 = digitalRead (button2);
  if (buttonState2 > 0){
    relayState2 = !relayState2;
      } 
      digitalWrite(relay2, relayState2);
  if (buttonState3 > 0){
    relayState3 = !relayState3;
      } 
      digitalWrite(relay3, relayState3);
  }
  
pinMode(relay1, OUTPUT);
pinMode(button1,INPUT_PULLUP);
pinMode(relay2, OUTPUT);
pinMode(button2,INPUT_PULLUP);
pinMode(relay3, OUTPUT);
pinMode(button3,INPUT_PULLUP);
}
//-----------------------------------------------------------------------------------------

//---------------FUNCAO DHT-----------------------------------------------------------------
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // (true) para Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha ao ler sensor DHT11!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  Serial.println("\n");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println("C");

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println("%");
}//-------------------------------------------------------------------------------------------

//------------FUNCAO RELAY--------------------------------------------------------------------
void somefunction(){
  buttonState1 = digitalRead (button1);
    if (buttonState1 < 1 || relayVButton1 > 0)    {
    relayState1 = !relayState1;
    } 
    digitalWrite(relay1, relayState1);
     
  buttonState2 = digitalRead (button2);
    if (buttonState2 < 1  || relayVButton2 > 0){
      relayState2 = !relayState2;
    } 
      digitalWrite(relay2, relayState2);
      
  buttonState3 = digitalRead (button3);
    if (buttonState3 < 1  || relayVButton3 > 0){
      relayState3 = !relayState3;
    } 
      digitalWrite(relay3, relayState3);

//----------------button virtual led----------------
  byte inp = digitalRead(relay1);   
  if (inp == HIGH){
   led1.on();
  }
   else
   led1.off();

  byte inp2 = digitalRead(relay2);   
  if (inp2 == HIGH){
   led2.on();
  }
   else
   led2.off();

  byte inp3 = digitalRead(relay3);   
  if (inp3 == HIGH){
   led3.on();
  }
   else
   led3.off();
}
//--------------leitura pino app-------------------- 
BLYNK_WRITE(V10){
  relayVButton1 = param.asInt();   
}

BLYNK_WRITE(V11){
  relayVButton2 = param.asInt(); 
}
BLYNK_WRITE(V12){
  relayVButton3 = param.asInt(); 
}
//TV ON-OFF
BLYNK_WRITE(V30){
  if(param.asInt()==1){
    irsend.sendNEC(0x20DF10EF,32); 
    delay(300);
    Blynk.virtualWrite(V30, LOW);
  }
}
//TV V-UP
BLYNK_WRITE(V31){
  if(param.asInt()){
    irsend.sendNEC(0x20DF40BF,32); 
  }
}
//TV V-DOWN
BLYNK_WRITE(V32){
  if(param.asInt()){
    irsend.sendNEC(0x20DFC03F,32); 
  }
}
//TV CH-UP
BLYNK_WRITE(V33){
  if(param.asInt()){
    irsend.sendNEC(0x20DF00FF,32); 
  }
}
//TV CH-DOWN
BLYNK_WRITE(V34){
  if(param.asInt()){
    irsend.sendNEC(0x20DF807F,32); 
  }
}
//TV MUTE
BLYNK_WRITE(V35){
  if(param.asInt()==1){
    irsend.sendNEC(0x20DF906F,32);
    delay(300);
    Blynk.virtualWrite(V35, LOW); 
  }
}
//TV INPUT
BLYNK_WRITE(V36){
  if(param.asInt()){
    irsend.sendNEC(0x20DFD02F,32); 
  }
}
//TV EXIT
BLYNK_WRITE(V37){
  if(param.asInt()){
    irsend.sendNEC(0x20DFDA25,32); 
  }
}
//TV OK
BLYNK_WRITE(V38){
  if(param.asInt()){
    irsend.sendNEC(0x20DF22DD,32); 
  }
}
//AC ON-OFF 22
BLYNK_WRITE(V50){
  if(param.asInt()==1){
    irsend.sendRaw(onac22, sizeof(onac22) / sizeof(onac22[0]), 38);
    delay(300);
    Blynk.virtualWrite(V50, LOW); 
  }
}
//AC ON-OFF 24
BLYNK_WRITE(V51){
  if(param.asInt()){
   irsend.sendRaw(onac24, sizeof(onac24) / sizeof(onac24[0]), khz);
   delay(300);
   Blynk.virtualWrite(V51, LOW);
  }
}
//AC 24
BLYNK_WRITE(V54){
  if(param.asInt()){
    irsend.sendRaw(ac24, sizeof(ac24) / sizeof(ac24[0]), khz);
    delay(300);
    Blynk.virtualWrite(V54, LOW);
  }
}
//AC 23
BLYNK_WRITE(V53){
  if(param.asInt()){
    irsend.sendRaw(ac23, sizeof(ac23) / sizeof(ac23[0]), khz);
    delay(300);
    Blynk.virtualWrite(V53, LOW);
  }
}
//AC 22 
BLYNK_WRITE(V52){
  if(param.asInt()){
    irsend.sendRaw(ac22, sizeof(ac22) / sizeof(ac22[0]), khz);
    delay(300);
    Blynk.virtualWrite(V52, LOW);
  }
}
//AC TIMER 24 
BLYNK_WRITE(V55){
  if(param.asInt()){
    irsend.sendRaw(timer24, sizeof(timer24) / sizeof(timer24[0]), khz); 
    delay(300);
    Blynk.virtualWrite(V55, LOW);
  }
}
//AC TIMER 23 
BLYNK_WRITE(V56){
  if(param.asInt()){
    irsend.sendRaw(timer23, sizeof(timer23) / sizeof(timer23[0]), khz);  
    delay(300);
    Blynk.virtualWrite(V55, LOW);
  }
}
//AC TIMER 22 
BLYNK_WRITE(V57){
  if(param.asInt()){
    irsend.sendRaw(timer22, sizeof(timer22) / sizeof(timer22[0]), khz); 
    delay(300);
    Blynk.virtualWrite(V57, LOW); 
  }
}

//------------------------------------------------------------------------------------------------

//----------------------------FUNCAO CLOCK---------------------------------------------------------
void clockDisplay()
{  
  String currentTime = String(hour()) + ":" + minute();
  String currentDate = String(day()) + "/" + month() + "/" + year();
 
  Serial.print("-ONLINE: \n");
  Serial.print("  HORA: ");
  Serial.print(currentTime);
  Serial.print("\n");
  Serial.print("\n");

  // Send time to the App
  Blynk.virtualWrite(V15, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V16, currentDate);
}
//--------------------------------------------------------------------------------------------------
//---------------------------------------GERAL-------------------------------------------
//INVERT LAMPS
BLYNK_WRITE(V1) {
  int estado = param.asInt();
  if (estado == 1) {
    relayState1 = relayState2;
    relayState1 = !relayState1;
    relayState2 = !relayState2;
    relayState3 = relayState2;
  }
  delay(300);
  Blynk.virtualWrite(V1, LOW);
}
//LAMPS OFF
BLYNK_WRITE(V100) {
  if (param.asInt() == 1) {
      if(relayState1 == 1){
          relayState1 = !relayState1;
      }
      if(relayState2 == 1){
          relayState2 = !relayState2;
      }
      if(relayState3 == 1){
          relayState3 = !relayState3;
      }      
      delay(300);
      Blynk.virtualWrite(V100, LOW);
  }
}
//PUSH ALL
BLYNK_WRITE(V101) {
  if (param.asInt() == 1) {
      if(relayState1 == 1){
          relayState1 = !relayState1;
      }
      if(relayState2 == 1){
          relayState2 = !relayState2;
      }
      if(relayState3 == 1){
          relayState3 = !relayState3;
      }
      irsend.sendRaw(onac22, sizeof(onac22) / sizeof(onac22[0]), 38);
      irsend.sendNEC(0x20DF10EF,32); 
      delay(300);
      Blynk.virtualWrite(V101, LOW);
  }  
}
//LAMPS AND TV
BLYNK_WRITE(V102) {
  if (param.asInt() == 1) {
      if(relayState1 == 1){
          relayState1 = !relayState1;
      }
      if(relayState2 == 1){
          relayState2 = !relayState2;
      }
      if(relayState3 == 1){
          relayState3 = !relayState3;
      }
      irsend.sendNEC(0x20DF10EF,32); 
      delay(300);
      Blynk.virtualWrite(V102, LOW);
  }  
}
//LAMPS AND AC
BLYNK_WRITE(V105) {
  if (param.asInt() == 1) {
      if(relayState1 == 1){
          relayState1 = !relayState1;
      }
      if(relayState2 == 1){
          relayState2 = !relayState2;
      }
      if(relayState3 == 1){
          relayState3 = !relayState3;
      }
      irsend.sendRaw(onac22, sizeof(onac22) / sizeof(onac22[0]), 38);      
      delay(300);
      Blynk.virtualWrite(V105, LOW);
  }  
}
//--------------------------------------------------------------------------------------

void loop() {
    Blynk.run(); 
    timer.run();                 
}
