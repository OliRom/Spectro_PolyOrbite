#include "api/Common.h"
#include "Arduino.h"
#include "TCD1304_GP.h"
#include "Fonctions.h"

TCD1304_GP::TCD1304_GP(byte clk_pin, byte os_pin, byte sh_pin, byte icg_pin) : _clk_pin(clk_pin), _os_pin(os_pin), _sh_pin(sh_pin), _icg_pin(icg_pin) {
  pinMode(_clk_pin, OUTPUT);
  pinMode(_sh_pin, OUTPUT);
  pinMode(_icg_pin, OUTPUT);

  _clk_pin_Pm = get_Pm(_clk_pin);
  _clk_pin_Pn = get_Pn(_clk_pin);
  _os_pin_An = get_ANn(_os_pin);
  _os_pin_Pn = get_Pn(_os_pin);
  _sh_pin_Pm = get_Pm(_sh_pin);
  _sh_pin_Pn = get_Pn(_sh_pin);
  _icg_pin_Pm = get_Pm(_icg_pin);
  _icg_pin_Pn = get_Pn(_icg_pin);

  ADCSetup();  // Initialisation du convertisseur ADC0

  digitalWriteFast(_clk_pin_Pm, _clk_pin_Pn, true);
}

uint16_t TCD1304_GP::_one_pixel_read(){
  _pulse_clock();  // 1000 ns
  ADCStart(false);
  _pulse_clock();  // 1000 ns
  _pulse_clock();  // 1000 ns
  _pulse_clock();  // 1000 ns
  return ADCRead(_os_pin_An);
}

void TCD1304_GP::_flush_data(){
  digitalWrite(_icg_pin, LOW);
  
}

void TCD1304_GP::_pulse_clock(){
  digitalWriteFast(_clk_pin_Pm, _clk_pin_Pn, false);  // 38 ns
  delayTicks(4);  // (500 - 38) ns
  digitalWriteFast(_clk_pin_Pm, _clk_pin_Pn, true);  // 38 ns
  delayTicks(4);  // (500 - 38) ns
}

void TCD1304_GP::capture_data(){
  digitalWriteFast(_sh_pin_Pm, _sh_pin_Pn, true);
  delayMicroseconds(1);
  digitalWriteFast(_sh_pin_Pm, _sh_pin_Pn, false);
  delayMicroseconds(_integration_time - 2);
  digitalWriteFast(_icg_pin_Pm, _icg_pin_Pn, false);
  delayTicks(1);
  digitalWriteFast(_sh_pin_Pm, _sh_pin_Pn, true);
  delayMicroseconds(1);
  digitalWriteFast(_sh_pin_Pm, _sh_pin_Pn, false);
  delayMicroseconds(5);
  digitalWriteFast(_icg_pin_Pm, _icg_pin_Pn, true);
}

void TCD1304_GP::shift_data(){
  noInterrupts();

  // Acquisition des données

  // Sortie des données
  ADCSelect(_os_pin_Pn, _os_pin_An, true);  // Sélection de la pin _os_pin lors des lectures analogiques
  for (int i=0; i<N_PIXELS; i++){
      _data[i] = _one_pixel_read();
  }

  interrupts();
}

void TCD1304_GP::set_integration_time(int time){
  _integration_time = time;
}

uint16_t TCD1304_GP::get_data(int i){
  return _data[i];  // Retourne le pointeur
}

int TCD1304_GP::get_n_pixel(){
  return N_PIXELS;
}