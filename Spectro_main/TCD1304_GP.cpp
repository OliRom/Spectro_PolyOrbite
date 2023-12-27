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
}

uint16_t TCD1304_GP::_one_pixel_read(){

}

void TCD1304_GP::_flush_data(){
  digitalWrite(_icg_pin, LOW);
  
}

void TCD1304_GP::capture_data(){
  digitalWriteFast(_sh_pin_Pm, _sh_pin_Pn, true);
  delayMicroseconds(1);
  digitalWriteFast(_sh_pin_Pm, _sh_pin_Pn, false);
  delayMicroseconds(_integration_time - 1);
}

void TCD1304_GP::shift_data(int16_t data[N_PIXELS]){
  noInterrupts();

  // Acquisition des données

  // Sortie des données
  ADCSelect(_os_pin_Pn, _os_pin_Pn, true);  // Sélection de la pin _os_pin lors des lectures analogiques
  for (int i=0; i<N_PIXELS; i++){
      _data[i] = _one_pixel_read();
  }

  interrupts();
}

void TCD1304_GP::set_integration_time(int time){
  _integration_time = time;
}