/*
 Fonctions pour contrôler le spectromètre.
 Ces fonctions sont celles principalement utilisées par l'utilisateur.
*/

#ifndef MAIN_FUN_H
#define MAIN_FUN_H

#include <Arduino.h>
#include <string>

#include "Parameters.h"
#include "Fonctions.h"
#include "MainFun.h"
#include "Laser.h"
#include "TCD1304_GP.h"
#include "StrCommander.h"
#include "TaskManager.h"

#define LASER_TIMER 10000
#define FAN_TIMER 5 * 60000


TCD1304_GP CCD(CLK_PIN, OS_PIN, SH_PIN, ICG_PIN);
Laser laser(LASER_PIN, TEC_PIN, LASER_POWER_PIN, LASER_TEMPERATURE_PIN);
int laser_target_temp = 30;

StrCommander cmd;     // Commandeur par chaine de caractères
String ard_str;       // String défini par Arduino
std::string command;  // Commande
std::string res;      // Réponse


// Tâches à exécuter à chaque de manière récurrente

// Lire le port série et exécuter la commande associée
void execute_command() {
  if (Serial.available() != 0) {
    ard_str = Serial.readStringUntil('\n');  // Arduino string
    ard_str.trim();                          // Trim the string (remove the \n at the end)
    command = std::string(ard_str.c_str());  // Convert to std::string

    // Serial.print("Received command: ");
    Serial.println(command.c_str());

    res = cmd.execute_command(&command);  // Execute command

    Serial.print("Results: ");
    Serial.println(res.c_str());

    Serial.println(char(4));  // End of transmission
  }
}
Task execute_command_task = { execute_command, 0 };

// Arrêter le laser
void stop_laser() {
  laser.activate(0);
}
Task stop_laser_task = { stop_laser, LASER_TIMER };

// Arrêter le ventilateur
void stop_fan() {
  // Seulement arrêter le ventilateur lorsque le laser est assez froid
  if (laser.get_temp() > laser_target_temp) {
  } else {
    digitalWrite(FAN_PIN, 0);
  }
}
Task stop_fan_task = { stop_fan, FAN_TIMER };

// Contrôle de la température à chaque seconde
void laser_thermostat() {
  float temp_error = laser.get_temp() - laser_target_temp;
  bool activate_tec = true;
  if (temp_error > 5) {
    laser.set_tec_power(100);
  } else if (temp_error >= 0) {
    laser.set_tec_power(50);
  } else if (temp_error >= -1) {
    laser.set_tec_power(20);
  } else {
    laser.set_tec_power(0);
    activate_tec = false;
  }

  if (activate_tec) {
    digitalWrite(FAN_PIN, 1);
    reset_task_timer(stop_fan_task);
  }
}
Task laser_thermostat_task = { laser_thermostat, 100 };


// Permettre l'activation du laser
bool allow_lasing(bool state, byte code) {
  return laser.allow_lasing(state, code);
}

// // Spécifier la température du laser
// void set_temp(float temp) {
//   laser.set_target_temp(temp);
// }

// Obtenir la température du laser
float get_temp() {
  return laser.get_temp();
}

// Spécifier le temps d'intégration
void set_integ_time(int time) {
  CCD.set_integration_time(time);
}

// Obtenir les données d'un spectre et les envoie sur le port série
std::string acquire_data(int n_measures) {
  std::string msg;

#ifdef DEBUG_MODE  // Pour simuler l'activation du laser
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDG, HIGH);  // Une mesure HIGH éteint la LED
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDB, HIGH);
#endif

  // Pour réduire le bruit dans les mesures
  digitalWrite(FAN_PIN, 0);
  laser.set_tec_power(0);

  // Mesure de calibration
  const pixels_format *calibration_data = CCD.get_data_pointer(CALIBRATION);  // Pointeur vers les données
  int n_tentatives = 0;
  const int shielded_limit = pow(2, 12) * 0.65;
  do {
    CCD.acquire_data(1, CALIBRATION);
    if (n_tentatives++ > 9) {
      msg += "Calibration impossible. Le temps d'intégration est probablement trop élevé.\n";
      msg += "La dernière valeur de calibration est: ";
      msg += std::to_string(calibration_data->util.shielded[N_SHIELDED_PIXELS / 2]);
      msg += " < " + std::to_string(shielded_limit) + ".";
      return msg;
    }
  } while (calibration_data->util.shielded[N_SHIELDED_PIXELS / 2] < shielded_limit);
  CCD.acquire_data(1, CALIBRATION);
  CCD.acquire_data(n_measures, CALIBRATION);

  // Prendre les mesures
#ifdef DEBUG_MODE
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDB, LOW);
#endif

  laser.activate(true);
  reset_task_timer(stop_laser_task);
  if (laser.get_state() == 0) {
    msg += "Acquisition impossible. allow_lasing() n'a probablement pas été appelé.";
    laser.activate(false);
    return msg;
  }

  CCD.acquire_data(1, DATA);
  CCD.acquire_data(n_measures, DATA);
  laser.activate(false);

#ifdef DEBUG_MODE
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDB, HIGH);
#endif

  // Traiter les données et les formatter sous forme de string
  const pixels_format *calibration = CCD.get_data_pointer(CALIBRATION);  // Calibration pointer
  const pixels_format *data = CCD.get_data_pointer(DATA);                // Data pointer
  for (int i = 0; i < N_PIXELS; i++) {
    PIXEL_DATA_TYPE pixel_value;
    if (calibration->all[i] < data->all[i]) {  // Une valeur élevée signifie un manque de lumière
      pixel_value = 0;
    } else {
      pixel_value = calibration->all[i] - data->all[i];
    }
    msg += std::to_string(pixel_value) + ",";
  }

  return msg;
}


// Fonctions de débugguage
#ifdef DEBUG_MODE
#include <Wire.h>
#include "AD5272.h"

// Activer le laser
void activate_laser(bool state) {
  laser.activate(state);
  reset_task_timer(stop_laser_task);
}

// Obtenir l'état du laser (allumé/éteint)
bool get_laser_state() {
  return laser.get_state();
}

// Ajuster la valeur du potentiomètre
AD5272 pot(ADDR_NC);  // Instance du potentiomètre
void set_pot_value(int value) {
  pot.set_res(static_cast<uint16_t>(value));
  Serial.println(pot.get_res());
  Serial.println(pot.get_ctrl());
}

// Spécifier la puissance du laser
void set_power(int power) {
  laser.set_target_power(power);
}

// Lire la puissance du laser
float get_power() {
  return laser.get_power();
}

// Spécifier le temps après lequel le laser s'éteint
void set_laser_timer(int time) {
  laser.set_timeout(time);
}

// Réinitialiser le timer du laser
void reset_laser_timer() {
  laser.reset_timout();
}

// Activer le ventilateur
void activate_fan(bool state) {
  digitalWrite(FAN_PIN, state ? HIGH : LOW);
}

// Ajuster la puissance du TEC
void set_tec_power(int value) {
  laser.set_tec_power(value);
}

// Debug setup
void debug_setup() {
  Wire.begin();
  pot.init();
}

#endif


std::map<std::string, str_cmd_struct> var_table = {
  { "laser_target_temp", { &laser_target_temp, INT } }
};

std::map<std::string, str_cmd_struct> fun_table = {
  { "allow_lasing", { (void *)allow_lasing, BOOL_BOOL_BYTE } },
  // { "set_temp", { (void *)set_temp, _FLOAT } },
  { "get_temp", { (void *)get_temp, FLOAT_ } },
  { "set_integ_time", { (void *)set_integ_time, _INT } },
  { "acquire_data", { (void *)acquire_data, STR_INT } },
// { "get_data", { (void *)get_data, STR_ } },

#ifdef DEBUG_MODE
  { "activate_laser", { (void *)activate_laser, _BOOL } },
  { "get_laser_state", { (void *)get_laser_state, BOOL_ } },
  { "set_laser_timer", { (void *)set_laser_timer, _INT } },
  { "reset_laser_timer", { (void *)reset_laser_timer, VOID } },
  { "set_power", { (void *)set_power, _INT } },
  { "get_power", { (void *)get_power, FLOAT_ } },
  { "set_pot_value", { (void *)set_pot_value, _INT } },
  { "activate_fan", { (void *)activate_fan, _BOOL } },
  { "set_tec_power", { (void *)set_tec_power, _INT } },
#endif

};


#endif
