// Library includes
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <WidgetRTC.h>
#include <IRsend.h>
#include "ir_codes.h"

// Pin defines
#define relay1 D6
#define relay2 D1
#define relay3 D2
#define button1 D4
#define button2 D7
#define button3 D5
#define IR_LED D0
#define RAWBUF 400
#define DHTPIN D3
#define DHTTYPE DHT11

// IR related constants
int khz = 38;
int nbits = 32;

// Timer/timeout values
unsigned int myServerTimeout = 15000; //  Server connection timeout
unsigned int myWiFiTimeout = 15000;   //  WiFi connection timeout
unsigned int blynkInterval = 60000;  // Check server frequency
unsigned int readSensorInterval = 60000;
unsigned int relayControlInterval = 300;
unsigned int clockDisplayInterval = 50000;

// Objects and classes instances
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WidgetRTC rtc;
WidgetLED led1(V20);
WidgetLED led2(V21);
WidgetLED led3(V22);
IRsend irsend(IR_LED);

// Blynk board auth code
char auth[] = "authcode";

// Wifi and Blynk credentials
char ssid[] = "Leo";
char pass[] = "senha";
char server[] = "blynk-cloud.com";

// Pre defined states
int relayVButton1 = 0;
int relayVButton2 = 0;
int relayVButton3 = 0;
int isFirstConnect = true;

// relay 1 = activated on HIGH, relay 2 and 3 = activated on LOW
// all are connected NC
// this way all lamps are OFF (open) when the project is turned ON
boolean relayState1 = 1;
boolean relayState2 = 0;
boolean relayState3 = 0;
boolean buttonState1 = 1;
boolean buttonState2 = 1;
boolean buttonState3 = 1;

// Connection messages
BLYNK_CONNECTED()
{
  if (isFirstConnect)
  {
    Blynk.syncAll();
    isFirstConnect = false;
  }
  rtc.begin();
}

// Setup
void setup()
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(IR_LED, OUTPUT);

  unsigned long startWiFi = millis();

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    if (millis() > startWiFi + myWiFiTimeout)
    {
      break;
    }
  }

  Blynk.config(auth, server);
  checkBlynk();
  dht.begin();

  timer.setInterval(readSensorInterval, sendSensor);     // DHT reading
  timer.setInterval(clockDisplayInterval, clockDisplay); // Real time clock
  timer.setInterval(relayControlInterval, relayControl); // Relay function
  timer.setInterval(blynkInterval, checkBlynk);          // Check blynk function
}

// Check Blynk connection function
void checkBlynk()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    unsigned long startConnecting = millis();
    while (!Blynk.connected())
    {
      Blynk.connect();
      if (millis() > startConnecting + myServerTimeout)
      {
        break;
      }
    }
  }
  if (WiFi.status() != 3)
  {
  }
}

// Relay control function
void relayControl()
{
  buttonState1 = digitalRead(button1);
  if (buttonState1 < 1 || relayVButton1 > 0)
  {
    relayState1 = !relayState1;
  }
  digitalWrite(relay1, relayState1);

  buttonState2 = digitalRead(button2);
  if (buttonState2 < 1 || relayVButton2 > 0)
  {
    relayState2 = !relayState2;
  }
  digitalWrite(relay2, relayState2);

  buttonState3 = digitalRead(button3);
  if (buttonState3 < 1 || relayVButton3 > 0)
  {
    relayState3 = !relayState3;
  }
  digitalWrite(relay3, relayState3);

  // Virtual LED display
  byte inp = digitalRead(relay1);
  if (inp == LOW)
  {
    led1.on();
  }
  else
    led1.off();

  byte inp2 = digitalRead(relay2);
  if (inp2 == HIGH)
  {
    led2.on();
  }
  else
    led2.off();

  byte inp3 = digitalRead(relay3);
  if (inp3 == HIGH)
  {
    led3.on();
  }
  else
    led3.off();
}

// Virtual pin reading
BLYNK_WRITE(V10)
{
  relayVButton1 = param.asInt();
}

BLYNK_WRITE(V11)
{
  relayVButton2 = param.asInt();
}

BLYNK_WRITE(V12)
{
  relayVButton3 = param.asInt();
}

// Turn tv on/off
BLYNK_WRITE(V30)
{
  if (param.asInt() == 1)
  {
    irsend.sendNEC(tv_onoff, nbits);
    delay(300);
    Blynk.virtualWrite(V30, LOW);
  }
}

// Turn tv volume up
BLYNK_WRITE(V31)
{
  if (param.asInt())
  {
    irsend.sendNEC(tv_volumeup, nbits);
  }
}

// Turn tv volume down
BLYNK_WRITE(V32)
{
  if (param.asInt())
  {
    irsend.sendNEC(tv_volumedown, nbits);
  }
}

// Turn tv channel up
BLYNK_WRITE(V33)
{
  if (param.asInt())
  {
    irsend.sendNEC(tv_chup, nbits);
  }
}

// Turn tv channel down
BLYNK_WRITE(V34)
{
  if (param.asInt())
  {
    irsend.sendNEC(tv_chdown, nbits);
  }
}

// Tv mute button
BLYNK_WRITE(V35)
{
  if (param.asInt() == 1)
  {
    irsend.sendNEC(tv_mute, nbits);
    delay(300);
    Blynk.virtualWrite(V35, LOW);
  }
}

// Tv input button
BLYNK_WRITE(V36)
{
  if (param.asInt())
  {
    irsend.sendNEC(tv_input, nbits);
  }
}

// Tv exit button
BLYNK_WRITE(V37)
{
  if (param.asInt())
  {
    irsend.sendNEC(tv_exit, nbits);
  }
}

// Tv ok button
BLYNK_WRITE(V38)
{
  if (param.asInt())
  {
    irsend.sendNEC(tv_ok, nbits);
  }
}

// Turn AC on/off 18C
BLYNK_WRITE(V18)
{
  if (param.asInt() == 1)
  {
    irsend.sendRaw(onac18, sizeof(onac18) / sizeof(onac18[0]), khz);
    delay(300);
    Blynk.virtualWrite(V18, LOW);
  }
}

// Turn AC on/off 22C
BLYNK_WRITE(V50)
{
  if (param.asInt() == 1)
  {
    irsend.sendRaw(onac22, sizeof(onac22) / sizeof(onac22[0]), khz);
    delay(300);
    Blynk.virtualWrite(V50, LOW);
  }
}

// Turn AC on/off 24C
BLYNK_WRITE(V51)
{
  if (param.asInt())
  {
    irsend.sendRaw(onac24, sizeof(onac24) / sizeof(onac24[0]), khz);
    delay(300);
    Blynk.virtualWrite(V51, LOW);
  }
}

// Change AC temperature 24C
BLYNK_WRITE(V54)
{
  if (param.asInt())
  {
    irsend.sendRaw(ac24, sizeof(ac24) / sizeof(ac24[0]), khz);
    delay(300);
    Blynk.virtualWrite(V54, LOW);
  }
}

// Change AC temperature 23C
BLYNK_WRITE(V53)
{
  if (param.asInt())
  {
    irsend.sendRaw(ac23, sizeof(ac23) / sizeof(ac23[0]), khz);
    delay(300);
    Blynk.virtualWrite(V53, LOW);
  }
}

// Change AC temperature 22C
BLYNK_WRITE(V52)
{
  if (param.asInt())
  {
    irsend.sendRaw(ac22, sizeof(ac22) / sizeof(ac22[0]), khz);
    delay(300);
    Blynk.virtualWrite(V52, LOW);
  }
}

// Change AC temperature 18C
BLYNK_WRITE(V19)
{
  if (param.asInt())
  {
    irsend.sendRaw(ac18, sizeof(ac18) / sizeof(ac18[0]), khz);
    delay(300);
    Blynk.virtualWrite(V19, LOW);
  }
}

// Ac timer, temperature 24C, 3 hours
BLYNK_WRITE(V55)
{
  if (param.asInt())
  {
    irsend.sendRaw(ac24, sizeof(ac24) / sizeof(ac24[0]), khz);
    delay(300);
    Blynk.virtualWrite(V55, LOW);
  }
}

// Ac timer, temperature 23C, 3 hours
BLYNK_WRITE(V56)
{
  if (param.asInt())
  {
    irsend.sendRaw(ac23, sizeof(ac23) / sizeof(ac23[0]), khz);
    delay(300);
    Blynk.virtualWrite(V55, LOW);
  }
}

// Ac timer, temperature 22C, 3 hours
BLYNK_WRITE(V57)
{
  if (param.asInt())
  {
    irsend.sendRaw(ac22, sizeof(ac22) / sizeof(ac22[0]), khz);
    delay(300);
    Blynk.virtualWrite(V57, LOW);
  }
}

// Clock display
void clockDisplay()
{
  String currentTime = String(hour()) + ":" + minute();
  String currentDate = String(day()) + "/" + month() + "/" + year();

  Blynk.virtualWrite(V15, currentTime);
  Blynk.virtualWrite(V16, currentDate);
}

// DHT temperature and humidity read
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // (true) to Fahrenheit

  Blynk.run();

  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  if (isnan(h) || isnan(t))
  {
    return;
  }
}

// Invert lamps state
BLYNK_WRITE(V1)
{
  int estado = param.asInt();
  if (estado == 1)
  {
    relayState1 = relayState2;
    relayState1 = !relayState1;
    relayState2 = !relayState2;
    relayState3 = relayState2;
  }

  delay(300);
  Blynk.virtualWrite(V1, LOW);
}

// Turn off lamps
BLYNK_WRITE(V100)
{
  if (param.asInt() == 1)
  {
    if (relayState1 == 1)
    {
      relayState1 = !relayState1;
    }
    if (relayState2 == 1)
    {
      relayState2 = !relayState2;
    }
    if (relayState3 == 1)
    {
      relayState3 = !relayState3;
    }

    delay(300);
    Blynk.virtualWrite(V100, LOW);
  }
}

// Turn off lamps, turn on/off tv, turn on/off AC
BLYNK_WRITE(V101)
{
  if (param.asInt() == 1)
  {
    if (relayState1 == 1)
    {
      relayState1 = !relayState1;
    }
    if (relayState2 == 1)
    {
      relayState2 = !relayState2;
    }
    if (relayState3 == 1)
    {
      relayState3 = !relayState3;
    }
    irsend.sendRaw(onac22, sizeof(onac22) / sizeof(onac22[0]), khz);
    irsend.sendNEC(tv_onoff, nbits);

    delay(300);
    Blynk.virtualWrite(V101, LOW);
  }
}

// Turn off lamps, turn on/off tv
BLYNK_WRITE(V102)
{
  if (param.asInt() == 1)
  {
    if (relayState1 == 1)
    {
      relayState1 = !relayState1;
    }
    if (relayState2 == 1)
    {
      relayState2 = !relayState2;
    }
    if (relayState3 == 1)
    {
      relayState3 = !relayState3;
    }
    irsend.sendNEC(tv_onoff, nbits);

    delay(300);
    Blynk.virtualWrite(V102, LOW);
  }
}

// Turn off lamps, turn on/off AC
BLYNK_WRITE(V105)
{
  if (param.asInt() == 1)
  {
    if (relayState1 == 1)
    {
      relayState1 = !relayState1;
    }
    if (relayState2 == 1)
    {
      relayState2 = !relayState2;
    }
    if (relayState3 == 1)
    {
      relayState3 = !relayState3;
    }
    irsend.sendRaw(onac22, sizeof(onac22) / sizeof(onac22[0]), khz);

    delay(300);
    Blynk.virtualWrite(V105, LOW);
  }
}

// Main Loop
void loop()
{
  timer.run();
  if (Blynk.connected())
  {
    Blynk.run();
  }
}
