
/*
 * SPI_slave.c
 *
 * Created: 4/27/2020 5:14:54 PM
 *  Author: safifi
 */ 


#include "SPI.h"
#include "LCD.h"
int main(void)
{
	uint8_t select;
	LCD_init();
	SPI_slave_init();
	SETBIT(DDRC,0);
	SETBIT(DDRC,1);
	SETBIT(DDRC,2);
	SETBIT(DDRC,3);
	SETBIT(DDRC,4);
	SETBIT(DDRC,5);
	while(1)
    {
		
		select=SPI_receive_char();
		LCD_write_char(select);
		switch (select)
		{
		case '1':
		TOGBIT(PORTC,0);
		SPI_send_char(GETBIT(PORTC,0));
		break;
		case '2':
		TOGBIT(PORTC,1);
		SPI_send_char(GETBIT(PORTC,1));
		break;
		case '3':
		TOGBIT(PORTC,2);
		SPI_send_char(GETBIT(PORTC,2));
		break;
		case '4':
		TOGBIT(PORTC,3);
		SPI_send_char(GETBIT(PORTC,3));
		break;
		case '5':
		TOGBIT(PORTC,4);
		SPI_send_char(GETBIT(PORTC,4));
		break;
		case '6':
		TOGBIT(PORTC,5);
		SPI_send_char(GETBIT(PORTC,5));
		break;
		default:
		SPI_send_char('n');
		break;
		}
		/*if(select!='0'){
		SETBIT(PORTC,0);
		
		
		}
		else{
		CLRBIT(PORTC,0);
		}*/
    }
}