#define UP_BTN_PIN 12
#define DOWN_BTN_PIN 11
#define BACK_BTN_PIN 10
#define SELECT_BTN_PIN 9
#define BUZZER_PIN 8
#define RED_LED_PIN 7
#define GREEN_LED_PIN 6 

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte currentMenuItem = 0;

// 0 kikapcsolt
// 1 élesített
byte state = 0;

bool printedArmed = false;

String menu[] = {
  "Kikapcsol",
  "Elesit"
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
      monitorMode();
      break;
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
      state = 1;
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
