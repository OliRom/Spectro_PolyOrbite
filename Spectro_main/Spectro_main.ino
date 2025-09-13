#include <Wire.h>

#include "Parameters.h"
#include "Fonctions.h"
#include "MainFun.h"
#include "Laser.h"
#include "TCD1304_GP.h"
#include "StrCommander.h"
#include "TaskManager.h"


String ard_str;
std::string command;
std::string res;

void setup() {
  // Wire.begin();  // Pour la communication avec le potentiomètre
  Serial.begin(9600);

  pinMode(FAN_PIN, OUTPUT);
  activate_fan(false);

  CCD.set_integration_time(DEFAULT_INTEGRATION_TIME);

  cmd.set_var_table(&var_table);
  cmd.set_fun_table(&fun_table);

  laser.init();

  while (!Serial) {}
  Serial.println("Ready!");

#ifdef DEBUG_MODE
  debug_setup();
#endif

}

void loop() {
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

  run_task(stop_laser_task);
  run_task(laser_thermostat_task);
  run_task(stop_fan_task);
}
