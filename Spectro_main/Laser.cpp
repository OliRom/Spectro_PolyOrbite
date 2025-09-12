#include <new>
#include <Arduino.h>
#include "Laser.h"
#include "Fonctions.h"

Laser::Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin)
  : _laser_pin(laser_pin), _tec_pin(tec_pin), _pd_pin(pd_pin), _temp_pin(temp_pin) {
  pinMode(_tec_pin, OUTPUT);
  digitalWrite(_tec_pin, LOW);

  pinMode(_pd_pin, INPUT);
  pinMode(_temp_pin, INPUT);

  // PWMSetup(_tec_pin);
  // PWMPinSelect(_tec_pin);
  // PWMSetPeriod(_tec_pin, 100'000'000);  // Fréquence de 1Hz
  // PWMSetDutyCycle(_tec_pin, 0);
  // PWMStart(_tec_pin, true);

  pinMode(_laser_pin, OUTPUT);
  digitalWrite(_laser_pin, LOW);
}

// void Laser::_update_10s() {
//   float temp_error = _target_temp - get_temp();
// }

void Laser::init() {
  AD5272 _pot(ADDR_NC);  // Instance pour le potentiomètre
  _pot.init();           // Initialiser le potentiomètre
}

void Laser::activate(bool state) {
  if ((!state) or _allow_lasing) {
    digitalWrite(_laser_pin, state ? HIGH : LOW);
    _state = state;
  }
}

bool Laser::allow_lasing(bool state, byte code) {
  if ((state == true) and (code == LASING_CODE)) {
    _allow_lasing = true;
  } else {
    _allow_lasing = false;
    activate(false);  // Éteindre le laser
  }

  return _allow_lasing;
}

float Laser::get_power() {
  int meas = analogRead(_pd_pin);          // Lecture de la pin de puissance
  float V_out = (float)meas * 3.3 / 1024;  // Conversion de la mesure en valeur de tension

  float I = (VIN_POWER - V_out) / R_POWER;  // Courant circulant dans la photodiode (A)
  float P = _PD_SLOPE * I;                  // Puissance (W)

  return P * 1000;  // Puissance (mW)
}

float Laser::get_temp() {
  int meas = analogRead(_temp_pin);        // Lecture de la pin de température
  float V_out = (float)meas * 3.3 / 1024;  // Conversion de la mesure en valeur de tension

  float R_therm = R_TEMP * (VIN_TEMP / V_out - 1);
  float T_inv = 1.0 / _T_0 + 1.0 / _BETA * log(R_therm / _R_0);  // Inverse de la température (K)

  return 1.0 / T_inv - 273.15;  // Température (oC)
}

bool Laser::get_state() {
  return _state;
}

void Laser::set_tec_power(int power) {
  analogWrite(_tec_pin, static_cast<int>(2.55 * constrain(power, 0, 100)));
}

void Laser::set_target_power(int power) {
  _power = power;
}

// void Laser::set_target_temp(float temp) {
//   _target_temp = temp;
// }

void Laser::set_timeout(int time) {
  _timeout = time;
}

void Laser::reset_timout() {
}
