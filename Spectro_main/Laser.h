#ifndef LASER_H
#define LASER_H

#include <Arduino.h>

#define MAX_POWER 500  // Puissance maximale du laser (mW)

class Laser{
  private:
    byte _laser_pin;  // Pin d'activation du laser
    byte _tec_pin;  // Pin du TEC (système de refroidissement)
    byte _pd_pin;  // Pin de la photodiode (pour la puissance du laser)
    byte _temp_pin;  // Pin de la thermistance

    bool _state = false;  // État du laser (true: on, false: off)
    int _laser_PWM_period = 256;  // Période du PWM de la pin d'activation du laser
    int _power = 5;  // Puissance du laser

    float _target_temp = 25;

  public:
    Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin);

    void activate(bool state);  // Activer/désactiver le laser

    float get_power();  // Obtenir la puissance du laser (mW)
    float get_temp();  // Obtenir la température du laser (oC)
    bool get_state();  // Obtenir l'état du laser (true: on, false: off)

    void set_target_power(int power);  // Spécifier la puissance cible du laser (mW)
    void set_target_temp(float temp);  // Spécifier la température cible du laser (oC)
};

#endif