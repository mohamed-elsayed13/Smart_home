/*
 * EEPROM.h
 *
 * Created: 13/04/2020 4:30:12 AM
 *  Author: M
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include "header.h"
uint8_t EEPROM_read(uint16_t add); // add from 0 to 1023 in atmega 32
void EEPROM_write(uint16_t add , uint8_t data);



#endif /* EEPROM_H_ */