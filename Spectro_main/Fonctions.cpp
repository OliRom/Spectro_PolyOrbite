void ADC6_setup(){
  // Configuration de AN012/P014 en mode analogique
  R_PMISC->PWPR_b.B0WI = 0;  // Déverrouiller l'écriture dans le bit PFSWE
  R_PMISC->PWPR_b.PFSWE = 1;  // Activer l'écriture dans le registre PmnPFS

  // Associer la fonction analogue à la pin P014
  volatile uint32_t *P046PFS_ptr = (volatile uint32_t *)(0x40080800 + 0x4 * 14);  // Adresse du registre P046PFS (m=0, n=14)
  *P046PFS_ptr = ((unsigned long)*P046PFS_ptr & R_PFS_PORT_PIN_PmnPFS_PSEL_Msk) | 0x0A008000;  // 0b 0000 1010 0000 0000 1000 0000 0000 0000

  R_PMISC->PWPR_b.B0WI = 0;  // Verrouiller le registre PFSWE


  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA502 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)));  // Déverrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA501 & (R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)));  // Déverrouiller l'accès aux registres système
  R_MSTP->MSTPCRD_b.MSTPD15 = 0;  // Activer l'ADC1
  R_MSTP->MSTPCRD_b.MSTPD16 = 0;  // Activer l'ADC0
  R_SYSTEM->SCKDIVCR_b.PCKA = 0b000;  // Division de la clock PCKA par 1
  R_SYSTEM->SCKDIVCR_b.PCKC = 0b000;  // Dicision de la clock PCKC par 1
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC1_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système
  R_SYSTEM->PRCR = ((R_SYSTEM->PRCR & ~(R_SYSTEM_PRCR_PRKEY_Msk | R_SYSTEM_PRCR_PRC0_Msk)) | (0xA500 & R_SYSTEM_PRCR_PRKEY_Msk));  // Verrouiller l'accès aux registres système

  R_ADC0->ADANSA_b[0].ANSA12 = 1;  // Sélection du canal m=0, n=12
  
  R_ADC0->ADCER_b.ADRFMT = 0;  // Format de données justifié à droite
  R_ADC0->ADCER_b.ADPRC = 0b00;  // Résolution de 12 bits
}


uint16_t ADC6_analogRead(){
  R_ADC0->ADCSR_b.ADST = 1;  // Activer la conversion ADC
  while (R_ADC0->ADCSR_b.ADST);  // Attendre la fin de la conversion

  return R_ADC0->ADDR[12];  // Lire le registre où la valeur est enregistrée
}


void digitalWriteFast(byte m, byte n, bool state){
  volatile uint32_t *PmnPFS_ptr = (volatile uint32_t *)(0x40080800 + 0x040 * m + 0x4 * n);  // Adresse du registre PmnPFS
  *PmnPFS_ptr = 0x0B000804 | state;  // 0b 0000 1011 0000 0000 0000 1000 0000 0100
}