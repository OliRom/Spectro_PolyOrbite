#include <Arduino.h>
#include "Fonctions.h"


void ADCSetup() {
  R_PMISC->PWPR_b.B0WI = 0;   // Déverrouiller l'écriture dans le bit PFSWE
  R_PMISC->PWPR_b.PFSWE = 1;  // Activer l'écriture dans le registre PmnPFS
  R_PMISC->PWPR_b.B0WI = 1;   // Verrouiller le registre PFSWE

  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA502 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)));  // Déverrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA501 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)));  // Déverrouiller l'accès aux registres système
  // R_MSTP->MSTPCRD_b.MSTPD15 = 0;  // Activer l'ADC1
  R_MSTP->MSTPCRD_b.MSTPD16 = 0;                                                                                                   // Activer l'ADC0
  R_SYSTEM->SCKDIVCR_b.PCKA = 0b001;                                                                                               // Division de la clock PCKA par 2
  R_SYSTEM->SCKDIVCR_b.PCKC = 0b001;                                                                                               // Division de la clock PCKC par 2
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système

  R_ADC0->ADCER_b.ADRFMT = 0;  // Format de données justifié à droite
  // R_ADC1->ADCER_b.ADRFMT = 0;
  R_ADC0->ADCER_b.ADPRC = 0b00;  // Résolution de 12 bits
  // R_ADC1->ADCER_b.ADPRC = 0b00;
}


void ADCSelect(int Pn, int An, bool activation) {
  // Associer la fonction analogue à la pin Pmn
  if (activation) {
    volatile uint32_t *PmnPFS_ptr = (volatile uint32_t *)(0x40080800 + 0x4 * Pn);              // Adresse du registre PmnPFS
    *PmnPFS_ptr = ((unsigned long)*PmnPFS_ptr & R_PFS_PORT_PIN_PmnPFS_PSEL_Msk) | 0x0A008000;  // 0b 0000 1010 0000 0000 1000 0000 0000 0000
  }

  // Sélectionner/déselectionner la pin Amn lors d'une lecture analogique
  uint16_t mask = 0b1 << An;
  R_ADC0->ADANSA[0] = (R_ADC0->ADANSA[0] & ~mask) | (activation * mask);
}


void ADCStart(bool wait = true) {
  R_ADC0->ADCSR_b.ADST = 1;  // Activer la conversion ADC
  if (wait) {
    while (R_ADC0->ADCSR_b.ADST)
      ;
  }  // Attendre la fin de la conversion
}


uint16_t ADCRead(int n) {  // Utiliser le nom de la pin ANmn
  return R_ADC0->ADDR[n];  // Lire le registre où la valeur est enregistrée
}


int get_ANn(int pin) {
  switch (pin) {
    case A0: return 6;
    case A1: return 5;
    case A2: return 4;
    case A3: return 2;
    case A4: return 1;
    case A5: return 3;
    case A6: return 12;
  }
}


int get_Pn(int pin) {
  switch (pin) {
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
    case D7:
      return 2;
      /*case D8: return 0;
    case D9: return 4;
    case D10: return 15;
    case D11: return 7;
    case D12: return 8;
    case D13: return 10;
    case D14: return 2;*/
  }
}


int get_Pm(int pin) {
  switch (pin) {
    case D0: return 1;
    case D1: return 1;
    case D2: return 1;
    case D3: return 3;
    case D4: return 4;
    case D5: return 2;
    case D6: return 6;
    case D7:
      return 4;
      /*case D8: return 9;
    case D9: return 2;
    case D10: return 3;
    case D11: return 4;
    case D12: return 4;
    case D13: return 1;
    case D14: return 6;*/
  }
}


void digitalWriteFast(byte m, byte n, bool state) {                                         // Utiliser le nom de la pin Pmn
  volatile uint32_t *PmnPFS_ptr = (volatile uint32_t *)(0x40080800 + 0x040 * m + 0x4 * n);  // Adresse du registre PmnPFS
  *PmnPFS_ptr = 0x0B000804 | state;                                                         // 0b 0000 1011 0000 0000 0000 1000 0000 0100
}


void PWMSetup(int pin) {
  R_PMISC->PWPR_b.B0WI = 0;   // Déverrouiller l'écriture dans le bit PFSWE
  R_PMISC->PWPR_b.PFSWE = 1;  // Activer l'écriture dans le registre PmnPFS
  R_PMISC->PWPR_b.B0WI = 1;   // Verrouiller le registre PFSWE

  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA502 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)));  // Déverrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA501 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)));  // Déverrouiller l'accès aux registres système

  switch (pin) {
    case D0:
      R_MSTP->MSTPCRE_b.MSTPE30 = 0;  // Activer le GPT1
      break;

    case D1:
      R_MSTP->MSTPCRE_b.MSTPE23 = 0;  // GPT8
      break;

    case D2:
      R_MSTP->MSTPCRE_b.MSTPE28 = 0;  // GPT3
      break;

    case D3:
      R_MSTP->MSTPCRE_b.MSTPE24 = 0;  // GPT7
      break;

    case D4:
      R_MSTP->MSTPCRE_b.MSTPE25 = 0;  // GPT6
      break;

      /* case D5:
      R_MSTP->MSTPCRE_b.MSTPE?? = 0;  // Pas associé à un GPT particulier
      break;*/

    case D6:
      R_MSTP->MSTPCRE_b.MSTPE25 = 0;  // GPT6
      break;
  }

  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système
}


void PWMPinSelect(int pin, bool activation) {
  int m = get_Pm(pin);
  int n = get_Pn(pin);
  R_GPT0_Type *GPT = get_GPT_n(pin);

  volatile uint32_t *PmnPFS_ptr = (volatile uint32_t *)(0x40080800 + 0x40 * m + 0x4 * n);  // Adresse du registre PmnPFS
  *PmnPFS_ptr = 0b0000'0011'0000'0001'0000'0000'0000'0000;                                 // Associer la Pin à la fonction GPT en mode I/O périphérique

  if (PWM_pin_is_A(pin)) {
    GPT->GTIOR_b.OAE = activation;  // Activer la sortie de la pin GTIOCnA
    GPT->GTIOR_b.GTIOA = 0b1001;    // Fonction de la pin GTIOCnA (voir tableau 21.4 du datasheet)
    GPT->GTBER_b.CCRA = 0b01;       // GTCCRA buffer operation (GTCCRA<->GTCCRC)
  } else {
    GPT->GTIOR_b.OBE = activation;  // Activer la sortie de la pin GTIOCnB
    GPT->GTIOR_b.GTIOB = 0b1001;    // Fonction de la pin GTIOCnB (voir tableau 21.4 du datasheet)
    GPT->GTBER_b.CCRB = 0b01;       // GTCCRB buffer operation (GTCCRB<->GTCCRE)
  }

  GPT->GTBER_b.PR = 0b01;  // GTPR buffer operation (GTPBR<->GTPR)
}


void PWMSetPeriod(int pin, uint32_t T) {
  R_GPT0_Type *GPT = get_GPT_n(pin);

  // Période réelle = T+1
  GPT->GTPBR = T - 1;                                                  // Buffer du compteur
  if ((T > GPT->GTCNT) and ~(GPT->GTCR_b.CST)) { GPT->GTPR = T - 1; }  // Si T < compteur et si le compteur est arrêté
}


void PWMSetDutyCycle(int pin, uint32_t n) {
  R_GPT0_Type *GPT = get_GPT_n(pin);

  // Duty cycle réel = n+1
  if (PWM_pin_is_A(pin)) {
    GPT->GTCCR[2] = n - 1;  // Registre de comparaison
  } else {
    GPT->GTCCR[3] = n - 1;  // Registre de comparaison
  }
}


void PWMStart(int pin, bool state, bool wait) {
  R_GPT0_Type *GPT = get_GPT_n(pin);
  if (state) {
    GPT->GTPC_b.ASTP = 0;  // Désactiver le stop function
    GPT->GTPC_b.PCEN = 0;  // Désactiver le period counter
    GPT->GTCR_b.CST = 1;   // Activer le compteur
  } else {
    GPT->GTPC_b.PCNT = 1;  // Period counter
    GPT->GTPC_b.ASTP = 1;  // Activer le stop function
    GPT->GTPC_b.PCEN = 1;  // Activer le period counter

    while (wait && GPT->GTCR_b.CST) {}  // Attendre la fin du cycle
  }

  // GPT->GTCR_b.CST = state;  // État du compteur (activé/désactivé)
}

R_GPT0_Type *get_GPT_n(int pin) {
  switch (pin) {
    case D0: return R_GPT1;
    case D1: return R_GPT8;
    case D2: return R_GPT3;
    case D3: return R_GPT7;
    case D4: return R_GPT6;
    // case D5: return ???;
    case D6: return R_GPT6;
  }
}

bool PWM_pin_is_A(int pin) {
  switch (pin) {
    case D0: return true;
    case D1: return false;
    case D2: return true;
    case D3: return false;
    case D4: return false;
    // case D5: return ???;
    case D6: return true;
  }
}