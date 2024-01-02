#include "Parameters.h"
#include "Fonctions.h"
#include "Laser.h"
#include "TCD1304_GP.h"


TCD1304_GP TCD(CLK_PIN, OS_PIN, SH_PIN, ICG_PIN);


void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("\nDebut");

  TCD.set_integration_time(200000);

  // ADCSetup();  // Initialisation du convertisseur ADC0
  // ADCSelect(14, 12, true);  // Sélection de la pin P014 = A012 = A6
  // ADCStart(false);
  // //delayMicroseconds(1);

  // uint16_t val;
  // val = ADCRead(12);
  // // pinMode(A3, INPUT);
  // Serial.println(val);

  // Serial.println();
  // Serial.println(get_ANn(A0));
  // Serial.println(get_Pn(A5));
  // Serial.println(get_Pm(D1));
}

void loop() {
  if (Serial.available() > 0){
    int tps = Serial.parseInt();
    TCD.set_integration_time(tps);
    Serial.print("Temps d'intégration: ");
    Serial.println(tps);
    delay(1000);
    while (Serial.available() > 0){Serial.read();}
  }

  TCD.capture_data();
  TCD.shift_data();

  for (int i=0; i<TCD.get_n_pixel(); i++){
    Serial.print(i);
    Serial.print(",");
    Serial.println(TCD.get_data(i));
  }

  delay(1000);
}
