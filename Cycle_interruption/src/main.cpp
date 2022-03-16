#include <Arduino.h>
#define enable 17

/* création d'un timer */
hw_timer_t * timerProgression = NULL;

// the number of the signal pin
const int signalPin = 16;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 200;
const int signalChannel = 0;
const int resolution = 13;

bool agrandir = true;
int dutycycle = 0;

void Progression(){
  ledcWrite(signalChannel, dutycycle);
  if (agrandir == true )
  {  
    dutycycle++;  
      if (dutycycle > 8191)
      {
        agrandir = false;
      }
  }
  else
  {
    dutycycle--;
      if(dutycycle<0)
      {
        agrandir = true;
      }
  }
}

void setup(){
  pinMode(enable,OUTPUT);
  // configure signal PWM functionalitites
  ledcSetup(signalChannel, freq, resolution); 
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(signalPin, signalChannel);

  /* Utilise le premier timer sur les quatres  */
 // timer 0 (4 timers possible) sélectioné avec une prédivision de 80 -> 80MHz / 80=1MHz soit   1 us ( division possible de 2 à 65536) et autorisation 
   timerProgression = timerBegin(0, 80, true);  // incrémentation toutes les us true sur front montant de l'horloge
                                    // true: compte sur front montant
  
  timerAttachInterrupt(timerProgression, &Progression, true);  // valide l'interruption du timer. l'interruption déclenche la fonction progression

  timerAlarmWrite(timerProgression, 2500, true);  // alarme calée pour déterminer la temporisation
                                          // true rechargement automatique
  timerAlarmEnable(timerProgression);  // alarm autorisée
digitalWrite(enable, LOW);
}
 
void loop(){
  
}