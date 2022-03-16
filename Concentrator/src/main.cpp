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

// key pulldown - use in keysRead();
// char key;

// Characters displayed on screen
char Data[16];
// Characters count
byte keysCount;
// Screen number
int indice = 0x00;
// key return
char action;
// Key update speed | laste value 15
const int temp = 15;

// Number of cage
byte maxCage = 32;
byte emptyCage = 32;

// Data need to be saved
int ETUDE;
String TECHID;
byte ANIMALS;
byte ANUMBRE;
int TSTART;
int TESTDURATION;
byte BRIGHTNESS;

/// --------------------- FUNCTION ------------------------ ///

char keysRead(bool);
void keysActions(char);
void testCreation(void);
void saveData(String, String, String, String);
String toSring(void);

/// ----------------------- SETUP ------------------------- /// 

void setup() {
  Serial.begin(115200);
  /// init screen ///
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Actimetre       ");
  lcd.setCursor(0,1);
  lcd.print("Initialisation  ");
  /// init keypad ///
  // raws input
  pinMode( 35, INPUT_PULLDOWN);
  pinMode( 34, INPUT_PULLDOWN);
  pinMode( 39, INPUT_PULLDOWN);
  pinMode( 36, INPUT_PULLDOWN);

  for (byte i = 0; i < ROWS; i++)
  {
		digitalWrite(pinRows[i],LOW);
  }

  // column output
  pinMode( 32, OUTPUT);
  pinMode( 33, OUTPUT);
  pinMode( 16, OUTPUT);
  pinMode( 17, OUTPUT);

  for (byte i = 0; COLUMNS < 4; i++)
  {
		digitalWrite(pinColumn[i],LOW);
  }

  /// INIT SD CARD PIN ///
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Initializing SD card...");
  /*
  if (!SD.begin(5)) {
    Serial.println("initialization failed!");
    //while (1);
  }
  */
  Serial.println("initialization done.");
  /// --------------- ///
  delay(5000);
  lcd.clear();
}

void loop() {

  testCreation();
  
}

/// KEYPADE READER FUNCTION ///
char keysRead(bool ActiveKeypad){
  char key = 0x00;
  bool inputKeyDown = false;
  Serial.println("Start read");
  
  while ((key != '#') && (key != '*') && (key != 'A') & (key != 'B') && (key != 'C')) {

    for (byte i = 0; i < COLUMNS; i++) {
      digitalWrite(pinColumn[i], HIGH);
      delay(temp);

      for (byte j = 0; j < ROWS; j++) {
        inputKeyDown = digitalRead(pinRows[j]);

        if (inputKeyDown == true) {
          key = hexKeypad[j][i];
          
          if (ActiveKeypad == true) {
            keysActions(key);
          }
          Serial.print(key);
          
          inputKeyDown = false;
        }
        delay(temp);
      }
      digitalWrite(pinColumn[i], LOW);
    }

  }
  Serial.println("return active");
  return key;
}

/// READ SPECIAL KEYS ///


/// WRITE NUMBER


/// DISPLAY KEY FUNCTION ///
void keysActions(char _key){

  if (_key == 'D'){

    while (keysCount != 0) {
      Data[keysCount--] = 0; //clear last key
    }

  } else {

    /* display _key */
    Data[keysCount] = _key;
    lcd.setCursor(keysCount,1);
    lcd.print(Data[keysCount]);
    keysCount++;
  }

}

/// DISPLAY TEXT FUNCTION ///
// switch for each page
void testCreation() {

  switch (indice) {

    case 0x00:
      lcd.clear();
      /// Maine menu ///
      lcd.setCursor(0,0);
      lcd.print("A- Configuration");
      lcd.setCursor(0,1);
      lcd.print("         B-Help!");
      // Waiting key A or B
      action = keysRead(false);
      if (action == 'A'){
        indice++;
      } else if (action == 'B'){
        indice = 0x10;
      }
    break;

    case 0x01:
      lcd.clear();
      /// Test Settings ///
      lcd.setCursor(0,0);
      lcd.print("\"#\" = Retour  ");
      lcd.setCursor(0,1);
      lcd.print("         *-Start");
      // Waiting key # or *
      action = keysRead(false);
      if (action == '#') {
        indice--;
      } else if (action == '*') {
        indice++;
      }
    break;

    case 0x02:
      lcd.clear();
      /// N° Etude ///
      lcd.setCursor(0,0);
      lcd.print("Numero d'Etude");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      action = keysRead(true);
      if (action == '#') {
        indice--;
      } else if (action == '*') {
        String STUDY = String(Data);
        indice++;
      }

    break;

    case 0x03:
      lcd.clear();
      /// Paraphe ///
      lcd.setCursor(0,0);
      lcd.print("Paraphe  ");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      if (action == '#') {
        indice--;
      } else if (action == '*'){
        String TECHID = String(Data);
        indice++;
      }
    break;

    case 0x04:
      lcd.clear();
      /// Animal ///
      lcd.setCursor(0,0);
      lcd.print("  Quel Animal ? ");
      lcd.setCursor(0,0);
      lcd.print("A-Rats    B-Mice");
      // Waiting key # or A or B
      action = keysRead(false);
      if (action == '#') {
        indice--;
      } else if(action == 'A') {
        ANIMALS = 1;
        indice++;
      } else if(action == 'B') {
        ANIMALS = 0;
        indice++;
      }

    break;

    case 0x05:
      lcd.clear();
      /// Number of Mice/Rats ///
      lcd.setCursor(0,0);
      lcd.print("combien ?");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      action = keysRead(true);
      if (action == '#')
      {
        indice--;
      } else if (action == '*'){
        
        ANUMBRE = (byte) (Data[0] - '0') + (Data[1] - '0')*10;
        Serial.println(ANUMBRE);
        if ((ANUMBRE > maxCage) || (ANUMBRE < emptyCage)) {
          indice = 0xEE;
        } else {
          indice++;
        }
        
      }
    break;

    case 0x06:
      lcd.clear();
      /// Start hour ///
      lcd.setCursor(0,0);
      lcd.print("Start hour    ");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      action = keysRead(true);
      if (action == '#') {
        indice--;
      } else if (action == '*'){
        indice++;
      }

    break;

    case 0x07:
      lcd.clear();
      /// duration ///
      lcd.setCursor(0,0);
      lcd.print("duration");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      action = keysRead(true);
      if (action == '#') {
        indice--;
      } else if (action == '*'){
        indice++;
      }
    break;

    case 0x08:
      lcd.clear();
      /// Brightness ///
      lcd.setCursor(0,0);
      lcd.print("Brightness  ");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      action = keysRead(true);
      if (action == '#') {
        indice--;
      } else if (action == '*'){
        indice++;
      }

    break;

    case 0x09:
      lcd.clear();
      /// Group ID ///
      lcd.setCursor(0,0);
      lcd.print("Group ID  ");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      action = keysRead(true);
      if (action == '#') {
        indice--;
      } else if (action == '*'){
        indice++;
      }

    break;

    case 0x0A:
      lcd.clear();
      /// Start Test ///
      lcd.setCursor(0,0);
      lcd.print("\"*\" = Start  ");
      // Waiting key # or *
      action = keysRead(false);
      if (action == '#') {
        indice--;
      } else if (action == '*'){
        indice = 0xFF;
      }
    break;


    case 0x10:
      lcd.clear();
      /// Help ///
      lcd.setCursor(0,0);
      lcd.print("Control Settings");
      lcd.setCursor(0,1);
      lcd.print("*-Next  #-Retour");
      // Waiting key # or *
      action = keysRead(false);
      if (action == '#') {
        indice = 0x00;
      }

    break;
    
    // ERREUR
    case 0xEE:
      lcd.clear();
      // Erreur not enough cage
      lcd.setCursor(0,0);
      lcd.print("    ERROR !!    ");
      lcd.setCursor(0,1);
      lcd.print("Not enough cage ");
      // Waiting key #
      action = keysRead(false);
      if (action == '#') {
        indice = 0x05;
      }
    break;

    // Save Data & send to All ESP
    case 0xFF:
      lcd.clear();
      // Starting page 
      lcd.setCursor(0,0);
      lcd.print("STARTING ....   ");
      lcd.setCursor(0,1);
      lcd.print("Sending data ...");
    break;
  }

}

/// SEND DATA TO SD CARD ///
void saveData(String nEtude, String groupe, String espname ,String data){
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
