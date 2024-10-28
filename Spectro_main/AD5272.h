#ifndef AD5272_H
#define AD5272_H

#include <Arduino.h>

// Address codes
#define ADDR_GND 0b010'1111
#define ADDR_VDD 0b010'1100
#define ADDR_NC  0b010'1110

// Commands
#define RDAC_W 0b0001  // Write to RDAC
#define RDAC_R 0b0010  // Read content of RDAC
#define SOFT_RST 0b0100  // Software reset
#define CTRL_W 0b0111  // Write to control register
#define CTRL_R 0b1000  // Read content of control register
#define SOFT_SHUT_DOWN 0b1001  // Software shutdown

// Control register bits
#define RDAC_WE_BIT 1  // RDAC write enable bit number


// Structure de données pour formatter les commandes à envoyer
struct _AD5272_cmd_format {
  uint16_t data : 10;
  uint16_t cmd : 4;

  _AD5272_cmd_format(): data(0), cmd(0) {};
  _AD5272_cmd_format(byte cmd, uint16_t data): cmd(cmd), data(data) {};

  byte get_w1() {return (cmd << 2) | ((data >> 8) & 0b11);}  // Obtenir le premier byte de la commande
  byte get_w2() {return data & 0xFF;}  // Obtenir le second byte de la commande
  uint16_t get_2w() {return (cmd << 10) | data;}  // Obtenir les 2 bytes de la commande
};


class AD5272 {
  private:
    byte _addr;  // Adresse I2C

    void _send_cmd(byte cmd, uint16_t data=0);  // Envoyer une commande par I2C sans réponse
    void _send_cmd(_AD5272_cmd_format data);
    
    uint16_t _send_cmd_resp(byte cmd, uint16_t data=0);  // Envoyer une commande par I2C avec réponse
    uint16_t _send_cmd_resp(_AD5272_cmd_format data);

  public:
    AD5272(byte addr);  // Initialisation
    void init();  // À appeler avant la première utilisation une fois que Wire.begin() a été exécuté, et après chaque Software Reset.

    void set_res(uint16_t value);  // Set la valeur de la résistance
    uint16_t get_res();  // Get la valeur de la résistance

    void set_ctrl(byte bit, bool value);  // Set la valeur d'un bit du registre de controle
    byte get_ctrl();  // Get la valeur du registre de controle
    
    void software_reset();  // Software reset
    void software_shutdown(bool shut_down);  // Éteindre/allumer le software
};

#endif