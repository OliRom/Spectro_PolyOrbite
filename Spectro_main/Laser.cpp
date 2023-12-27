#include "Arduino.h"
#include "Laser.h"

Laser::Laser(byte laser_pin, byte tec_pin, byte pd_pin, byte temp_pin) : _laser_pin(laser_pin), _tec_pin(tec_pin), _pd_pin(pd_pin), _temp_pin(temp_pin) {
  pinMode(_laser_pin, OUTPUT);
  pinMode(_tec_pin, OUTPUT);
  pinMode(_pd_pin, INPUT);
  pinMode(_temp_pin, INPUT);
}

void Laser::activate(bool state){

}

float Laser::get_power(){

}

float Laser::get_temp(char unit){

}

bool Laser::get_state(){

}

void Laser::set_target_power(){

}

void Laser::set_target_temp(char unit){

}