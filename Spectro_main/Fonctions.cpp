#include "Arduino.h"
#include "Fonctions.h"


void ADCSetup(){
  R_PMISC->PWPR_b.B0WI = 0;  // Déverrouiller l'écriture dans le bit PFSWE
  R_PMISC->PWPR_b.PFSWE = 1;  // Activer l'écriture dans le registre PmnPFS
  R_PMISC->PWPR_b.B0WI = 0;  // Verrouiller le registre PFSWE

  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA502 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)));  // Déverrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA501 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)));  // Déverrouiller l'accès aux registres système
  // R_MSTP->MSTPCRD_b.MSTPD15 = 0;  // Activer l'ADC1
  R_MSTP->MSTPCRD_b.MSTPD16 = 0;  // Activer l'ADC0
  R_SYSTEM->SCKDIVCR_b.PCKA = 0b000;  // Division de la clock PCKA par 1
  R_SYSTEM->SCKDIVCR_b.PCKC = 0b000;  // Division de la clock PCKC par 1
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système

  R_ADC0->ADCER_b.ADRFMT = 0;  // Format de données justifié à droite
  R_ADC1->ADCER_b.ADRFMT = 0;
  R_ADC0->ADCER_b.ADPRC = 0b00;  // Résolution de 12 bits
  R_ADC1->ADCER_b.ADPRC = 0b00;
}


void ADCSelect(int Pn, int An, bool activation){
  // Associer la fonction analogue à la pin Pmn
  if (activation){
    volatile uint32_t *PmnPFS_ptr = (volatile uint32_t *)(0x40080800 + 0x4 * Pn);  // Adresse du registre PmnPFS
    *PmnPFS_ptr = ((unsigned long)*PmnPFS_ptr & R_PFS_PORT_PIN_PmnPFS_PSEL_Msk) | 0x0A008000;  // 0b 0000 1010 0000 0000 1000 0000 0000 0000
  }

  // Sélectionner/déselectionner la pin Amn lors d'une lecture analogique
  uint16_t mask = 0b1 << An;
  R_ADC0->ADANSA[0] = (R_ADC0->ADANSA[0] & ~mask) | (activation * mask);
}


void ADCStart(){
  R_ADC0->ADCSR_b.ADST = 1;  // Activer la conversion ADC
  while (R_ADC0->ADCSR_b.ADST);  // Attendre la fin de la conversion
}


uint16_t ADCRead(int m){  // Utiliser le nom de la pin ANmn
  return R_ADC0->ADDR[m];  // Lire le registre où la valeur est enregistrée
}


int get_ANn(int pin){
  switch (pin){
    case A0: return 6;
    case A1: return 5;
    case A2: return 4;
    case A3: return 2;
    case A4: return 1;
    case A5: return 3;
    case A6: return 12;
  }
}


int get_Pn(int pin){
  switch (pin){
    case A0: return 6;
    case A1: return 5;
    case A2: return 4;
    case A3: return 2;
    case A4: return 1;
    case A5: return 15;
    case A6: return 14;

    case D0: return 5;
    case D1: return 6;
    case D2: return 11;
    case D3: return 3;
    case D4: return 1;
    case D5: return 10;
    case D6: return 1;
    case D7: return 2;
    /*case D8: return 0;
    case D9: return 4;
    case D10: return 15;
    case D11: return 7;
    case D12: return 8;
    case D13: return 10;
    case D14: return 2;*/
  }
}


int get_Pm(int pin){
  switch (pin){
    case D0: return 1;
    case D1: return 1;
    case D2: return 1;
    case D3: return 3;
    case D4: return 4;
    case D5: return 2;
    case D6: return 6;
    case D7: return 4;
    /*case D8: return 9;
    case D9: return 2;
    case D10: return 3;
    case D11: return 4;
    case D12: return 4;
    case D13: return 1;
    case D14: return 6;*/
  }
}


void digitalWriteFast(byte m, byte n, bool state){  // Utiliser le nom de la pin Pmn
  volatile uint32_t *PmnPFS_ptr = (volatile uint32_t *)(0x40080800 + 0x040 * m + 0x4 * n);  // Adresse du registre PmnPFS
  *PmnPFS_ptr = 0x0B000804 | state;  // 0b 0000 1011 0000 0000 0000 1000 0000 0100
}