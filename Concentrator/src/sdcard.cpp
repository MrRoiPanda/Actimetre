#include <Arduino.h>
#include <SD.h>

/// --------------------- SD CARD ------------------------- ///

File myFile;

/// SAVE PARAMETERS DATA ///
void saveSettings(String _etude, String _techid, byte _animal, byte _anumber, int _tstart, int _tduration, byte _brightness, byte _gid){
  String path;
  path = "/" + _etude;
  if (SD.exists(path + "/settings.txt") == false) {
    SD.mkdir(path);
    Serial.println("file Created");
  }
  // Creat text file + open in read mode
  myFile = SD.open(path + "/settings.txt", FILE_WRITE);
  // Header / Column name
  myFile.print("Etude : ");
  myFile.println(_etude);
  myFile.print("Technicien : ");
  myFile.println(_techid);
  myFile.print("Animal : ");
  myFile.println(_animal);
  myFile.print("Nb d'animaux : ");
  myFile.println(_anumber);
  myFile.print("Heur de debut : ");
  myFile.println(_tstart);
  myFile.print("Durée : ");
  myFile.println(_tduration);
  myFile.print("luminosité : ");
  myFile.println(_brightness);
  myFile.print("ID du Groupe : ");
  myFile.println(_gid);
  // Close file
  myFile.close();
  Serial.println("Data saved");
  
}

/// SEND DATA TO SD CARD ///
void saveData(String _etude, String _gid, String espname ,String data){
  String path;
  path = "/" + _etude;
  if (SD.exists(path + "/data.csv") == false)
  {
    SD.mkdir(path);
    Serial.println("file Created");
  }
  
  // Creat text file + open in read mode
  myFile = SD.open(path + "/data.csv", FILE_WRITE);
  // Header / Column name
  myFile.println("Groupe ; Cage ; Donnee" );
  // Write in file
  myFile.println(_gid + ';' + espname + ';' + data);
  // Close file
  myFile.close();
  
}