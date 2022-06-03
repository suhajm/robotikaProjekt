#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define upButton 8
#define downButton 7
#define selectButton 6
#define menuButton 5
#define buzzer 11

int menu = 1;

void setup() {
   Serial.begin(9600);
   lcd.init();
   lcd.backlight();
   pinMode(upButton, INPUT_PULLUP);
   pinMode(downButton, INPUT_PULLUP);
   pinMode(selectButton, INPUT_PULLUP);
   pinMode(menuButton, INPUT_PULLUP);
   pinMode(buzzer, OUTPUT);
   updateMenu();
}
void loop ()
{
  if(digitalRead(upButton) == 1)
  {
    digitalWrite(buzzer, HIGH);
  }
  else {
    digitalWrite(buzzer, LOW);
  }
  /*if (!digitalRead(downButton)){
    menu++;
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)){
    menu--;
    updateMenu();
    delay(100);
    while(!digitalRead(upButton));
  }
  if (!digitalRead(selectButton)){
    executeAction();
    updateMenu();
    delay(100);
    while (!digitalRead(selectButton));
  }*/
} 

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">MenuItem1");
      lcd.setCursor(0, 1);
      lcd.print(" MenuItem2");
      break;
    case 2:
      lcd.clear();
      lcd.print(" MenuItem1");
      lcd.setCursor(0, 1);
      lcd.print(">MenuItem2");
      break;
    case 3:
      lcd.clear();
      lcd.print(">MenuItem3");
      lcd.setCursor(0, 1);
      lcd.print(" MenuItem4");
      break;
    case 4:
      lcd.clear();
      lcd.print(" MenuItem3");
      lcd.setCursor(0, 1);
      lcd.print(">MenuItem4");
      break;
    case 5:
      menu = 4;
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
  }
}

void action1() {
  lcd.clear();
  lcd.print(">Executing #1");
  delay(1500);
}
void action2() {
  lcd.clear();
  lcd.print(">Executing #2");
  delay(1500);
}
void action3() {
  lcd.clear();
  lcd.print(">Executing #3");
  delay(1500);
}
void action4() {
  lcd.clear();
  lcd.print(">Executing #4");
  delay(1500);
}
