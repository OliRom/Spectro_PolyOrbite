#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <Arduino.h>


void ADC6_setup();  // Activation de la pin A6 en mode lecture raide
uint16_t ADC6_analogRead();  // Lecture analogue à 1.5MHz sur la pin A6
void digitalWriteFast();  // Écriture rapide sur une pin digitale (38.0 ns = 26.3 MHz)


#endif