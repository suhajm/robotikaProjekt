// http://forum.arduino.cc/index.php?topic=209140.30

#include "Arduino.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Using built in LED pin for demo
#define ledPin 13
#define clear_button 9

// Pulse meter connected to any Analog pin
#define sensorPin A0
#define buzzer 11

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Values from provided (eBay) code
float alpha = 0.75;
int period = 50;
float max = 0.0;

// ------------------------------------------------------------
// SETUP      SETUP      SETUP      SETUP      SETUP      SETUP
// ------------------------------------------------------------
void setup() {

	// Inbuilt LED
	pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(clear_button, INPUT);


  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
}

// ------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// ------------------------------------------------------------
void loop() {
	static float oldValue = 500;

	static unsigned long bpmMills = millis();
	static int bpm = 0;
 
	static unsigned long timeBetweenBeats = millis();
	int minDelayBetweenBeats = 400;

	int rawValue = analogRead((unsigned char) sensorPin);

	float value = alpha * oldValue + (1 - alpha) * rawValue;
	float change = value - oldValue;
	oldValue = value;

	if ((change >= max) && (millis() > timeBetweenBeats + minDelayBetweenBeats)) {

		// Reset max every time we find a new peak
		max = change;

		// Flash LED and beep the buzzer
		//digitalWrite(ledPin, 1);
		tone(buzzer, 2000, 50);

		timeBetweenBeats = millis();
		bpm++;
	}
	else {
		digitalWrite(ledPin, 0);
	}
	max = max * 0.97;

	if (millis() >= bpmMills + 15000) {
		lcd.print("BPM (approx): ");
		lcd.println(bpm * 4);
    lcd.println();
		bpm = 0;
		bpmMills = millis();
	}

	delay(period);
}

void buzz(){
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec
}
