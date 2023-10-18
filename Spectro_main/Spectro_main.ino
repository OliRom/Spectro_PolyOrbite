#include "Parameters.h"
#include "Fonctions.h"
#include "Laser.h"
#include "TCD1304_GP.h"


void setup() {
  Serial.begin(9600);
  delay(5000);

  ADC6_setup();  // Initialisation du port A6 en analogRead rapide
}

void loop() {

}
