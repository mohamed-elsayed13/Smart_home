/*
 * ADC.c
 *
 * Created: 4/12/2020 4:02:43 PM
 *  Author: safifi
 */ 

#include "ADC.h"

void ADC_init(){
	SETBIT(ADMUX,REFS0);
	SETBIT(ADCSRA,ADEN);
	SETBIT(ADCSRA,ADPS0);
	SETBIT(ADCSRA,ADPS1);
	SETBIT(ADCSRA,ADPS2);	
}

uint16_t ADC_read(){

	SETBIT(ADCSRA,ADSC); // start conversion
	while(GETBIT(ADCSRA,ADSC) == 1){}
	return ADC;
}