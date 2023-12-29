#ifndef TCD1304_GP_H
#define TCD1304_GP_H

#include <Arduino.h>
#include <stdint.h>

#define N_PIXELS 3694  // Nombre total de pixels
#define N_DUMMY_PIXELS_1 32  // Nombre de pixels inactifs au début
#define N_EFF_PIXELS 3648  // Nombre de pixels actifs
#define N_DUMMY_PIXELS_2 14  // Nombre de pixels inactifs à la fin


class TCD1304_GP{
  private:
    byte _clk_pin;  // Pin de la clock
    byte _os_pin;  // Pin où sortent les données
    byte _sh_pin;  // Pin du temps d'intégration
    byte _icg_pin;  // Pin du "Integration clear gate"

    // Valeurs Pn et An ou Pn des pins. Ces valeurs servent au lectures analogiques et écritures digitales rapides
    int _clk_pin_Pm;
    int _clk_pin_Pn;
    int _os_pin_An;
    int _os_pin_Pn;
    int _sh_pin_Pm;
    int _sh_pin_Pn;
    int _icg_pin_Pm;
    int _icg_pin_Pn;

    int _integration_time;  // Temps d'intégration (us)
    uint16_t _data[N_PIXELS];  // Liste contenant les données

    uint16_t _one_pixel_read();  // Lecture d'un pixel. Procédure à répérer N_PIXELS fois
    void _flush_data();  // Sortir toutes les données lorsque ICG monte
    void _pulse_clock();

  public:
    TCD1304_GP(byte clk_pin, byte os_pin, byte sh_pin, byte icg_pin);

    void capture_data();  // Lecture d'une image
    void shift_data();  // Obtenir les données d'une mesure

    void set_integration_time(int time);  // Spécifier le temps d'intégration (us)
    uint16_t get_data(int i);

    int get_n_pixel();
};

#endif
