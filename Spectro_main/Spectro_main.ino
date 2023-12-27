#include "Parameters.h"
#include "Fonctions.h"
#include "Laser.h"
#include "TCD1304_GP.h"


TCD1304_GP TCD(CLK_PIN, OS_PIN, SH_PIN, ICG_PIN);


void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("\nDebut");

  TCD.set_integration_time(5);

  // ADCSetup();  // Initialisation du convertisseur ADC0
  // ADCSelect(14, 12, true);  // Sélection de la pin P014 = A012 = A6
  // ADCStart();

  // uint16_t val;
  // val = ADCRead(12);
  // pinMode(A3, INPUT);
  // Serial.println(val);

  // Serial.println();
  // Serial.println(get_ANn(A0));
  // Serial.println(get_Pn(A5));
  // Serial.println(get_Pm(D1));
}

void loop() {
  //Serial.println("loop");
  TCD.capture_data();
}
