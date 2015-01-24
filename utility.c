#include <p30F4013.h>

const int delay_time_long = 200;
const int delay_time_short = 25;

volatile int delay_counter = 0;

static inline void delay_ms_routine(void){
	delay_counter++;
}

void delay_ms(unsigned int msec){
	delay_counter = 0;
	while(delay_counter < msec);
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void){
	IFS0bits.T1IF = 0;
	delay_ms_routine();
}

