#include <Wire.h>

#include "Parameters.h"
#include "Fonctions.h"
#include "MainFun.h"
#include "Laser.h"
#include "TCD1304_GP.h"
#include "StrCommander.h"
#include "TaskManager.h"


void setup() {
  // Wire.begin();  // Pour la communication avec le potentiomètre
  Serial.begin(9600);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  CCD.set_integration_time(DEFAULT_INTEGRATION_TIME);

  cmd.set_var_table(&var_table);  // Pour exécuter les commandes reçues par port série
  cmd.set_fun_table(&fun_table);

  laser.init();

  while (!Serial) {}  // Attendre que le port série soit prêt
  Serial.println("Ready!");

#ifdef DEBUG_MODE
  debug_setup();
#endif
}

void loop() {
  run_task(execute_command_task);   // Exécute les commandes reçues par port série
  run_task(stop_laser_task);        // Arrête le laser après 10s (par sécurité)
  run_task(laser_thermostat_task);  // Régule la température du laser
  run_task(stop_fan_task);          // Arrête la fan lorsque non nécessaire
}
