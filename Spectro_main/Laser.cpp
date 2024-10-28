#include <Arduino.h>
#include "Laser.h"
#include "Fonctions.h"

Laser::Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin) : _laser_pin(laser_pin), _tec_pin(tec_pin), _pd_pin(pd_pin), _temp_pin(temp_pin) {
  pinMode(_tec_pin, OUTPUT);
  pinMode(_pd_pin, INPUT);
  pinMode(_temp_pin, INPUT);

  PWMSetup(_laser_pin);
  PWMPinSelect(_laser_pin);
  PWMSetPeriod(_laser_pin, _laser_PWM_period);  // Fréquence de 390 625 Hz
  PWMSetDutyCycle(_laser_pin, _laser_PWM_period * _power / MAX_POWER);
}

void Laser::activate(bool state){
  PWMStart(_laser_pin, state);
  _state = state;
}

float Laser::get_power(){

}

float Laser::get_temp(){

}

bool Laser::get_state(){ return _state; }

void Laser::set_target_power(int power){ _power = power; }
void Laser::set_target_temp(float temp){ _target_temp = temp; }
