/*
 * Smart_home_master.c
 *
 * Created: 29/05/2020 5:23:27 AM
 *  Author: Mohamed Elsayed
 */ 
/**************************************  Timer Tick  ****************************************************/
// looks like Watchdog timer reset application and ask for the password every tick 
#define tick 245
/*
tick 31  --->> 0.5 sec  
tick 62  --->> 1   sec
tick 93  --->> 1.5 sec
tick 124 --->> 2   sec
	 |		   |
	 |		   |
	 V		   V
tick 245 --->> 4   sec  max value 
*/
/***************************************  Block Mode Timer  *********************************************/
#define block_timer 2000 
 /*1000 ---> 1 second*/
/********************************************************************************************************/
#include "LCD.h"
#include "keypad.h"
#include "EEPROM.h"
#include "Timer0.h"
#include "SPI.h"
void admin_display();					 // Lcd on Admin mode first page
void admin_display2();					 // Lcd on Admin mode second page
void guest_display();					 // Lcd on Guest mode 
void get_password ();					 // Main function --> get password and state(Admin or Guest) and select what you want to control
void ADMIN();							 // Open Admin features 
void GUEST();							 // Open Guest features
uint8_t button_num=0;					 // Store your selection (room1 or 2 or tv ...) to be sent to slave Micro-controller
uint8_t block_mode;						 // If you forget password block_mode value will be updated 
uint8_t start_msg=0;					 // It must be 0 at the begining to display instructions
uint8_t count=0;						 // That let you move betwwen cases of get_password function
uint8_t initial=0;						 // In your first open this value will be important to set your password for the first time 
uint8_t disp_falg=0;					 // This flag is to select which display function to use
uint8_t Set_admin_pass=0;				 // This flag will let you set Admin password one time in the begining 
uint8_t Set_guest_pass=0;				 // This flag will let you set Guest password one time in the begining
uint8_t state=0;						 // Admin(0) or guest(1)
uint8_t a1,a2,a3,a4;					 // right password digits --> admin
uint8_t g1,g2,g3,g4;					 //right password digits	--> guest
uint8_t c1,c2,c3,c4;					 // password digits entered by user
int main(void){
	Set_admin_pass=EEPROM_read(24);		 // Must be 255 at the begining 
	Set_guest_pass=EEPROM_read(29);		 //	Must be 255 at the begining
	a1=EEPROM_read(20);					 // first digit of admin mode
	a2=EEPROM_read(21);					 // second digit of admin mode
	a3=EEPROM_read(22);					 // third digit of admin mode
	a4=EEPROM_read(23);	                 // forth digit of admin mode
	g1=EEPROM_read(25);					 // first digit of guest mode
	g2=EEPROM_read(26);					 // second digit of guest mode
	g3=EEPROM_read(27);				     // third digit of guest mode
	g4=EEPROM_read(28);					 // forth digit of guest mode
	SETBIT(DDRC,0);						 // LED FOR ADMIN MODE
	SETBIT(DDRC,2);						 // LED FOR GUEST MODE
	SETBIT(DDRC,4);						 // LED FOR BLOCK MODE
	LCD_init();							 // Initializing LCD	
	keypad_init();						 // Initializing Keypad	
	SPI_master_init();					 // Initializing SPI	
	timer_normal_init();				 // Initializing Timer0 in normal mode	
	block_mode=EEPROM_read(30);			 // To check if you were in block mode when you restarted your application 
	if(Set_admin_pass==15 && Set_guest_pass==255 ){initial=2;}	//This will let you set Guest password if you didn't with Admin password
	while(1)
    {
		
		if(block_mode==3){										//This will block you 20 sec if you are in block mode
		SETBIT(PORTC,4);
		LCD_write_command(1);
		LCD_write_string("you are blocked");
		LCD_write_command(0xc0);
		LCD_write_string("for 20 sec");
		_delay_ms(block_timer);
		CLRBIT(PORTC,4);
		LCD_write_command(1);
		LCD_write_string("Enter right pass");
		block_mode=0;
		EEPROM_write(30,0);
		}
		
		
		if (Set_admin_pass==255	&& initial==0 )	 				//To set first value of Admin password
		{
			
			LCD_write_command(1);
			LCD_write_string("Welcome to Smart");
			LCD_write_command(0xc0);
			LCD_write_string("Home");
			_delay_ms(500);
			LCD_write_command(1);
			LCD_write_string("Ur first log in");
			_delay_ms(300);
			LCD_write_command(1);
			LCD_write_string("Enter Admin Pass");
			initial=1;
			count=1;
			state=0;
			start_msg=1;
			Set_admin_pass=15;
			_delay_ms(200);
		}
		if (Set_guest_pass==255 && initial==2 )				//To set first value of guest password
		{
			initial=3;
			LCD_write_command(1);
			LCD_write_string("Enter Guest pass");
			count=1;
			state=1;			
			start_msg=1;
			_delay_ms(200);
		}
		if(start_msg==0){									//Start message to user before writing password
		LCD_write_command(1);
		LCD_write_string("select Mode: ");
		LCD_write_command(0xc0);
		LCD_write_string("0>Admin 1>Guest");
		start_msg++;
		CLRBIT(PORTB,4);
		SPI_send_char('0');
		SETBIT(PORTB,4);
		}
		get_password();
		
	}
}

void get_password ()
{
	int8_t key=keypad_read();
	if(key != -1 && key!='N'){
		switch (count)
		{
			case 0 :											//Get state (Admin or Guest) 
			if (key-48==0)
			{
				state=0;
				count++;
				LCD_write_command(1);
				LCD_write_string("Admin password");
				
			}
			else if (key-48==1)
			{
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
				LCD_write_string("Select Mode : ");
				LCD_write_command(0xc0);
				LCD_write_string("0>Admin  1>Guest");
			}
			_delay_ms(200);
			break;
			case 1 :													//Get first Digit
			LCD_write_command(0xc3);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc3);
			LCD_write_char('*');
			c1=key-48;													//First digit of entered password from user
			count++;
			break;
			case 2 :													//Get second Digit
			LCD_write_command(0xc4);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc4);
			LCD_write_char('*');
			c2=key-48;													//Second digit of entered password from user
			count++;
			break;
			case 3 :													//Get third Digit
			LCD_write_command(0xc5);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc5);
			LCD_write_char('*');
			c3=key-48;													// third digit of entered password from user
			count++;
			break;
			case 4 :													//Get forth Digit
			LCD_write_command(0xc6);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc6);
			LCD_write_char('*');
			LCD_write_command(1);
			c4=key-48;												   // forth digit of entered password from user
			
			if (( c1==a1&&c2==a2&&c3==a3&&c4==a4) && state==0 && initial==0){
				/*call function to enter admin mode */
				key=-1;
				ADMIN();
				block_mode=0;
				EEPROM_write(30,0);
				key=-1;
			}
			else if (( c1==g1&&c2==g2&&c3==g3&&c4==g4)&& state==1 && initial==0 ){
				/*call function to enter guest mode */
				key=-1;
				GUEST();
				block_mode=0;
				EEPROM_write(30,0);
				key=-1;
			}
			else if ( initial==1 && state==0 ){ 
				/*Store Admin Password in EEPROM*/
				EEPROM_write(20,c1);
				EEPROM_write(21,c2);
				EEPROM_write(22,c3);
				EEPROM_write(23,c4);
				EEPROM_write(24,15);
				initial++;
				
			}
			else if (initial==3 && state==1 ){		
				/*Store Guest Password in EEPROM*/
				EEPROM_write(25,c1);
				EEPROM_write(26,c2);
				EEPROM_write(27,c3);
				EEPROM_write(28,c4);
				EEPROM_write(29,15);
				initial++;
				LCD_write_command(1);
				a1=EEPROM_read(20);					 // first digit of admin mode
				a2=EEPROM_read(21);					 // second digit of admin mode
				a3=EEPROM_read(22);					 // third digit of admin mode
				a4=EEPROM_read(23);	                 // forth digit of admin mode
				g1=EEPROM_read(25);					 // first digit of guest mode
				g2=EEPROM_read(26);					 // second digit of guest mode
				g3=EEPROM_read(27);				     // third digit of guest mode
				g4=EEPROM_read(28);					 // forth digit of guest mode
				start_msg=0;
				initial=0;
				count=0;
			}
			
			else {
				/*Check wrong tries and select going to block mode or not*/
				block_mode++;
				LCD_write_command(1);
				LCD_write_string("Wrong password");
				_delay_ms(150);
				LCD_write_command(1);
				LCD_write_num(3-block_mode);
				LCD_write_string(" Tries left");
				EEPROM_write(30,block_mode);
				_delay_ms(100);
				count=1;
				if(block_mode==3){
				SETBIT(PORTC,4);
				LCD_write_command(1);
				LCD_write_string("you are blocked");
				LCD_write_command(0xc0);
				LCD_write_string("for 20 sec");
				_delay_ms(block_timer);
				CLRBIT(PORTC,4);
				LCD_write_command(1);
				LCD_write_string("Enter right pass");
				block_mode=0;
				EEPROM_write(30,0);
				}
				
			}
			break;
			case 5:										//To get selection of the room you want to control
			switch (key){
				case '1':								//ROOM1
				if(disp_falg==0){
				LCD_write_command(1);
				LCD_write_string("Room 1");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				button_num=key;
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				key=-1;
				count=6;
				}
				//else{key=-1;}				
				break;
				case '2':								//ROOM2
				if(disp_falg==0){
				LCD_write_command(1);
				LCD_write_string("Room 2");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				button_num=key;
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				key=-1;
				count=6;
				}				
				break;
				case '3':								//ROOM3
				if(disp_falg==0){
				LCD_write_command(1);
				LCD_write_string("Room 3");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				button_num=key;
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				key=-1;
				count=6;
				}				
				break;
				case '4':								//ROOM4
				button_num=key;
				if (state==1){
				LCD_write_command(1);
				LCD_write_string("Room 4");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				key=-1;
				count=6;
				}
				else if(state==0 && disp_falg==0){
					key=-1;
					admin_display2();
					disp_falg=1;}
				else if (state==0 && disp_falg==1){
					LCD_write_command(1);
					LCD_write_string("Room 4");
					LCD_write_command(0xc0);
					LCD_write_string("1>on 2>off 3>back");
					CLRBIT(PORTB,4);
					SPI_send_char(key);
					SETBIT(PORTB,4);
					key=-1;
					count=6;
					disp_falg=0;
				}		
				else {key=-1;}
				break;
				case '5':											//TV
				button_num=key;
				if (state==0 && disp_falg==1){
				LCD_write_command(1);
				LCD_write_string("TV");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				disp_falg=0;
				key=-1;
				count=6;
				}
				else if(state==1) {
					LCD_write_command(1);
					guest_display();
				}
				break;
				case '6':										//Air_COND
				button_num=key;
				if(state==0 && disp_falg==1){
				LCD_write_command(1);
				LCD_write_string("COND");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				disp_falg=0;
				key=-1;
				count=6;
				}				
				else if(state==1) {
					LCD_write_command(1);
					guest_display();
				}
				break;
				case '7':									//Display first page
				button_num=key;
				if(state==0&&disp_falg==1){
					admin_display();
					disp_falg=0;
					}
				else{key=-1;}
				break;
				case -1:
				break;
				default:
				if(state==0){admin_display();}
				else if(state==1){guest_display();}	
				disp_falg=0;
				break;
			}
			break;
			case 6:													//To select action on the room yo selected in case 5
			switch (key){
				case '1':											//Status ON 
				LCD_write_command(1);
				LCD_write_string("status:   on ");
				LCD_write_command(0xc0);
				LCD_write_string("2>off  3>back");			 
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				_delay_ms(100);
				CLRBIT(PORTB,4);
				SPI_send_char(button_num);
				SETBIT(PORTB,4);
				key=-1;
				break;
				case '2':											//Status OFF
				LCD_write_command(1);
				LCD_write_string("status:  off ");
				LCD_write_command(0xc0);
				LCD_write_string("1>on   3>back");
				CLRBIT(PORTB,4);
				SPI_send_char(key);
				SETBIT(PORTB,4);
				_delay_ms(100);
				CLRBIT(PORTB,4);
				SPI_send_char(button_num);
				SETBIT(PORTB,4);
				key=-1;
				break;
				case '3':											//Return to first Display page
				CLRBIT(PORTB,4);
				SPI_send_char('0');
				SETBIT(PORTB,4);
				if(state==0){
					admin_display();
					key=-1;
					count=5;
				}
				else if(state==1){
					guest_display();
					key=-1;
					count=5;
				}
				break;
				default:
				LCD_write_command(1);
				LCD_write_string("wrong entry");
				LCD_write_command(0xc0);
				LCD_write_string("1>on 2>off 3>back");
				break;
			}
			break;				
		
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
	if (admin_mode==tick && state==0)			// 1.5sec
	{
		
		TOGBIT(PORTC,0);
		admin_mode=0;
		guest_mode=0;
		count=0;
		start_msg=0;
		CLRBIT(TIMSK,TOIE0);
	}
	else if (guest_mode==tick && state==1)		// 1.5sec
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