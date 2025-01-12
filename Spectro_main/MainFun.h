#ifndef MAIN_FUN_H
#define MAIN_FUN_H

#include <Arduino.h>

#include "Parameters.h"
#include "Fonctions.h"
#include "MainFun.h"
#include "Laser.h"
#include "TCD1304_GP.h"
#include "StrCommander.h"


TCD1304_GP CCD(CLK_PIN, OS_PIN, SH_PIN, ICG_PIN);
Laser laser(LASER_PIN, TEC_PIN, LASER_POWER_PIN, LASER_TEMPERATURE_PIN);
StrCommander cmd;

void activate_laser(bool state) {
  laser.activate(state);
}

bool get_laser_state() {
  return laser.get_state();
}

void allow_lasing(bool state, byte code) {
  laser.allow_lasing(state, code);
}

void set_laser_timer(int time) {
  laser.set_timeout(time);
}

void reset_laser_timer() {
  laser.reset_timout();
}

void set_temp(float temp) {
  laser.set_target_temp(temp);
}

float get_temp() {
  return laser.get_temp(VIN_TEMP, R_TEMP);
}

void set_power(int power) {
  laser.set_target_power(power);
}

float get_power() {
  return laser.get_power(VIN_POWER, R_POWER);
}

void set_integ_time(int time) {
  CCD.set_integration_time(time);
}

void acquire_data() {
  laser.activate(true);
  CCD.acquire_data();
  laser.activate(false);
}

void get_data() {
}


// std::map<std::string, str_cmd_struct> var_table = {
//   { "int1", { &i1, INT } },
//   { "int2", { &i2, INT } },
//   { "float1", { &f1, FLOAT } },
//   { "bool1", { &b1, BOOL } },
//   { "age", { &pers.age, INT } }
// };
std::map<std::string, str_cmd_struct> var_table = {};

std::map<std::string, str_cmd_struct> fun_table = {
  { "activate_laser", { (void*)activate_laser, _BOOL } },
  { "get_laser_state", { (void*)get_laser_state, BOOL_ } },
  { "allow_lasing", { (void*)allow_lasing, _BOOL_BYTE } },
  { "set_laser_timer", { (void*)set_laser_timer, _INT } },
  { "reset_laser_timer", { (void*)reset_laser_timer, VOID } },
  { "set_temp", { (void*)set_temp, _FLOAT } },
  { "get_temp", { (void*)get_temp, FLOAT_ } },
  { "set_power", { (void*)set_power, _INT } },
  { "get_power", { (void*)get_power, FLOAT_ } },
  { "set_integ_time", { (void*)set_integ_time, _INT } },
  { "acquire_data", { (void*)acquire_data, VOID } },
};


#endif
