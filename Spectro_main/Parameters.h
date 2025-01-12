#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>

// * * * * * * * * * * * * * * //
// * * * Contrôle du CCD * * * //
// * * * * * * * * * * * * * * //

// Pin de contrôle et de données
#define OS_PIN A0   // Pin de données
#define SH_PIN D0   // Shift gate
#define ICG_PIN D1  // Integration clear gate
#define CLK_PIN D2  // Clock

// Paramètres
#define INTEGRATION_TIME  // Temps d'intégration
#define CLK_FREQ 1e6      // Fréquence de la clock (Hz)


// * * * * * * * * * * * * * * * //
// * * * Contrôle du Laser * * * //
// * * * * * * * * * * * * * * * //
#define TEC_PIN D4    // Pin d'activation du TEC du laser
#define LASER_PIN D5  // Pin d'activation du laser

// Pour la lecture de la puissance du laser
#define LASER_POWER_PIN A5  // Pin de lecture de la puissance du laser
#define VIN_POWER 3.3              // Tension d'alimentation du circuit de mesure de la température
#define R_POWER 1500               // Résistance du circuit de mesure de la température

// Pour la lecture de la température du laser
#define LASER_TEMPERATURE_PIN A6  // Pin de lecture de la température du laser
#define VIN_TEMP 3.3              // Tension d'alimentation du circuit de mesure de la température
#define R_TEMP 8700               // Résistance du circuit de mesure de la température


// * * * * * * * * * * * * * * * //
// * * * Autres paramètres * * * //
// * * * * * * * * * * * * * * * //
#define FAN_PIN D3  // Pin du ventilateur

#endif