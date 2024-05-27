#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define utsePin_room A0
#define utsePin_gate A1
#define tmpPin_room A2
#define tmpPin_gate A3
// #define utsePin_room A4
// #define utsePin_gate A5
#define ldrPin A6
#define gasPin A7

#define utstPin_gate 13
#define utstPin_room 12 // brown
#define blueLed 110
#define lightPin 10
#define ventilatorPin 90
#define pirPin 8
#define thiefBuzzerPin 7 // temp
#define doorPin 6
#define gatePin 5
#define redLed 4
#define fanPin 3
#define greenLed 2
#define sv3_default 174

#define DHTTYPE DHT11
// Serial not work
// int in1 = 1;
// int in2 = 0;

DHT dht(tmpPin_room, DHTTYPE);
OneWire oneWire(tmpPin_gate);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo1;
Servo myservo2;
Servo myservo3;

// Adafruit_LiquidCrystal lcd(0);
int ldr = 0;
float TEMP = 0;
int cm = 0;
int gas = 0;
int pir = 0;

float CM2 = 0;
float TEMP2 = 0;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT); // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}
void setup()
{ // room tmp
  pinMode(tmpPin_room, INPUT);
  // room gas
  pinMode(gasPin, INPUT);
  // pir
  pinMode(pirPin, INPUT);

  pinMode(fanPin, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  // Serial not work
  // pinMode(in1, OUTPUT);
  // pinMode(in2, OUTPUT);
  // room uts
  pinMode(utstPin_room, OUTPUT);
  pinMode(utsePin_room, INPUT);
  // gate dist
  pinMode(utstPin_gate, OUTPUT);
  pinMode(utsePin_gate, INPUT);
  // nMode(13, OUTPUT);
  pinMode(blueLed, OUTPUT);  // 13
  pinMode(redLed, OUTPUT);   // 4
  pinMode(greenLed, OUTPUT); // 2

  myservo1.attach(doorPin);
  myservo2.attach(ventilatorPin);

  // gate
  myservo3.attach(gatePin); // 10

  Serial.begin(9600);
  dht.begin();
  //  sensors.begin();
  //  lcd.init();
  //  lcd.backlight();
  // lcd.begin(16, 2);
  // Serial.flush();
  // Serial.println();
  // Serial.println();
}
void loop()
{

  delay(1000);
  // lcd.print("Hell");
  //  gate

  // FOR LIGHT ON/OFF
  ldr = analogRead(ldrPin);
  Serial.print("SUN BRIGHTNESS : ");
  Serial.print(ldr);
  if (ldr < 380)
  {
    digitalWrite(lightPin, HIGH);
    Serial.println(" : ROOM DARK ==>> LIGHT ON");
  }
  else
  {
    digitalWrite(lightPin, LOW);
    Serial.println(" : ROOM BRIGHT ==>> LIGHT OFF");
  }

  // FOR FAN ON/OFF
  // TEMP = -40 + 0.488155 * (analogRead(tmpPin_room) - 20);
  TEMP = dht.readTemperature();
  Serial.print("TEMP : ");
  Serial.print(TEMP);

  if (TEMP > 25)
  {
    // digitalWrite(fanPin, HIGH);
    Serial.println(" : HIGH TEMP ==>> FAN ON");
  }
  else
  {
    // digitalWrite(fanPin, LOW);
    Serial.println(" : LOW TEMP ==>> FAN OFF");

    // fan speed control
    // digitalWrite(in1, HIGH);
    // digitalWrite(in2, LOW);
    // for (int i = 0; i < 256; i++) {
    //   analogWrite(fanPin, i);
    //   delay(20);
    // }
  }

  // DOOR OPEN CLOSE
  cm = 0.01723 * readUltrasonicDistance(utstPin_room, utsePin_room);
  Serial.print("DISTANCE(cm) : ");
  Serial.print(cm);
  // myservo1.write(0);
  // delay(500);
  if (cm > 0 && cm < 6)
  {
    myservo1.write(90);
    delay(2000);
    Serial.println(" : PERSON ENTER ==>> DOOR OPEN");
  }
  else
  {
    myservo1.write(0);
    Serial.println(" : NO PERSON ENTER ==>> DOOR CLOSE");
  }

  // GAS BASED VENTILATION OPEN
  gas = analogRead(gasPin);
  // gas = map(gas, 300, 750, 0, 100);
  Serial.print("GAS : ");
  Serial.print(gas);
  myservo2.write(0);
  if (gas > 900)
  {
    myservo2.write(180);
    delay(100);
    Serial.println(" : GAS LEAKAGE ==>> VENTILATOR OPEN");
  }
  else
  {
    // myservo2.write(0);
    Serial.println(" : NO GAS LEAKAGE ==>> VENTILATOR CLOSE");
  }

  // THIEF DETECTION FOR SECURE ROOM
  pir = digitalRead(pirPin);
  Serial.print("THIEF : ");
  Serial.print(pir);
  if (pir == 1)
  {
    digitalWrite(thiefBuzzerPin, HIGH);
    delay(50);
    digitalWrite(thiefBuzzerPin, LOW);
    delay(50);
    digitalWrite(thiefBuzzerPin, HIGH);
    delay(50);
    digitalWrite(thiefBuzzerPin, LOW);
    delay(50);
    digitalWrite(thiefBuzzerPin, HIGH);
    delay(50);
    digitalWrite(thiefBuzzerPin, LOW);
    delay(50);
    digitalWrite(thiefBuzzerPin, HIGH);
    delay(50);
    digitalWrite(thiefBuzzerPin, LOW);
    Serial.println(" : THIEF  ==>> BUZZER ON");
  }
  else
  {
    digitalWrite(thiefBuzzerPin, LOW);
    Serial.println(" : NO THIEF  ==>> BUZZER OFF");
  }
  Serial.println();
}