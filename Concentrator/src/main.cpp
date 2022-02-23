#include <Arduino.h>
#include "rgb_lcd.h"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>

/// --------------------- SD CARD ------------------------- ///
File myFile;

/// --------------------- SCREEN -------------------------- ///
rgb_lcd lcd;

/// --------------------- KEYPAD -------------------------- ///
const byte ROWS = 4;    // 4 lignes
const byte COLUMNS = 4; // 4 colonnes

// Matrice
const char hexKeypad[COLUMNS][ROWS] = {
	{'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

const int16_t pinRows[ROWS] = {35, 34, 39, 36};        // IN
const int16_t pinColumn[COLUMNS] = {17, 16, 33, 32};   // OUT

void setup() {
  Serial.begin(115200);

  /// init keypad ///
  for (byte i = 0; i < ROWS; i++)
  {
		digitalWrite(pinRows[i],LOW);
  }
  pinMode( 35, INPUT);
  pinMode( 34, INPUT);
  pinMode( 39, INPUT);
  pinMode( 36, INPUT);

  // column output
  for (byte i = 0; COLUMNS < 4; i++)
  {
		digitalWrite(pinColumn[i],LOW);
  }
  pinMode( 32, OUTPUT);
  pinMode( 33, OUTPUT);
  pinMode( 16, OUTPUT);
  pinMode( 17, OUTPUT);

  /// INIT SD CARD PIN ///
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing SD card...");

  if (!SD.begin(5)) {
    Serial.println("initialization failed!");
    while (1);
  }

  Serial.println("initialization done.");

  delay(50);
}

void loop() {
  
}

/// KEYPADE READER FUNCTION ///

/// DISPLAY TEXT FUNCTION ///
// switch for each page 

/// SEND DATA TO SD CARD ///

/// SEND DATA TO OTHERS ESP32 ///
