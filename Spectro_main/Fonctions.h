#include <stdint.h>
#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <Arduino.h>

/* * * Pour les lectures analogiques rapides * * */
void ADCSetup();  // Activation de la pin Amn en mode analogique
void ADCSelect(int Pn, int An, bool activation=true);  // Sélection de la pin lors des lectures analogues
void ADCStart();  // Débuter une lecture analogique (1.5MHz)
uint16_t ADCRead(int m);  // Obtenir la valeur de la lecture analogique - Utiliser ANmn

int get_ANn(int pin);  // Obtenir la valeur n dans ANmn de la pin
int get_Pn(int pin);  // Obtenir la valeur n dans Pmn de la pin
int get_Pm(int pin);  // Obtenir la valeur m dans Pmn de la pin


/* * * Pour les écritures digitales rapides * * */
void digitalWriteFast(byte m, byte n, bool state);  // Écriture rapide sur une pin digitale (38.0 ns = 26.3 MHz) - Utiliser Pmn


#endif