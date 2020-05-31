/*
 * EEPROM.c
 *
 * Created: 13/04/2020 4:29:43 AM
 *  Author: M
 */ 
#include "EEPROM.h"

uint8_t EEPROM_read(uint16_t add){
	while(GETBIT(EECR,EEWE)==1){};
		EEAR =add;
		SETBIT(EECR,EERE);
		return EEDR;
}
void EEPROM_write(uint16_t add , uint8_t data){ 
	while(GETBIT(EECR,EEWE)==1){};
    EEAR =add;
	EEDR =data;
	SETBIT(EECR,EEMWE);
	SETBIT(EECR,EEWE);
}
