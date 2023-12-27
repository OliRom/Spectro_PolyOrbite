#include "Parameters.h"
#include "Fonctions.h"
#include "Laser.h"
#include "TCD1304_GP.h"


void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("\nDebut");

  ADCSetup();  // Initialisation du convertisseur ADC0
  ADCSelect(14, 12, true);  // Sélection de la pin P014 = A012 = A6
  ADCStart();

  uint16_t val;
  val = ADCRead(12);
  pinMode(A3, INPUT);
  Serial.println(val);

  Serial.println();
  Serial.println(get_ANn(A0));
  Serial.println(get_Pn(A5));
  Serial.println(get_Pm(D1));
}

void loop() {

}
