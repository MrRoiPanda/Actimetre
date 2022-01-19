#include <Arduino.h>
#include "rgb_lcd.h"
// Class
rgb_lcd lcd;

/// ---------------- Matrice ---------------- ///
// Creat
const byte ROWS = 4;    // 4 lignes
const byte COLUMNS = 4; // 4 colonnes

// keypad mapping
const char hexKeypad[COLUMNS][ROWS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// IO Pins
const int16_t pinRows[ROWS] = {35, 34, 39, 36};      // entrée
const int16_t pinColumn[COLUMNS] = {17, 16, 33, 32}; // sortie

// 
char key;
const int temp = 15; 

// Key utilities 
void keysActions(char);

void setup() {
  Serial.begin(115200);
  // LCD size
  lcd.begin(16, 2);
  // Input
  pinMode(35, INPUT);
  pinMode(34, INPUT);
  pinMode(39, INPUT);
  pinMode(36, INPUT);

  for (byte i = 0; i < ROWS; i++)
  {
    digitalWrite(pinRows[i], LOW);
  }
  // Output
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  for (byte i = 0; COLUMNS < 4; i++)
  {
    digitalWrite(pinColumn[i], LOW);
  }

  Serial.println("End setup");
  delay(10);
}

void loop() {
  // Read keypad
  bool inputKeyDown = false;
  Serial.println("Start loop");
  while (1)
  {
    for (byte i = 0; i < COLUMNS; i++)
    {
      digitalWrite(pinColumn[i], HIGH);
      delay(temp);
      for (byte j = 0; j < ROWS; j++)
      {
        inputKeyDown = digitalRead(pinRows[j]);
        if (inputKeyDown == true)
        {
          key = hexKeypad[j][i];
          keysActions(key);
          inputKeyDown = false;
        }
        delay(temp);
      }
      digitalWrite(pinColumn[i], LOW);
    }
  }
}

void keysActions(char KEY) {
  switch (KEY)
  {
  case '#':
    lcd.clear();
    break;

  default:
    lcd.print(KEY);
    break;
  }
}