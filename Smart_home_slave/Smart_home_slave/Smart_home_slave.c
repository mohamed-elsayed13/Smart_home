
/*
 * Smart_home_slave.c
 *
 * Created: 6/1/2020 5:14:54 AM
 *  Author: M
 */ 
/****************************************default temp******************************************************/
#define Temperature 25
/**********************************************************************************************************/
#include "SPI.h"
#include "ADC.h"
#include "Timer0.h"
uint16_t temp=0;		// for temperature reading of lm35
uint8_t Air_COND=0;		// flag to know if Air_COND. is on or off
int main(void)
{
	uint8_t select;		// to select which room to control 
	ADC_init();
	SPI_slave_init();
	timer_normal_init();
	SETBIT(DDRC,0);		//ROOM1
	SETBIT(DDRC,1);		//ROOM2
	SETBIT(DDRC,2);		//ROOM3
	SETBIT(DDRC,3);		//ROOM4
	SETBIT(DDRC,4);		//TV
	SETBIT(DDRC,5);		//Air_COND
	while(1)
    {
		
		select=SPI_receive_char();
		switch (select)
		{
		case '1':
		select=SPI_receive_char();
		if (select=='1'){SETBIT(PORTC,0);}
		else if (select=='2'){CLRBIT(PORTC,0);}
		break;
		case '2':
		select=SPI_receive_char();
		if (select=='1'){SETBIT(PORTC,1);}
		else if (select=='2'){CLRBIT(PORTC,1);}	
		break;
		case '3':
		select=SPI_receive_char();
		if (select=='1'){SETBIT(PORTC,2);}
		else if (select=='2'){CLRBIT(PORTC,2);}
		break;
		case '4':
		select=SPI_receive_char();
		if (select=='1'){SETBIT(PORTC,3);}
		else if (select=='2'){CLRBIT(PORTC,3);}
		break;
		case '5':
		select=SPI_receive_char();
		if (select=='1'){SETBIT(PORTC,4);}
		else if (select=='2'){CLRBIT(PORTC,4);}
		break;
		case '6':
		temp=ADC_read();
		temp=temp * 0.48828125;
		select=SPI_receive_char();
		if(select=='1'&& temp<Temperature){Air_COND=1;}		
		else if (select=='1'&& temp>=Temperature){
			SETBIT(PORTC,5);
			Air_COND=1;
			}
		else if (select=='2'){
			CLRBIT(PORTC,5);
			Air_COND=0;
			}
		break;
		default:
		break;
		}
		
    }
}
ISR(TIMER0_OVF_vect){			// to check temp. every 0.5 sec.
static uint8_t timer=0;
timer++;
temp=ADC_read();
temp=temp * 0.48828125;
if(timer==20&& Air_COND==1 && temp>=Temperature ){
SETBIT(PORTC,5);
timer=0;
}	
else if(timer==20&& Air_COND==1 && temp<=Temperature){
	CLRBIT(PORTC,5);
	timer=0;
}
}