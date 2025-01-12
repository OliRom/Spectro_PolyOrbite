#include <Arduino.h>
#include "Laser.h"
#include "Fonctions.h"

Laser::Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin)
  : _laser_pin(laser_pin), _tec_pin(tec_pin), _pd_pin(pd_pin), _temp_pin(temp_pin) {
  pinMode(_tec_pin, OUTPUT);
  pinMode(_pd_pin, INPUT);
  pinMode(_temp_pin, INPUT);

  PWMSetup(_laser_pin);
  PWMPinSelect(_laser_pin);
  PWMSetPeriod(_laser_pin, _laser_PWM_period);  // Fréquence de 390 625 Hz
  PWMSetDutyCycle(_laser_pin, _laser_PWM_period * _power / MAX_POWER);
}

void Laser::activate(bool state) {
  PWMStart(_laser_pin, state);
  _state = state;
}

void Laser::allow_lasing(bool state, byte code) {
  if (state == false) {
    _allow_lasing = false;
  }

  else if ((state == true) and (code == LASING_CODE)) {
    _allow_lasing = true;
  }
}

float Laser::get_power(float V_in, float R) {
  int meas = analogRead(_temp_pin);        // Lecture de la pin de puissance
  float V_out = (float)meas * 3.3 / 1024;  // Conversion de la mesure en valeur de tension

  float I = (V_in - V_out) / R;  // Courant circulant dans la photodiode (A)
  float P = _PD_SLOPE * I;       // Puissance (W)

  return P * 1000;  // Puissance (mW)
}

float Laser::get_temp(float V_in, float R) {
  int meas = analogRead(_temp_pin);        // Lecture de la pin de température
  float V_out = (float)meas * 3.3 / 1024;  // Conversion de la mesure en valeur de tension

  float R_therm = R * (V_in / V_out - 1);
  float T_inv = 1 / _T_0 + 1 / _BETA * log(R_therm / _R_0);  // Inverse de la température (K)

  return 1 / T_inv - 273.15;  // Température (oC)
}

bool Laser::get_state() {
  return _state;
}

void Laser::set_target_power(int power) {
  _power = power;
}

void Laser::set_target_temp(float temp) {
  _target_temp = temp;
}

void Laser::set_timeout(int time) {
  _timeout = time;
}

void Laser::reset_timout() {
}
