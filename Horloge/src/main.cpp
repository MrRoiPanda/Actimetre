#include <Arduino.h>
#include "PCF8583.h"
#define enable 17
/* création d'un timer */
hw_timer_t * timerProgression = NULL;
PCF8583 rtc(0xA0);

void assertEquals(const char * test, byte expected, byte real) {
    if (expected == real) {
        Serial.print("OK: ");
         Serial.println(real);

    } else {
        Serial.print("KO: ");
        Serial.println(real);
        Serial.println(expected);       
    }
}

void Calcul_heure(){ 
    int64_t rng_s = random(0,59)  * 1000;
    int64_t rng_m = random(0,58) * 60000;
    Serial.println(rng_s);
    Serial.println(rng_m);
    int64_t sum = rng_s + rng_m;
    Serial.println(sum);
    delay( sum );
    assertEquals("getSeconds +" , rng_s / 1000 , rtc.getSecond());
    assertEquals("getMinutes +", rng_m / 60000  , rtc.getMinute());
}


void setup() {
    
    Serial.begin(9600);
    pinMode(enable,OUTPUT);

    rtc.setTime(0, 0, 0);
   /*assertEquals("getSeconds", 0, rtc.getSecond());
    assertEquals("getMinutes", 0, rtc.getMinute());*/

    // timer 0 (4 timers possible) sélectioné avec une prédivision de 80 -> 80MHz / 80=1MHz soit 1 us ( division possible de 2 à 65536) et autorisation 
    timerProgression = timerBegin(0, 80, true);   // incrémentation toutes les us true sur front montant de l'horloge
                                                        // true: compte sur front montant
  
    timerAttachInterrupt(timerProgression, &Calcul_heure, true);  // valide l'interruption du timer. l'interruption déclenche la fonction progression

    timerAlarmWrite(timerProgression, 2500, true);  // alarme calée pour déterminer la temporisation
                                          // true rechargement automatique
    timerAlarmEnable(timerProgression);  // alarm autorisée
    digitalWrite(enable, LOW);
}

void loop() {}