/*
 * Smart_home_master.c
 *
 * Created: 29/05/2020 5:23:27 AM
 *  Author: M
 */ 

#include "LCD.h"
#include "keypad.h"
#include "EEPROM.h"
#include "Timer0.h"
#include "SPI.h"
void admin_display();
void admin_display2();
void guest_display();
void get_password ();
void ADMIN();
void GUEST();
uint8_t block_mode;
uint8_t start_msg=0;
uint8_t count=0;
uint8_t initial=0;
uint8_t disp_falg=0;
uint8_t Set_admin_pass=0;
uint8_t Set_guest_pass=0;
uint8_t state=0; // Admin(0) or guest(1)
uint8_t a1=1,a2=2,a3=3,a4=4; // right password digits --> admin
uint8_t g1,g2,g3,g4;		//right password digits	--> guest
uint8_t c1,c2,c3,c4; // password digits entered by user
int main(void){
	Set_admin_pass=EEPROM_read(24);
	Set_guest_pass=EEPROM_read(29);
	a1=EEPROM_read(20); // first digit of admin mode
	a2=EEPROM_read(21);	// second digit of admin mode
	a3=EEPROM_read(22);	// third digit of admin mode
	a4=EEPROM_read(23);	// forth digit of admin mode
	g1=EEPROM_read(25);	// first digit of guest mode
	g2=EEPROM_read(26);	// second digit of guest mode
	g3=EEPROM_read(27);	// third digit of guest mode
	g4=EEPROM_read(28);	// forth digit of guest mode
	SETBIT(DDRC,0);		// LED FOR ADMIN MODE
	SETBIT(DDRC,2);		// LED FOR GUEST MODE
	SETBIT(DDRC,4);		// LED FOR BLOCK MODE
//	sei();
//	int8_t key;
	LCD_init();
	keypad_init();
	SPI_master_init();
	timer_normal_init();
//    SETBIT(PORTC,4);
//	LCD_write_string("enter password");
	block_mode=EEPROM_read(30);
	while(1)
    {
		
		if(block_mode==3){
		SETBIT(PORTC,4);
		LCD_write_command(1);
		LCD_write_string("you are blocked");
		LCD_write_command(0xc0);
		LCD_write_string("for 20 sec");
		_delay_ms(2000);
		CLRBIT(PORTC,4);
		LCD_write_command(1);
		LCD_write_string("Enter right pass");
		block_mode=0;

		}
		
		
		if (Set_admin_pass==255	&& initial==0 )		// to set first value of admin password
		{
			LCD_write_command(1);
			LCD_write_string("Enter new admin");
			initial=1;
			count=1;
			state=0;
			start_msg=1;
			_delay_ms(200);
		}
		if (Set_guest_pass==255 && initial==2)		// to set first value of guest password
		{
			initial=3;
			LCD_write_command(1);
			LCD_write_string("Enter new guest");
			count=1;
			state=1;			
			start_msg=1;
			_delay_ms(200);
		}
		if(start_msg==0){						// start message to user before writing password
		LCD_write_command(1);
		LCD_write_string("0>admin&1>guest ");
		_delay_ms(500);
		start_msg++;
		}
		get_password();
				
				
		/*
		key=keypad_read();
		if (key!=-1)
		{
		LCD_write_command(0x1);
		LCD_write_char(key);
		CLRBIT(PORTB,4);
		SPI_send_char(key);
		SETBIT(PORTB,4);
		sei();				// test
		_delay_ms(100);
		}*/
	}
}

void get_password ()
{
	int8_t key=keypad_read();
	if(key != -1 && key!='N'){
		switch (count)
		{
			case 0 :
			if (key-48==0)
			{
				block_mode=0;
				EEPROM_write(30,0);
				state=0;
				count++;
				LCD_write_command(1);
				LCD_write_string("Admin password");
				
			}
			else if (key-48==1)
			{
				block_mode=0;
				EEPROM_write(30,0);
				state=1;
				count++;
				LCD_write_command(1);
				LCD_write_string("Guest password");
				
			}
			else {
				LCD_write_command(1);
				LCD_write_string("wrong entry");
				_delay_ms(200);
				LCD_write_command(1);
				LCD_write_string("0>admin&1>guest ");
			}
			_delay_ms(200);
			break;
			case 1 :
			LCD_write_command(0xc3);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc3);
			LCD_write_char('*');
			c1=key-48;   // first digit of entered password from user
			count++;
			break;
			case 2 :
			LCD_write_command(0xc4);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc4);
			LCD_write_char('*');
			c2=key-48;  // second digit of entered password from user
			count++;
			break;
			case 3 :
			LCD_write_command(0xc5);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc5);
			LCD_write_char('*');
			c3=key-48;   // third digit of entered password from user
			count++;
			break;
			case 4 :
			LCD_write_command(0xc6);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc6);
			LCD_write_char('*');
			LCD_write_command(1);
			c4=key-48;  // forth digit of entered password from user
			
			if (( c1==a1&&c2==a2&&c3==a3&&c4==a4) && state==0 && initial==0){
				/*call function to enter admin mode */
				key=-1;
				ADMIN();
			}
			else if (( c1==g1&&c2==g2&&c3==g3&&c4==g4)&& state==1 && initial==0 ){
				/*call function to enter guest mode */
				key=-1;
				GUEST();
			}
			else if (initial==1 && state==0 ){
				EEPROM_write(20,c1);
				EEPROM_write(21,c2);
				EEPROM_write(22,c3);
				EEPROM_write(23,c4);
				EEPROM_write(24,15);
				initial++;
				
			}
			else if (initial==3&& state==1 ){
				EEPROM_write(25,c1);
				EEPROM_write(26,c2);
				EEPROM_write(27,c3);
				EEPROM_write(28,c4);
				EEPROM_write(29,15);
				initial++;
				start_msg=0;
			}
			
			else {
				block_mode++;
				LCD_write_command(1);
				LCD_write_string("Wrong password");
				_delay_ms(300);
				LCD_write_command(1);
				LCD_write_num(3-block_mode);
				LCD_write_string(" Tries left");
				_delay_ms(100);
				EEPROM_write(30,block_mode);				
				count=1;
				if(block_mode==3){
				SETBIT(PORTC,4);
				LCD_write_command(1);
				LCD_write_string("you are blocked");
				LCD_write_command(0xc0);
				LCD_write_string("for 20 sec");
				_delay_ms(2000);
				CLRBIT(PORTC,4);
				LCD_write_command(1);
				LCD_write_string("Enter right pass");
				block_mode=0;
				EEPROM_write(30,0);
				}
			}
/*			case 5:
			switch (key){
				case '1':
				LCD_write_command(1);
				LCD_write_string("Room 1");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				key=-1;
				count=6;
				break;
				case '2':
				LCD_write_command(1);
				LCD_write_string("Room 2");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				key=-1;
				count=6;
				break;
				case '3':
				LCD_write_command(1);
				LCD_write_string("Room 3");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				key=-1;
				count=6;
				break;
				case '4':
				if (state==1){
				LCD_write_command(1);
				LCD_write_string("Room 4");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				key=-1;
				count=6;
				}
				else if(state==0){
					key=-1;
					admin_display2();
					disp_falg=1;}
				else if (state==0 && disp_falg==1){
					LCD_write_command(1);
					LCD_write_string("Room 4");
					LCD_write_command(0xc0);
					LCD_write_string("1>on 2>off 3>back");
					key=-1;
					count=6;
					disp_falg=0;
				}		
				
				break;
				case '5':
				if (state==0){
				LCD_write_command(1);
				LCD_write_string("TV");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				key=-1;
				count=6;
				}
				else if(state==1) {
					LCD_write_command(1);
					LCD_write_string("wrong entry");
					_delay_ms(100);
					LCD_write_command(1);
					guest_display();
				}
				break;
				case '6':
				if(state==0){
				LCD_write_command(1);
				LCD_write_string("COND");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				key=-1;
				count=6;
				}				
				else if(state==1) {
					LCD_write_command(1);
					LCD_write_string("wrong entry");
					_delay_ms(100);
					LCD_write_command(1);
					guest_display();
				}
				break;
				case '7':
				admin_display();
				break;
				case -1:
				break;
				default:
				LCD_write_command(1);
				LCD_write_string("wrong entry");
				_delay_ms(100);
				LCD_write_command(1);
				admin_display();

				break;
			}
			break;
			case 6:
			switch (key){
				case '1':
				LCD_write_command(1);
				LCD_write_string("Room 1");
				LCD_write_command(0xc0);
				LCD_write_string("on");				// change this to spi send 
				
				break;
				case '2':
				LCD_write_command(1);
				LCD_write_string("Room 1");		// change this to spi send 	
				LCD_write_command(0xc0);
				LCD_write_string("off");
				break;
				case '3':
				if(state==0){
					admin_display();
					count=5;
				}
				else if(state==1){
					guest_display();
					count=5;
				}
				break;
				default:
				LCD_write_command(1);
				LCD_write_string("wrong entry");
				break;
			}
			break;				
		*/
		}
	
	}
}
void ADMIN(){
	SETBIT(PORTC,0);
	SETBIT(TIMSK,TOIE0);
	admin_display();
	count=5;
	
}
void GUEST(){
	SETBIT(PORTC,2);
	SETBIT(TIMSK,TOIE0);
	guest_display();
	count=5;	

}
ISR(TIMER0_OVF_vect){
	
	static uint8_t admin_mode = 0;
	static uint8_t guest_mode = 0;
	admin_mode++;
	guest_mode++;
	if (admin_mode==255 && state==0)			// 1.5sec
	{
		
		TOGBIT(PORTC,0);
		admin_mode=0;
		guest_mode=0;
		count=0;
		start_msg=0;
		CLRBIT(TIMSK,TOIE0);
	}
	else if (guest_mode==255 && state==1)	// 1.5sec
	{
		
		CLRBIT(PORTC,2);
		admin_mode=0;
		guest_mode=0;
		count=0;
		start_msg=0;
		CLRBIT(TIMSK,TOIE0);
	}
}
void admin_display(){
	LCD_write_command(1);
	LCD_write_string("1>Room1");
	LCD_write_command(0x88);
	LCD_write_string("2>Room2");
	LCD_write_command(0xc0);
	LCD_write_string("3>Room3");
	LCD_write_command(0xc8);
	LCD_write_string("4>More");
}
void admin_display2(){
	LCD_write_command(1);
	LCD_write_string("4>Room4");
	LCD_write_command(0x88);
	LCD_write_string("5>TV");
	LCD_write_command(0xc0);
	LCD_write_string("6>COND");
	LCD_write_command(0xc8);
	LCD_write_string("7>back");
}
void guest_display(){
	LCD_write_command(1);
	LCD_write_string("1>Room1");
	LCD_write_command(0x88);
	LCD_write_string("2>Room2");
	LCD_write_command(0xc0);
	LCD_write_string("3>Room3");
	LCD_write_command(0xc8);
	LCD_write_string("4>Room4");
}		