#ifndef LASER_H
#define LASER_H

#include <Arduino.h>

#define MAX_POWER 500            // Puissance maximale du laser (mW)
#define LASING_CODE 0b1010'1010  // Code qui doit être fourni pour permettre au laser de s'allumer

// Paramètres de la thermistance
#define _T_0 298.15  // Tepérature à 25oC (K)
#define _R_0 10000   // Résistance à 25oC (Ohm)
#define _BETA 3450   // Paramètre de sensibilité de la thermistance (s.u.)

// Paramètre de la photodiode
#define _PD_SLOPE 250  // P = m * I; m = 250 mW / mA = 250 W / A

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

  float _target_temp = 25;  // Température cible pour le laser (oC)

public:
  Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin);

  void activate(bool state);                       // Activer/désactiver le laser
  void allow_lasing(bool state, byte code = 0b0);  // Permettre l'activation du laser

  float get_power(float V_in, float R);  // Obtenir la puissance du laser (mW)
  float get_temp(float V_in, float R);   // Obtenir la température du laser (oC); V_in: tension d'alimentation (V), R: résistance de mesure (Ohm)
  bool get_state();                      // Obtenir l'état du laser (true: on, false: off)

  void set_target_power(int power);  // Spécifier la puissance cible du laser (mW)
  void set_target_temp(float temp);  // Spécifier la température cible du laser (oC)
  void set_timeout(int time);        // Spécifier le temps après lequel le laser s'éteint (ms)
  void reset_timout();               // Réinitialiser le timeout après lequel le laser s'éteint automatiquement
};

#endif