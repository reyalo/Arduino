#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DHTTYPE DHT11

DHT dht(A2, DHTTYPE);
OneWire oneWire(A3);
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
  pinMode(A2, INPUT);
  // room gas
  pinMode(A7, INPUT);
  // pir
  pinMode(8, INPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  // room uts
  pinMode(12, OUTPUT);
  pinMode(A0, INPUT);
  // gate dist
  pinMode(13, OUTPUT);
  pinMode(A1, INPUT);
  // nMode(13, OUTPUT);
  pinMode(11, OUTPUT);  // 13
  pinMode(4, OUTPUT);   // 4
  pinMode(2, OUTPUT); // 2

  myservo1.attach(6);
  myservo2.attach(5);

  // gate
  myservo3.attach(3); // 10

  Serial.begin(9600);
  dht.begin();
  sensors.begin();
  lcd.init();
  lcd.backlight();
  // lcd.begin(16, 2);
  // Serial.flush();
  // Serial.println();
  // Serial.println();
}

void loop()
{
  // delay(1000);
  // lcd.print("Hell");
  //  gate
  CM2 = 0.01723 * readUltrasonicDistance(13, A1);
  // lcd.print("DIST : ");
  // lcd.print(CM2);
  Serial.print("DISTANCE(cm) : ");
  Serial.println(CM2);

  if (CM2 < 15)
  {
    delay(500);

    // TEMP2 = -40 + 0.488155 * (analogRead(A3) - 20); // lm35
    // TEMP2 = analogRead(A3); // 7 in nano  //ds18
    sensors.requestTemperatures();
    TEMP2 = sensors.getTempCByIndex(0);
    Serial.println(TEMP2);
    lcd.setCursor(0, 0);
    lcd.print("Temparature:");
    lcd.print(TEMP2);
    // Serial.print("Tmp:");
    // Serial.print(TEMP2);
    lcd.setCursor(0, 1);
    if (TEMP2 <= 37.4)
    {
      lcd.print("Celsius-NORMAL");
      Serial.println("Celsius-NORMAL");
    }
    else
    {
      lcd.print("Celsius-HIGH");
      Serial.println("Celsius-HIGH");
    }

    digitalWrite(11, HIGH);
    Serial.println("SANITIZING");
    delay(100);
    digitalWrite(11, LOW);

    if (TEMP2 <= 37.4)
    {
      myservo3.write(0);
      Serial.println("Doar Open");
      digitalWrite(2, HIGH);
      delay(2000);
      myservo3.write(90);
    }
    else
    {
      myservo3.write(90);
      Serial.println("Doar Not Open");
      digitalWrite(4, HIGH);
      tone(4, 523, 1000);
      delay(2000);
    }

    digitalWrite(4, LOW);
    digitalWrite(2, LOW);
    myservo3.write(90);
    lcd.clear();
    Serial.println();
    delay(1000);
  }

  else
  {
    lcd.print("PUT YOUR HAND");
    delay(500);
    lcd.clear();
    Serial.println();
  }

  // end gate

  // FOR LIGHT ON/OFF
  ldr = analogRead(A6);
  Serial.print("SUN BRIGHTNESS : ");
  Serial.print(ldr);
  if (ldr < 380)
  {
    digitalWrite(10, HIGH);
    Serial.println(" : ROOM DARK ==>> LIGHT ON");
  }
  else
  {
    digitalWrite(10, LOW);
    Serial.println(" : ROOM BRIGHT ==>> LIGHT OFF");
  }

  // FOR FAN ON/OFF
  // TEMP = -40 + 0.488155 * (analogRead(A2) - 20);
  TEMP = dht.readTemperature();
  Serial.print("TEMP : ");
  Serial.print(TEMP);
  if (TEMP > 30)
  {
    digitalWrite(9, HIGH);
    Serial.println(" : HIGH TEMP ==>> FAN ON");
  }
  else
  {
    digitalWrite(9, LOW);
    Serial.println(" : LOW TEMP ==>> FAN OFF");
  }

  // DOOR OPEN CLOSE
  cm = 0.01723 * readUltrasonicDistance(12, A0);
  Serial.print("DISTANCE(cm) : ");
  Serial.print(cm);
  myservo1.write(0);
  if (cm < 120)
  {
    myservo1.write(90);
    delay(100);
    Serial.println(" : PERSON ENTER ==>> DOOR OPEN");
  }
  else
  {
    myservo1.write(0);
    Serial.println(" : NO PERSON ENTER ==>> DOOR CLOSE");
  }

  // GAS BASED VENTILATION OPEN
  gas = analogRead(A7);
  // gas = map(gas, 300, 750, 0, 100);
  Serial.print("GAS : ");
  Serial.print(gas);
  myservo2.write(0);
  if (gas > 900)
  {
    myservo2.write(360);
    delay(100);
    Serial.println(" : GAS LEAKAGE ==>> VENTILATOR OPEN");
  }
  else
  {
    myservo2.write(0);
    Serial.println(" : NO GAS LEAKAGE ==>> VENTILATOR CLOSE");
  }

  // THIEF DETECTION FOR SECURE ROOM
  pir = digitalRead(8);
  Serial.print("THIEF : ");
  Serial.print(pir);
  if (pir == 1)
  {
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);
    delay(50);
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);
    delay(50);
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);
    delay(50);
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);
    Serial.println(" : THIEF  ==>> BUZZER ON");
  }
  else
  {
    digitalWrite(7, LOW);
    Serial.println(" : NO THIEF  ==>> BUZZER OFF");
  }
  Serial.println();
}