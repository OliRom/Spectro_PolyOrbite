#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>

// * * * * * * * * * * * * * * //
// * * * Contrôle du CCD * * * //
// * * * * * * * * * * * * * * //

// Pin de contrôle et de données
#define OS_PIN A6  // Pin de données
#define SH_PIN D0  // Shift gate
#define ICG_PIN D1  // Integration clear gate
#define CLK_PIN D2  // Clock

// Paramètres
#define INTEGRATION_TIME  // Temps d'intégration
#define CLK_FREQ 1e6  // Fréquence de la clock (Hz)


// * * * * * * * * * * * * * * * //
// * * * Contrôle du Laser * * * //
// * * * * * * * * * * * * * * * //


#endif