#include "Parameters.h"
#include "Fonctions.h"
#include "MainFun.h"
#include "Laser.h"
#include "TCD1304_GP.h"
#include "StrCommander.h"


void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("\nDebut\n");

  CCD.set_integration_time(200000);
}

// void loop() {
//   if (Serial.available() != 0) {
//     ard_str = Serial.readStringUntil('\n');  // Arduino string
//     ard_str.trim();                          // Trim the string (remove the \n at the end)
//     command = std::string(ard_str.c_str());  // Convert to std::string

//     Serial.print("Received command: ");
//     Serial.println(command.c_str());

//     res = cmd.execute_command(&command);  // Execute command

//     Serial.print("Results: ");
//     Serial.println(res.c_str());

//     Serial.println();
//   }
// }

void loop() {
  if (Serial.available() > 0) {
    int tps = Serial.parseInt();
    CCD.set_integration_time(tps);
    Serial.print("Temps d'intégration: ");
    Serial.println(tps);
    delay(1000);
    while (Serial.available() > 0) { Serial.read(); }
  }

  // CCD.capture_data();
  // CCD.shift_data();

  CCD.acquire_data(10);

  for (int i = 0; i < CCD.get_n_pixel(); i++) {
    Serial.print(i);
    Serial.print(",");
    Serial.println(CCD.get_data(i));
  }

  delay(500);
}
