#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>


// * * * * * * * * * * * * * * //
// * * * Options générales * * //
// * * * * * * * * * * * * * * //
// #define DEBUG_MODE  // Décommenter cette ligne pour être en mode débugguage


// * * * * * * * * * * * * * * //
// * * * Contrôle du CCD * * * //
// * * * * * * * * * * * * * * //

// Pin de contrôle et de données
#define OS_PIN A0   // Pin de données
#define SH_PIN D0   // Shift gate
#define ICG_PIN D1  // Integration clear gate
#define CLK_PIN D2  // Clock

// Paramètres
#define DEFAULT_INTEGRATION_TIME 500  // Temps d'intégration par défaut
#define CLK_FREQ 1e6                  // Fréquence de la clock (Hz)


// * * * * * * * * * * * * * * * //
// * * * Contrôle du Laser * * * //
// * * * * * * * * * * * * * * * //
#define TEC_PIN D4                // Pin d'activation du TEC du laser
#define LASER_PIN D5              // Pin d'activation du laser
#define LASER_POWER_PIN A5        // Pin de lecture de la puissance du laser
#define LASER_TEMPERATURE_PIN A6  // Pin de lecture de la température du laser


// * * * * * * * * * * * * * * * //
// * * * Autres paramètres * * * //
// * * * * * * * * * * * * * * * //
#define FAN_PIN D3  // Pin du ventilateur

#endif