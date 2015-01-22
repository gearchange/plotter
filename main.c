#include <p30F4013.h>
#include <math.h>

_FOSC(CSW_FSCM_OFF & FRC_PLL16)
_FWDT(WDT_OFF)
_FBORPOR(PBOR_OFF & MCLR_EN)
_FGS(CODE_PROT_OFF)

#define SYS_FREQ	(29480000)
#define M_PI		(3.14159265358979323846264338327950288)

#define buzzer		LATDbits.LATD9
#define buttonA		(PORTAbits.RA11)
#define buttonB		(PORTCbits.RC15)

typedef struct{
	int left_deg;
	int right_deg;
} HAND_STATE;

const double L1 = 30.0;
const double L2 = 50.0;
const double L3 = 33.4 / 2.0;

const int delay_time = 200;
const int delay_dt = 20;

const int hand_up_deg = 20;
const int hand_down_deg = 6;

volatile int delay_counter = 0;

static inline long usec_to_PR(int t){
	return (double)(t) * 29.48 - 1;
}

static inline long deg_to_usec(int deg){
	return ((500.0 / 45.0) * (double)deg) + 1540;
}

void delay_ms(unsigned int msec){
	delay_counter = 0;
	while(delay_counter < msec);
}

static inline void delay_ms_routine(int time){
	delay_counter += time;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void){
	IFS0bits.T1IF = 0;
	delay_ms_routine(1);
}

void hand_up(void){
	for(int i = hand_down_deg; i <= hand_up_deg; i++){
		OC3RS = usec_to_PR(deg_to_usec(i)) / 64;
		delay_ms(delay_dt);
	}
}

void hand_down(void){
	for(int i = hand_up_deg; i >= hand_down_deg; i--){
		OC3RS = usec_to_PR(deg_to_usec(i)) / 64;
		delay_ms(delay_dt);
	}
}

void set_hand(HAND_STATE hand){
	OC1RS = usec_to_PR(deg_to_usec(hand.right_deg - 30)) / 64;
	OC2RS = usec_to_PR(deg_to_usec(-hand.left_deg + 30)) / 64;
}

static inline double square(double d){
	return d * d;
}

static inline int rad_to_deg(double rad){
	return (180.0 / M_PI) * rad;
}

static inline double deg_to_rad(int deg){
	return (M_PI / 180.0) * (double)deg;
}

void set_xy(double x, double y){
	y = 25.0 + y;

	double l_atan = atan2(y, x + L3);
	double r_atan = atan2(y, x - L3);
	double l_acos = acos((square(x + L3) + square(L1) - square(L2) + square(y)) / (2.0 * L1 * sqrt(square(y) + square(x + L3))));
	double r_acos = acos((square(x - L3) + square(L1) - square(L2) + square(y)) / (2.0 * L1 * sqrt(square(y) + square(x - L3))));

	set_hand((HAND_STATE){
		180 - rad_to_deg(l_acos + l_atan),
		-rad_to_deg(r_acos - r_atan)
	});
}

void drow_square(void){
	for(int i = 0; i >= -30; i--){
		set_xy((double)i, 0.0);
		delay_ms(delay_dt);
	}
	delay_ms(delay_time);
	hand_down();

	for(int i = -30; i <= 30; i++){
		set_xy((double)i, 0.0);
		delay_ms(delay_dt);
	}
	delay_ms(delay_time);

	for(int i = 0; i <= 20; i++){
		set_xy(30.0, (double)i);
		delay_ms(delay_dt);
	}
	delay_ms(delay_time);

	for(int i = 30; i >= -30; i--){
		set_xy((double)i, 20.0);
		delay_ms(delay_dt);
	}
	delay_ms(delay_time);

	for(int i = 20; i >= 0; i--){
		set_xy(-30.0, (double)i);
		delay_ms(delay_dt);
	}
	delay_ms(delay_time);
	hand_up();
	delay_ms(delay_time);
	set_xy(0.0, 0.0);
}

void drow_hougan(void){

	for(int j = 0; j < 27; j += 3){
		set_xy(-35, j);
		delay_ms(delay_time);
		hand_down();
		for(int i = -35; i <= 35; i++){
			set_xy((double)i, j);
			delay_ms(delay_dt);
		}
		delay_ms(delay_time);
		hand_up();
	}

	for(int j = -35; j < 35; j += 3){
		set_xy(j, 25);
		delay_ms(delay_time);
		hand_down();
		for(int i = 25; i >= 0; i--){
			set_xy(j, i);
			delay_ms(delay_dt);
		}
		delay_ms(delay_time);
		hand_up();
	}
	delay_ms(delay_time);
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
	OC3RS = usec_to_PR(deg_to_usec(hand_up_deg)) / 64;

	set_xy(0.0, 0.0);

}

int main(void){

	init_device();

	while(buttonA == buttonB);

	drow_hougan();

	while(1);

}

