/*
 Classe pour contrôler le CCD.
*/

#ifndef TCD1304_GP_H
#define TCD1304_GP_H

#include <Arduino.h>

#define PIXEL_DATA_TYPE uint32_t  // Type de données des pixels (uint16_t, uint32_t, ...)
#define N_PIXELS 3694             // Nombre total de pixels
#define N_DUMMY_PIXELS_1 16       // Nombre de pixels inactifs au début
#define N_SHIELDED_PIXELS 13      // Nombre de pixels cachés de la lumière
#define N_BUFFER_PIXELS 3         // Nombre de pixels avant les pixels effectifs
#define N_EFF_PIXELS 3648         // Nombre de pixels actifs
#define N_DUMMY_PIXELS_2 14       // Nombre de pixels inactifs à la fin


enum read_destination {
  DATA,
  CALIBRATION,
};

union pixels_format {
  PIXEL_DATA_TYPE all[N_PIXELS];

  struct {
    PIXEL_DATA_TYPE dummy_1[N_DUMMY_PIXELS_1];    // Pixels dummy au début
    PIXEL_DATA_TYPE shielded[N_SHIELDED_PIXELS];  // Pixels cachés de la lumière
    PIXEL_DATA_TYPE buffer[N_BUFFER_PIXELS];      // Pixels de transition entre les shielded et les efficaces
    PIXEL_DATA_TYPE effective[N_EFF_PIXELS];      // Pixels efficaces; ceux qui servent à mesurer
    PIXEL_DATA_TYPE dummy_2[N_DUMMY_PIXELS_2];    // Pixels dummy à la fin
  } util;
};


class TCD1304_GP {
private:
  byte _clk_pin;  // Pin de la clock
  byte _os_pin;   // Pin où sortent les données
  byte _sh_pin;   // Pin du temps d'intégration
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
  // PIXEL_DATA_TYPE _data[N_PIXELS];  // Liste contenant les données<
  pixels_format _calibration;  // Données de calibration
  pixels_format _data;         // Structure contenant les données des pixels

  PIXEL_DATA_TYPE _one_pixel_read();  // Lecture d'un pixel. Procédure à répérer N_PIXELS fois
  void _flush_data();                 // Sortir toutes les données lorsque ICG monte
  void _pulse_clock();

  void _capture_data();                                                             // Lecture d'une image; acquisition par le CCD
  void _shift_data(bool replace_data = true, read_destination destination = DATA);  // Obtenir les données d'une mesure à partir du CCD pour les stocker en mémoire

public:
  TCD1304_GP(byte clk_pin, byte os_pin, byte sh_pin, byte icg_pin);

  // Lire les données du capteur. Correspond à "_capture_data()" suivi de "_shift_data()".
  // Mettre calibration à true pour enregistrer les données dans le registre de calibration.
  void acquire_data(int acquisition_nb = 1, read_destination destination = DATA);

  void set_integration_time(int time);                                       // Spécifier le temps d'intégration (us)
  const pixels_format* get_data_pointer(read_destination destination = DATA) const;  // Pointeur vers les données en mémoire
  PIXEL_DATA_TYPE get_data(int i, read_destination destination = DATA);      // Retourne la valeur du pixel i

  int get_n_pixel();  // Obtenir le nombre de pixels du CCD
};

#endif
