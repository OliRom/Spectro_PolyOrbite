#ifndef TCD1304_GP_H
#define TCD1304_GP_H

#include <Arduino.h>
// #include <stdint.h>

#define N_PIXELS 3694  // Nombre total de pixels
#define N_DUMMY_PIXELS_1 16  // Nombre de pixels inactifs au début
#define N_SHIELDED_PIXELS 13  // Nombre de pixels cachés de la lumière
#define N_BUFFER_PIXELS 3  // Nombre de pixels avant les pixels effectifs
#define N_EFF_PIXELS 3648  // Nombre de pixels actifs
#define N_DUMMY_PIXELS_2 14  // Nombre de pixels inactifs à la fin


union pixels_format {
  uint16_t all[N_PIXELS];

  struct {
    uint16_t dummy_1[N_DUMMY_PIXELS_1];  // Pixels dummy au début
    uint16_t shielded[N_SHIELDED_PIXELS];  // Pixels cachés de la lumière
    uint16_t buffer[N_BUFFER_PIXELS];  // Pixels de transition entre les shielded et les efficaces
    uint16_t effective[N_EFF_PIXELS];  // Pixels efficaces; ceux qui servent à mesurer
    uint16_t dummy_2[N_DUMMY_PIXELS_2];  // Pixels dummy à la fin
  } util;
};


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
    //uint16_t _data[N_PIXELS];  // Liste contenant les données
    pixels_format _data;  // Structure contenant les données des pixels

    uint16_t _one_pixel_read();  // Lecture d'un pixel. Procédure à répérer N_PIXELS fois
    void _flush_data();  // Sortir toutes les données lorsque ICG monte
    void _pulse_clock();

  public:
    TCD1304_GP(byte clk_pin, byte os_pin, byte sh_pin, byte icg_pin);

    void capture_data();  // Lecture d'une image
    void shift_data(bool replace_data=true);  // Obtenir les données d'une mesure
    void acquire_data(int acquisition_nb=1);  // Lire les données du capteur. Équivalent à "capture_data()" suivi de "shift_data()".

    void set_integration_time(int time);  // Spécifier le temps d'intégration (us)
    uint16_t get_data(int i);

    int get_n_pixel();
};

#endif
