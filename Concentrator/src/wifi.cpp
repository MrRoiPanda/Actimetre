

// SEND DATA TO OTHERS ESP32 ///


#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress1[] = {0x08, 0x3A, 0xF2, 0xAA, 0xF0, 0x14}; //définit l'adresse mac dans broadcastAddress1
uint8_t broadcastAddress2[] = {0x08, 0x3A, 0xF2, 0xB9, 0x35, 0xA4}; //définit l'adresse mac dans broadcastAddress2


// Création d'une structure contenant nos variables
// Doit corréler avec les autres programmes
typedef struct struct_message {
  int id; //un id pour chaque board
  int statut_sujet; //variable statut sujet
  int y;  //variable non définie
  bool type_animal; // variable définissant le type d'animal   0 -> souris    1 -> rat
  byte nombre_animaux;  //nombre d'animaux lors du test
  int heure_debut;  //heure de début du test
  int duree_test; //durée du test
  byte brightness;  //luminosité dans la cage

}struct_message;


struct_message cage1; //structure séparée pour les différentes cages -> permet de définir des valeurs différentes à des variables semblables
struct_message cage2; //structure séparée pour les différentes cages -> permet de définir des valeurs différentes à des variables semblables

struct_message myData;  //structure qui récupère et envoie les données

struct_message board1;  //Créer une structure pour contenir les lectures de chaque tableau (carte 1)
struct_message board2;  //Créer une structure pour contenir les lectures de chaque tableau (carte 2)

struct_message boardsStruct[2] = {board1, board2};  //Créer un tableau avec toutes les structures

// rappel lorsque les données sont envoyées
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.println(); //saut à la ligne
  Serial.print("Packet to: ");
  // Copie l'adresse mac de l'expéditeur dans une chaîne
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");  //message de statut de l'envoi
}



// fonction de rappel qui sera exécutée lors de la réception des données
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18] ;
  Serial.println(); //saut à la ligne
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr); //on écrit l'adresse mac de l'émetteur
  memcpy(&myData, incomingData, sizeof(myData));  //on récupère les données reçues
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len); //on écrit l'id de la board et le nombre de données reçues (bytes)
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].statut_sujet = myData.statut_sujet; //on récupère la valeur de la variable
  boardsStruct[myData.id-1].y = myData.y; //on récupère la valeur de la variable
  Serial.printf("statut du sujet vivant/pas vivant: %d \n", boardsStruct[myData.id-1].statut_sujet);  //on affiche la valeur
  Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y); //on affiche la valeur
  Serial.println(); //saut à la ligne
}
 
void setup() {
  //Initialiser le moniteur série
  Serial.begin(115200);
  

  //Définir l'appareil en tant que station Wi-Fi
  WiFi.mode(WIFI_STA);

  //Initialise l'ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Une fois ESPNow initialisé avec succès, nous nous enregistrons pour recevoir et obtenir des informations sur recv packer
  esp_now_register_recv_cb(OnDataRecv);

  // Une fois ESPNow initialisé avec succès, nous nous enregistrons pour recevoir et obtenir des informations sur send packer
  esp_now_register_send_cb(OnDataSent);

  //fonction d'appairage
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  //appairage d'un premier ESP
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6); //on l'appaire en fonction de l'adresse mac définit au début du programme
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer"); //message de statut de l'appairage
    return;
  }
  
  
  //appairage d'un second ESP
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6); //on l'appaire en fonction de l'adresse mac définit au début du programme
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer"); //message de statut de l'appairage
    return;
  }
}

void loop() {

cage1.type_animal = 0;  //type d'animal   0 -> souris    1 -> rat
cage1.nombre_animaux = 2; //nombre d'animaux
cage1.heure_debut = 12; //heure de début du test
cage1.duree_test = 1230;  //durée du test
cage1.brightness = 50;  //luminosité dans la cage


cage2.heure_debut = 16; //heure de début du test
cage2.duree_test = 6500;  //durée du test
cage2.type_animal = 1;  //type d'animal   0 -> souris    1 -> rat
cage2.nombre_animaux = 6; //nombre d'animaux
cage2.brightness = 50;  //luminosité dans la cage


esp_err_t result1 = esp_now_send(
  broadcastAddress1,  //choix de l'adresse mac à laquelle seront envoyées les données
  (uint8_t *) &cage1, //les données de la structure cage1 sont envoyées
  sizeof(struct_message));
   
if (result1 == ESP_OK) {  //message de validation de l'envoi
  Serial.println("Sent with success");
}
else {
  Serial.println("Error sending the data");
}


esp_err_t result2 = esp_now_send(
  broadcastAddress2,  //choix de l'adresse mac à laquelle seront envoyées les données
  (uint8_t *) &cage2, //les données de la structure cage1 sont envoyées
  sizeof(struct_message));

if (result2 == ESP_OK) {   //message de validation de l'envoi
  Serial.println("Sent with success");
}
else {
  Serial.println("Error sending the data");
}


  delay(5000);  //delai de 5000
}
