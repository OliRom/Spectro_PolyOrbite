#include <stdint.h>
#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <Arduino.h>

/* * * Pour les lectures analogiques rapides * * */
void ADCSetup();  // Activation de la pin Amn en mode analogique
void ADCSelect(int Pn, int An, bool activation=true);  // Sélection de la pin lors des lectures analogues
void ADCStart(bool wait);  // Débuter une lecture analogique (1.5MHz)
uint16_t ADCRead(int n);  // Obtenir la valeur de la lecture analogique - Utiliser ANmn

int get_ANn(int pin);  // Obtenir la valeur n dans ANmn de la pin
int get_Pn(int pin);  // Obtenir la valeur n dans Pmn de la pin
int get_Pm(int pin);  // Obtenir la valeur m dans Pmn de la pin


/* * * Pour les écritures digitales rapides * * */
void digitalWriteFast(byte m, byte n, bool state);  // Écriture rapide sur une pin digitale (38.0 ns = 26.3 MHz) - Utiliser Pmn


/* * * Delay * * */
#define delayTicks(n) { volatile int i; for(i=0; i<n; ++i) {} }  // Delai = 72.5 * n + 96 (ns)


/* * * Pour le PWM * * */
void PWMSetup(int pin);
void PWMPinSelect(int pin, bool activation=true);
void PWMSetPeriod(int pin, uint32_t T);  // Clk frequency: 100MHz
void PWMSetDutyCycle(int pin, uint32_t n);
void PWMStart(int pin, bool state, bool wait=false);  // Attendre la fin du cycle avant de passer à la prochaine instruction si wait=true

R_GPT0_Type* get_GPT_n(int pin);  // Obtenir le canal GPT (R_GPTn) de la pin
bool PWM_pin_is_A(int pin);  // Savoir si la pin est GTIOnA ou GTIOnB

#endif