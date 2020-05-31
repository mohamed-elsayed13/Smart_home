/*
 * LCD.c
 *
 * Created: 24/03/2020 5:17:09 PM
 *  Author: M
 */ 
#include "LCD.h"

void LCD_init (){
INIT_LCD();
LCD_write_command(0X3);
_delay_ms(4);
LCD_write_command(0X3);
_delay_ms(4);
LCD_write_command(0X3);
_delay_ms(4);
LCD_write_command(0X2);
LCD_write_command(0X28);
LCD_write_command(0X08);
LCD_write_command(0X01);
LCD_write_command(0X06);
_delay_ms(20);
LCD_write_command(0x0C);
}

void LCD_write_command(uint8_t cmd){
	 RS(0);
	 // send most byte
	 D4(GETBIT(cmd,4));
	 D5(GETBIT(cmd,5));
	 D6(GETBIT(cmd,6));
	 D7(GETBIT(cmd,7));
	 EN(1);
	 _delay_ms(1);
	 EN(0);
	 _delay_ms(1);
	 
	 // send least byte
	 
	 D4(GETBIT(cmd,0));
	 D5(GETBIT(cmd,1));
	 D6(GETBIT(cmd,2));
	 D7(GETBIT(cmd,3));
	 EN(1);
	 _delay_ms(1);
	 EN(0);
	 _delay_ms(1);
}

void LCD_write_char(uint8_t data){
	 RS(1);
	 // send most byte
	 D4(GETBIT(data,4));
	 D5(GETBIT(data,5));
	 D6(GETBIT(data,6));
	 D7(GETBIT(data,7));
	 EN(1);
	 _delay_ms(1);
	 EN(0);
	 _delay_ms(1);
	 
	 // send least byte
	 
	 D4(GETBIT(data,0));
	 D5(GETBIT(data,1));
	 D6(GETBIT(data,2));
	 D7(GETBIT(data,3));
	 EN(1);
	 _delay_ms(1);
	 EN(0);
	 _delay_ms(1);
}
void LCD_write_string(uint8_t txt[]){
	for (uint8_t i=0;txt[i]!='\0';i++){
		LCD_write_char(txt[i]);
	}
}
void LCD_write_num(uint32_t num){
uint8_t txt[10] = {0};
int8_t i = 0;
if(num == 0) {
	LCD_write_char('0');
	return;
}
for (i = 0; num != 0; i++ )
{
	txt[i] = num%10 + 48;
	num = num  / 10 ;
}
i--;
while(i >= 0){
	LCD_write_char(txt[i]);
	i--;
}
}