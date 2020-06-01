/*
 * Timer0.c
 *
 * Created: 16/04/2020 10:11:10 AM
 *  Author: M
 */ 
#include "Timer0.h"

void timer_normal_init(){
	TCCR0= (1<<FOC0) | (1<<CS02) | (1<<CS00);
	SETBIT(TIMSK,TOIE0);
	sei();
}
void timer_compare_init(){
	TCCR0= (1<<FOC0) | (1<<CS02) | (1<<CS00) | (1<<WGM01);
	OCR0 =200;
	SETBIT(TIMSK,OCIE0);
	sei();
}