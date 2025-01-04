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


// std::map<std::string, str_cmd_struct> var_table = {
//   { "int1", { &i1, INT } },
//   { "int2", { &i2, INT } },
//   { "float1", { &f1, FLOAT } },
//   { "bool1", { &b1, BOOL } },
//   { "age", { &pers.age, INT } }
// };

// std::map<std::string, str_cmd_struct> fun_table = {
//   { "Add", { (void*)add, INT_INT_INT } },
//   { "Divi", { (void*)divide, FLOAT_FLOAT } },
//   { "Afficheur", { (void*)afficheur, _CCHAR } },
//   { "intro", { (void*)intro, _CCHAR } }
// };


#endif
