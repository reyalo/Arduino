#include <Adafruit_LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

#define utsePin_room A1
#define utsePin_gate A0

#define utstPin_room 13
#define utstPin_gate 12

LiquidCrystal_I2C lcd(0x27, 20, 4);

float CM2 = 0;



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


void setup() {
  // put your setup code here, to run once:


  pinMode(utstPin_room, OUTPUT);
  pinMode(utsePin_room, INPUT);
  // gate dist
  pinMode(utstPin_gate, OUTPUT);
  pinMode(utsePin_gate, INPUT);


  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  // lcd.println("DIST : ");
  // Serial.flush();
  // Serial.println();
  // Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.setCursor(0, 0);


  CM2 = 0.01723 * readUltrasonicDistance(utstPin_gate, utsePin_gate);
  lcd.print("DIST : ");
  lcd.print(CM2);
  
  // CM2=14;
  // Serial.print("DISTANCE(cm) : ");
  // Serial.println(CM2);

  delay(500);

}
