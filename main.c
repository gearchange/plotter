#include <p30F4013.h>

_FOSC(CSW_FSCM_OFF & FRC_PLL16)
_FWDT(WDT_OFF)
_FBORPOR(PBOR_OFF & MCLR_EN)
_FGS(CODE_PROT_OFF)

#include "plotter.h"
#include "utility.h"
#include "gearcode.h"

#define buzzer	LATDbits.LATD9
#define buttonA	(PORTAbits.RA11)
#define buttonB	(PORTCbits.RC15)

const int x_max = 80;
const int y_max = 36;
const int dx = 3;
const int dy = 3;

void drow_grid(void){

	for(int y = 0; y <= y_max; y += dy){
		set_xy(0, y);
		delay_ms(delay_time_long);
		hand_down();
		for(int x = 0; x <= x_max; x += dx){
			set_xy(x, y);
			delay_ms(delay_time_short);
		}
		delay_ms(delay_time_long);
		hand_up();
	}

	for(int x = 0; x <= x_max; x += dx){
		set_xy(x, 0);
		delay_ms(delay_time_long);
		hand_down();
		for(int y = 0; y <= y_max; y += dy){
			set_xy(x, y);
			delay_ms(delay_time_short);
		}
		delay_ms(delay_time_long);
		hand_up();
	}

	delay_ms(delay_time_long);
	set_xy(0.0, 0.0);

}

void init_device(void){

	LATA = 0;
	LATB = 0;
	LATC = 0;
	LATD = 0;
	LATF = 0;

	TRISA = 0b0000100000000000;
	TRISB = 0;
	TRISC = 0b1000000000000000;
	TRISD = 0;
	TRISF = 0;

	ADPCFG = 0xffff;

	PR1 = usec_to_PR(1000);
	IEC0bits.T1IE = 1;
	IPC0bits.T1IP = 6;
	T1CONbits.TON = 1;

	PR2 = usec_to_PR(15000) / 64;
	T2CONbits.TCKPS = 0b10;
	T2CONbits.TON = 1;

	OC1CON = 0b110;
	OC2CON = 0b110;
	OC3CON = 0b110;

	set_home_position();

}

int main(void){

	init_device();

	while(buttonA == buttonB);

	drow_grid();

	char str[STRSIZE] = "POINT 				10 90";
	do_command(str);
	while(1);

}

