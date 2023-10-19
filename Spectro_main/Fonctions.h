#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <Arduino.h>

/* * * Pour les lectures analogiques rapides * * */
void ADCSetup(int n)();  // Activation de la pin Amn en mode analogique - Utiliser Pmn
void ADCSelect(int n, bool activation);  // Sélection de la pin lors des lectures analogues - Utiliser ANmn
void ADCStart();  // Débuter une lecture analogique (1.5MHz)
uint16_t ADCRead(int m);  // Obtenir la valeur de la lecture analogique - Utiliser ANmn


/* * * Pour les écritures digitales rapides * * */
void digitalWriteFast(byte m, byte n, bool state);  // Écriture rapide sur une pin digitale (38.0 ns = 26.3 MHz) - Utiliser Pmn


#endif