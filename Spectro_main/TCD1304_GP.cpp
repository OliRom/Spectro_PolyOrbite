#include "Arduino.h"
#include "TCD1304_GP.h"
#include "Fonctions.h"

TCD1304_GP::TCD1304_GP(byte clk_pin, byte os_pin, byte sh_pin, byte icg_pin) : _clk_pin(clk_pin), _os_pin(os_pin), _sh_pin(sh_pin), _icg_pin(icg_pin) {
  pinMode(_clk_pin, OUTPUT);
  pinMode(_sh_pin, OUTPUT);
  pinMode(_icg_pin, OUTPUT);

  int _clk_pin_Pm = get_Pm(_clk_pin);
  int _clk_pin_Pn = get_Pn(_clk_pin);
  int _os_pin_An = get_ANn(_os_pin);
  int _os_pin_Pn = get_Pn(_os_pin);
  int _sh_pin_Pm = get_Pm(_sh_pin);
  int _sh_pin_Pn = get_Pn(_sh_pin);
  int _icg_pin_Pm = get_Pm(_icg_pin);
  int _icg_pin_Pn = get_Pn(_icg_pin);

  ADCSetup();  // Initialisation du convertisseur ADC0
}

uint16_t TCD1304_GP::_one_pixel_read(){

}

void TCD1304_GP::_flush_data(){
  digitalWrite(_icg_pin, LOW);
  
}

void TCD1304_GP::read_data(){
  noInterrupts();

  // Acquisition des données

  // Sortie des données
  ADCSelect(_os_pin_Pn, _os_pin_Pn, true);  // Sélection de la pin _os_pin lors des lectures analogiques
  for (int i=0; i<N_PIXELS; i++){
      _data[i] = _one_pixel_read();
  }

  interrupts();
}

void TCD1304_GP::get_data(int16_t data[N_PIXELS]){

}

void TCD1304_GP::set_integration_time(int time){

}