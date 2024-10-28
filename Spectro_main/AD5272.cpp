#include <Arduino.h>
#include <Wire.h>
#include "AD5272.h"


AD5272::AD5272(byte addr) {
  _addr = addr;
}

void AD5272::init() {
  set_ctrl(RDAC_WE_BIT, true);
}

void AD5272::_send_cmd(byte cmd, uint16_t data) {
  _AD5272_cmd_format msg {cmd, data};
  _send_cmd(msg);
}

void AD5272::_send_cmd(_AD5272_cmd_format data) {
  Wire.beginTransmission(_addr);
  Wire.write(data.get_w1());
  Wire.write(data.get_w2());
  Wire.endTransmission();
}


uint16_t AD5272::_send_cmd_resp(byte cmd, uint16_t data) {
  _AD5272_cmd_format msg {cmd, data};
  return _send_cmd_resp(msg);
}

uint16_t AD5272::_send_cmd_resp(_AD5272_cmd_format data) {
  _send_cmd(data);

  uint16_t resp;
  Wire.requestFrom(_addr, 2);

  while (Wire.available()) {
    byte received_byte = Wire.read(); 
    resp = (resp << 8) | received_byte;
  }

  return resp;
}


void AD5272::set_res(uint16_t value) {_send_cmd(RDAC_W, value);}
uint16_t AD5272::get_res() {return _send_cmd_resp(RDAC_R);}


void AD5272::set_ctrl(byte bit, bool value) {
  byte new_ctrl = get_ctrl();
  bitWrite(new_ctrl, bit, (byte) value);
  _send_cmd(CTRL_W, (uint16_t) new_ctrl);
}

byte AD5272::get_ctrl() {return (byte) _send_cmd_resp(CTRL_R);}


void AD5272::software_reset() {_send_cmd(SOFT_RST);}
void AD5272::software_shutdown(bool shut_down) {_send_cmd(SOFT_SHUT_DOWN, (uint16_t) shut_down);}
