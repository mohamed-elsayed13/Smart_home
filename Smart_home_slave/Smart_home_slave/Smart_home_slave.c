
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
		select=SPI_receive_char();
		if (select=='1'){SETBIT(PORTC,5);}
		else if (select=='2'){CLRBIT(PORTC,5);}
		break;
		default:
		SPI_send_char('n');
		break;
		}
		
    }
}