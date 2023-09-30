#ifndef LASER_H
#define LASER_H

#include <Arduino.h>

class Laser{
  private:
    byte _laser_pin;  // Pin d'activation du laser
    byte _tec_pin;  // Pin du TEC (système de refroidissement)
    byte _pd_pin;  // Pin de la photodiode (pour la puissance du laser)
    byte _temp_pin;  // Pin de la thermistance

    bool _state = false;  // État du laser (true: on, false: off)

  public:
    Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin);

    void activate(bool state);  // Activer/désactiver le laser

    float get_power();  // Obtenir la puissance du laser (mW)
    float get_temp(char unit='k');  // Obtenir la température du laser ('k': Kelvin, 'c': Celsius)
    bool get_state();  // Obtenir l'état du laser (true: on, false: off)

    void set_target_power();  // Spécifier la puissance cible du laser (mW)
    void set_target_temp(char unit='k');  // Spécifier la température cible du laser ('k': Kelvin, 'c': Celsius)
};

#endif