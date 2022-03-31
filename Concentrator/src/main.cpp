#include <Arduino.h>
#include "rgb_lcd.h"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>

/// --------------------- SD CARD ------------------------- ///

//File myFile;

/// --------------------- SCREEN -------------------------- ///

rgb_lcd lcd;

/// --------------------- KEYPAD -------------------------- ///
const byte ROWS = 4;    // 4 lignes
const byte COLUMNS = 4; // 4 colonnes
const byte DEPTH = 3;

// Colone , ligne
const char hexKeypad[COLUMNS][ROWS] = {
	{'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Profondeur, ligne, colone
const char letters[DEPTH][ROWS][COLUMNS] = {
  {
    { 'A', 'D', 'G' , '.'}, 
    { 'J', 'M', 'P' , '.'},
    { 'S', 'V', 'Y' , '.'},
    { '*', '.', '#' , 'D'}
  },
  {
    { 'B', 'E', 'H' , '.'},
    { 'K', 'N', 'Q' , '.'},
    { 'T', 'W', 'Z' , '.'},
    { '*', '.', '#' , 'D'}
  },
  {
    { 'C', 'F', 'I' , '.'},
    { 'L', 'O', 'R' , '.'},
    { 'U', 'X', '.' , '.'},
    { '*', '.', '#' , 'D'}
  }
};

const int16_t pinRows[ROWS] = {35, 34, 39, 36};        // IN
const int16_t pinColumn[COLUMNS] = {17, 16, 33, 32};   // OUT

/// ----------------------------------------------------- ///

// last key
char lkey = '.'; 
// key counter
byte ckey = 0;
//
char value = '.';
char key = ',';
char letter = '?';

// Characters displayed on screen
char Data[16];
// Characters count
byte keysCount = 0;
// Screen number
int indices = 0x00;
// key return
char action;
// Key update speed | laste value 15
const int temp = 15;

// Number of cage
byte maxCage = 32;
byte emptyCage = 32;

// Data need to be saved
String ETUDE;
String TECHID;
byte ANIMALS;
byte ANUMBRE;
int TSTART;
int TESTDURATION;
byte BRIGHTNESS;
byte GID;

/// --------------------- FUNCTION ------------------------ ///

char keysRead(bool);
char keysReadLetters();
void keysActions(char);
void clearData(void);
void testCreation(void);
void saveData(String, String, String, String);
void saveSettings(String, String, byte, byte, int, int, byte, byte);

/// ----------------------- SETUP ------------------------- /// 

void setup() {
  
  Serial.begin(115200);
  /// init screen ///
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Actimetre       ");
  lcd.setCursor(0,1);
  lcd.print("Initialisation  ");

  /// ------------------- INIT KEYPAD --------------------- ///

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

  /// --------------- INIT SD CARD PIN ------------------ ///

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("Initializing SD card...");
  
  if (!SD.begin(5)) {
    Serial.println("initialization failed!");
    //while (1);
  }
  
  Serial.println("initialization done.");

  /// -------------------------------------------------- ///

  delay(5000);
  lcd.clear();
}

void loop() {

  testCreation();
  if (indices == 0xFF) {
    while (1) {
    /* code */
    }
  }
  
}

/// -------------- DISPLAY TEXT FUNCTION --------------- /// 

// switch for each "page"
void testCreation() {

  switch (indices) {

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
        indices++;
      } else if (action == 'B'){
        indices = 0x10;
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
        indices--;
      } else if (action == '*') {
        indices++;
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
        indices--;
      } else if (action == '*') {
        //String STUDY = String(Data);
        ETUDE = String(Data);
        indices++;
        clearData();
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
      action = keysReadLetters();
      if (action == '#') {
        indices--;
      } else if (action == '*'){
        TECHID = String(Data);
        indices = 0x04;
        clearData();
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
        indices = 0x03;
      } else if(action == 'A') {
        ANIMALS = 1;
        indices++;
        clearData();
      } else if(action == 'B') {
        ANIMALS = 0;
        indices++;
        clearData();
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
        indices--;
      } else if (action == '*'){
        
        ANUMBRE = (byte) (Data[0] - '0') * 10 + (Data[1] - '0');
        Serial.println(ANUMBRE);
        if ((ANUMBRE > maxCage) || (ANUMBRE > emptyCage)) {
          indices = 0xEE;
          ANUMBRE = 0;
        } else {
          indices++;
          clearData();
          ANUMBRE = 0;
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
        indices--;
      } else if (action == '*') {

        //TSTART = Data; // int
        indices++;
        clearData();
      }

    break;

    case 0x07:
      lcd.clear();
      /// duration ///
      lcd.setCursor(0,0);
      lcd.print("duration (min)");
      lcd.setCursor(0,1);
      keysCount = 0;
      // Waiting key # or *
      action = keysRead(true);
      if (action == '#') {
        indices--;
      } else if (action == '*'){
        // TESTDURATION = String(Data); // int
        indices++;
        clearData();
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
        indices--;
      } else if (action == '*'){
        // BRIGHTNESS = Data; // byte
        indices++;
        clearData();
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
        indices--;
      } else if (action == '*'){
        // GID = Data; // byte
        indices++;
        clearData();
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
        indices--;
      } else if (action == '*'){
        
        
        indices = 0xF0;
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
        indices = 0x00;
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
        indices = 0x05;
        clearData();
      }
    break;

    // Save Data & send to All ESP
    case 0xF0:
      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print(" SAVING...      ");
      Serial.println("Saving settings");
      
      saveSettings(ETUDE, TECHID, ANIMALS, ANUMBRE, TSTART, TESTDURATION, BRIGHTNESS, GID);
      indices = 0xF1;
    break;

    // Display Test info
    case 0xF1:
      // waiting data
      lcd.clear();
      // Starting page 
      lcd.setCursor(0,0);
      lcd.print("|Test:   0%    |");
      lcd.setCursor(0,1);
      lcd.print("|               ");
    break;
  }

}

/// --------------- KEYPADE READER FUNCTION --------------- ///

// Return char, use true if you need to display key
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


/// --------------- KEYPADE READER FUNCTION FOR LETTERS --------------- ///

// Return char
char keysReadLetters(){
  char key = 0x00;
  bool inputKeyDown = false;
  Serial.println("Start read");
  Serial.println("-------------------");
  
  while ((key != '#') && (key != '*') && (key != 'A') & (key != 'B') && (key != 'C')) {

    for (byte i = 0; i < COLUMNS; i++) {
      digitalWrite(pinColumn[i], HIGH);
      delay(temp);

      for (byte j = 0; j < ROWS; j++) {
        inputKeyDown = digitalRead(pinRows[j]);

        if (inputKeyDown == true) {
          Serial.print("Touche : ");
          Serial.println(hexKeypad[j][i]);

          Serial.print("last key : ");
          Serial.println(lkey);

          key = hexKeypad[j][i];

          if (key != lkey) {
            ckey = 0;
            Serial.print("lettre : ");
            Serial.println(letters[ckey][j][i]);
            
            letter = letters[ckey][j][i];
            //keysActions(letter);
          }

          if (key == lkey) {
            ckey++;

            if (ckey >= 3) {
              ckey = 0;
              //keysActions(letter);
            }
            else {
              //key = letters[ckey][j][i];
              Serial.println("meme touche !");
              Serial.print("lettre : ");
              Serial.println(letters[ckey][j][i]);
              letter = letters[ckey][j][i];
              
            }
            
          }

          //lcd.setCursor(0, 1);
          //lcd.print(letter);

          Serial.print("ckey : ");
          Serial.println(ckey);
          Serial.print("i : ");
          Serial.println(i);
          Serial.print("j : ");
          Serial.println(j);
          Serial.println("-------------------");
          lkey = key;
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


/// --------------- DISPLAY KEY FUNCTION --------------- ///

// need char to work
void keysActions(char _key){

  switch (_key)
  {
    case 'D':

      if (keysCount != 0) {
        Data[keysCount--] = 0; //clear last key
        lcd.setCursor(keysCount,1);
        lcd.print(' ');
      }

    break;

    case '*':
      //do nothing
    break;
    case '#':
      //do nothing
    break;
    case 'A':
      //do nothing
    break;
    case 'B':
      //do nothing
    break;
    case 'C':
      //do nothing
    break;
    
    default:
      /* display _key */
      Data[keysCount] = _key;
      lcd.setCursor(keysCount,1);
      lcd.print(Data[keysCount]);
      keysCount++;
    break;
  }

}

// Clear data array
void clearData(){
  for (size_t d = 0; d < 16; d++)
  {
    Data[d] = 0;
  }
}
