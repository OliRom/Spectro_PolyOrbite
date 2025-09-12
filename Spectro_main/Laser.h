/*
 Classe pour contrôler le laser.
*/

#ifndef LASER_H
#define LASER_H

#include <Arduino.h>

#include "AD5272.h"

#define MAX_POWER 500            // Puissance maximale du laser (mW)
#define LASING_CODE 0b0100'0000  // Code qui doit être fourni pour permettre au laser de s'allumer (caractère ascii '@') = 0x40 = 64

// Paramètres de la thermistance
#define _T_0 298.15  // Tepérature à 25oC (K)
#define _R_0 10000   // Résistance à 25oC (Ohm)
#define _BETA 3450   // Paramètre de sensibilité de la thermistance (s.u.)

#define VIN_TEMP 3.3  // Tension d'alimentation du circuit de mesure de la température
#define R_TEMP 8700   // Résistance du circuit de mesure de la température

// Paramètre de la photodiode
#define _PD_SLOPE 250  // P = m * I; m = 250 mW / mA = 250 W / A

#define VIN_POWER 3.3  // Tension d'alimentation du circuit de mesure de la puissance
#define R_POWER 1500   // Résistance du circuit de mesure de la puissance

class Laser {
private:
  byte _laser_pin;  // Pin d'activation du laser
  byte _tec_pin;    // Pin du TEC (système de refroidissement)
  byte _pd_pin;     // Pin de la photodiode (pour la puissance du laser)
  byte _temp_pin;   // Pin de la thermistance

  bool _state = false;          // État du laser (true: on, false: off)
  bool _allow_lasing = false;   // Permettre au laser de s'allumer
  int _laser_PWM_period = 256;  // Période du PWM de la pin d'activation du laser
  int _power = 5;               // Puissance du laser
  int _timeout = 0;             // Temps après lequel le laser s'éteint (ms)

  // float _target_temp = 25;  // Température cible pour le laser (oC)

  // void _update_10s();  // Cette fonction est appelée toutes les 10 secondes pour mettre à jour les paramètres du laser

public:
  Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin);

  void init();  // À appeler une fois que Wire.begin() a été appelé (pour le potentiomètre)

  void activate(bool state);                       // Activer/désactiver le laser
  bool allow_lasing(bool state, byte code = 0b0);  // Permettre l'activation du laser; retourne True si le laser peut laser

  float get_power();  // Obtenir la puissance du laser (mW)
  float get_temp();   // Obtenir la température du laser (oC); V_in: tension d'alimentation (V), R: résistance de mesure (Ohm)
  bool get_state();   // Obtenir l'état du laser (true: on, false: off)

  void set_tec_power(int power);  // Spécifier la puissance du TEC (%)

  void set_target_power(int power);  // Spécifier la puissance cible du laser (mW)
  // void set_target_temp(float temp);  // Spécifier la température cible du laser (oC)
  void set_timeout(int time);        // Spécifier le temps après lequel le laser s'éteint (ms)
  void reset_timout();               // Réinitialiser le timeout après lequel le laser s'éteint automatiquement
};

#endif