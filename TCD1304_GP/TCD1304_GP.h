#ifndef TCD1304_GP_H
#define TCD1304_GP_H

#include <Arduino.h>
#include <stdint.h>

#define N_PIXELS 3694  // Nombre total de pixels
#define N_DUMMY_PIXELS_1 32  // Nombre de pixels inactifs au début
#define N_EFF_PIXELS 3648  // Nombre de pixels actifs
#define N_DUMMY_PIXELS_2 14  // Nombre de pixels inactifs à la fin

#define CLK_FREQ 2e6  // Fréquence de la clock (Hz)


class TCD1304_GP{
  private:
    byte _clk_pin;  // Pin de la clock
    byte _os_pin;  // Pin où sortent les données
    byte _sh_pin;  // Pin du temps d'intégration
    byte _icg_pin;  // Pin du "Integration clear gate"

    int _integration_time;  // Temps d'intégration (us)
    int16_t _data[N_PIXELS];  // Liste contenant les données

  public:
    TCD1304_GP(byte clk_pin, byte os_pin, byte sh_pin, byte icg_pin);

    void read_data();  // Lecture d'une image
    void get_data(int16_t data[N_PIXELS]);  // Obtenir les données d'une mesure

    void set_integration_time(int time);  // Spécifier le temps d'intégration (us)
}

#endif
