#define UP_BTN_PIN 8
#define DOWN_BTN_PIN 7
#define BACK_BTN_PIN 6
#define SELECT_BTN_PIN 5
#define BUZZER_PIN 11
#define RED_LED_PIN 9
#define GREEN_LED_PIN 10 
#define sensorPin A0

#include "pitches.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte currentMenuItem = 0;

float alpha = 0.75;
int period = 50;
float max = 0.0;

// 0 kikapcsolt
// 1 élesített
byte state = 0;

bool printedArmed = false;

String menu[] = {
  "Szervusz",
  "Meres",
  "Zene"
};

int melody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5
};

int noteDurations[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
};

int menuSize = sizeof(menu) / sizeof(menu[0]);


// https://maxpromer.github.io/LCD-Character-Creator/
byte leftArrow[] = {0x00,0x04,0x08,0x1F,0x08,0x04,0x00,0x00};
byte upArrow[] = {0x00,0x04,0x0E,0x15,0x04,0x04,0x04,0x00};
byte downArrow[] = {0x00,0x04,0x04,0x04,0x15,0x0E,0x04,0x00};

void setup() {
  // put your setup code here, to run once:

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(UP_BTN_PIN, INPUT);
  pinMode(DOWN_BTN_PIN, INPUT);
  pinMode(BACK_BTN_PIN, INPUT);
  pinMode(SELECT_BTN_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, leftArrow);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);

  updateMenu();
}

void loop() {
  // put your main code here, to run repeatedly:

  switch (state) {
    case 0:
      checkButtons();
      break;
    case 1:
      heartrate();
      break;
    case 2:
      music();
      break;
  }
}

void updateMenu() {
  
  lcd.clear();

  lcd.setCursor(0, 0);

  // ha páros indexű a menüelem
  if ((currentMenuItem % 2) == 0) {
    // ha az utolsó elem
    if (currentMenuItem == menuSize - 1) {
      lcd.setCursor(0, 0);
      lcd.print(menu[currentMenuItem]);
      lcd.write(0);
    }
    // ha nem az utolsó elem
    if (currentMenuItem < menuSize - 1) {
      lcd.print(menu[currentMenuItem]);
      lcd.write(0);
      lcd.setCursor(0, 1);
      lcd.print(menu[currentMenuItem + 1]);
    }

    // ha nem az utolsó előtti elem
    if (currentMenuItem < menuSize - 2) {
      lcd.setCursor(15, 1);
      lcd.write(2);
    }

  }
  // ha páratlan indexű a menüelem
  else {
    lcd.print(menu[currentMenuItem - 1]);
    lcd.setCursor(0, 1);
    lcd.print(menu[currentMenuItem]);
    lcd.write(0);

    // utolsó előtti elem előtti elemek
    if (currentMenuItem <= menuSize - 2) {
      lcd.setCursor(15, 1);
      lcd.write(2);
    }

  }

  // ha van annyi elem, hogy 2 sornál többet tölt ki
  if (currentMenuItem > 1) {
    lcd.setCursor(15, 0);
    lcd.write(1);
  }

}

void checkButtons() {

  if(buttonPressed(UP_BTN_PIN)) {
    menuMoveUp();
    updateMenu();
  }

  if(buttonPressed(DOWN_BTN_PIN)) {
    menuMoveDown();
    updateMenu();
  }

  if(buttonPressed(SELECT_BTN_PIN)) {
    executeSelected();
  }

}

void monitorMode() {

  if (!printedArmed) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Elesitve");
    printedArmed = true;
  }


  // ide jön még a PIR szenzor figyelése

  digitalWrite(RED_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
  delay(1000);
  
}

int buttonPressed(uint8_t button) {
  byte prevState = LOW;
  byte state = digitalRead(button);
  if (state != prevState) {
    prevState = state;
    if (state == HIGH) {
      delay(100);
      return true;
    }
  }
  return false;
}

void music(){
  for (int thisNote = 0; thisNote < 112; thisNote++) {
    if(buttonPressed(BACK_BTN_PIN) )
      {
         updateMenu();
         state = 0;
         break;
      }
    int noteDuration = 750 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
    Serial.println(melody[thisNote]);
   }
}
void menuMoveUp() {
  if (currentMenuItem > 0) {
      currentMenuItem--;
    }
  else {
    currentMenuItem = menuSize - 1;
  }
}

void menuMoveDown() {
  if (currentMenuItem < menuSize - 1) {
      currentMenuItem++;
    }
  else {
    currentMenuItem = 0;
  }
}

void executeSelected() {
  switch(currentMenuItem) {
    case 0:
      state = 0;
      break;
    case 1:
      lcd.clear();
      state = 1;
      break;
    case 2:
      state = 2;
      break;
  }
}

void heartrate(){
  // Arbitrary initial value for the sensor value (0 - 1023)
  // too large and it takes a few seconds to 'lock on' to pulse
  static float oldValue = 500;

  // Time recording for BPM (beats per minute)
  static unsigned long bpmMills = millis();
  static int bpm = 0;

  // Keep track of when we had the the last pulse - ignore
  // further pulses if too soon (probably false reading)
  static unsigned long timeBetweenBeats = millis();
  int minDelayBetweenBeats = 400;

  // This is generic code provided with the board:
  // Read the sensor value (0 - 1023)
  int rawValue = analogRead((unsigned char) sensorPin);

  // Some maths (USA: math) to determine whether we are detected a peak (pulse)
  float value = alpha * oldValue + (1 - alpha) * rawValue;
  float change = value - oldValue;
  oldValue = value;

  // Forum suggested improvement (works very well)
  // Display data on the LED via a blip:
  // Empirically, if we detect a peak as being X% from
  // absolute max, we find the pulse even when amplitude
  // varies on the low side.

  // if we find a new maximum value AND we haven't had a pulse lately
  if ((change >= max) && (millis() > timeBetweenBeats + minDelayBetweenBeats)) {

    // Reset max every time we find a new peak
    max = change;

    // Flash LED and beep the buzzer
    digitalWrite(GREEN_LED_PIN, 1);
    tone(BUZZER_PIN, 2000, 50);

    // Reset the heart beat time values
    timeBetweenBeats = millis();
    bpm++;
  }
  else {
    // No pulse detected, ensure LED is off (may be off already)
    digitalWrite(RED_LED_PIN, 0);
  }
  // Slowly decay max for when sensor is moved around
  // but decay must be slower than time needed to hit
  // next pulse peak. Originally: 0.98
  max = max * 0.97;

  // Every 15 seconds extrapolate the pulse rate. Improvement would
  // be to average out BPM over last 60 seconds
  if (millis() >= bpmMills + 15000) {
    lcd.print("BPM (approx): ");
    lcd.println(bpm * 4);
    lcd.println();
    bpm = 0;
    bpmMills = millis();
  }
  if(buttonPressed(BACK_BTN_PIN))
  {
    updateMenu();
    state = 0;
    digitalWrite(GREEN_LED_PIN,0);
  }
  // Must delay here to give the value a chance to decay
  delay(period);
}
