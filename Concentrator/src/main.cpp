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

// key pulldown
char key;
// Key update speed
const int temp = 15;

/// --------------------- FUNCTION ------------------------ ///
void screenSelect(void);
void keysRead(void);
void keysActions (char);
void savedata(String, String, String, String);

void setup() {
  Serial.begin(115200);
  /// init screen ///
  lcd.begin(16, 2);
  lcd.display("Actimetre");
  lcd.cursor(0,1);
  lcd.display("Initialisation ...");
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
  // Page actimetre
  // 

  while(1){}
}

/// KEYPADE READER FUNCTION ///
void keysRead(){
  bool inputKeyDown = false;
  Serial.println("Start loop");
  while (1){
    for (byte i = 0; i < COLUMNS; i++) {
      digitalWrite(pinColumn[i], HIGH);
      delay(temp);
      for (byte j = 0; j < ROWS; j++){
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

void keysActions (char KEY) {

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

/// DISPLAY TEXT FUNCTION ///
// switch for each page
void screenSelect(){
  switch (expression)
  {
  case /* constant-expression */:
    /* code */
    break;
  
  default:
    break;
  }
}

/// SEND DATA TO SD CARD ///
void savedata(String nEtude, String groupe, String espname ,String data){
  String _path;
  _path = "/" + nEtude;
  if (SD.exists(_path + "/data.csv") == false)
  {
    SD.mkdir(_path);
    Serial.println("file Created");
  }
  
  // Creat text file + open in read mode
  myFile = SD.open(_path + "/data.csv", FILE_WRITE);
  // Header / Column name
  myFile.println("Groupe ; Cage ; Donnee" );
  // Write in file
  myFile.println(groupe + ';' + espname + ';' + data);
  // Close file
  myFile.close();
  
}

/// SEND DATA TO OTHERS ESP32 ///
// Hugo's task
