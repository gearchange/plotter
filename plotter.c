#include <p30F4013.h>
#include <math.h>

#include "plotter.h"
#include "utility.h"

#define M_PI	(3.14159265358979323846264338327950288)

const double limiting_angles = 90.0;
const double neutral_usec = 1540.0;
const double limiting_usec = 1000.0;

const double L1 = 30.0;
const double L2 = 50.0;
const double L3 = 33.4 / 2.0;

const double offset_y = 25.0;

const int delay_time = 200;
const int delay_dt = 20;

const int hand_up_deg = 20;
const int hand_down_deg = 6;

static inline int deg_to_usec(double deg){
	return ((limiting_usec / limiting_angles) * deg) +neutral_usec;
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

void set_hand(int left_deg, int right_deg){
	OC1RS = usec_to_PR(deg_to_usec(right_deg - 30)) / 64;
	OC2RS = usec_to_PR(deg_to_usec(-left_deg + 30)) / 64;
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

	y += offset_y;

	double l_atan = atan2(y, x + L3);
	double r_atan = atan2(y, x - L3);
	double l_acos = acos((square(x + L3) + square(L1) - square(L2) + square(y))
			/ (2.0 * L1 * sqrt(square(y) + square(x + L3))));
	double r_acos = acos((square(x - L3) + square(L1) - square(L2) + square(y))
			/ (2.0 * L1 * sqrt(square(y) + square(x - L3))));

	set_hand(180 - rad_to_deg(l_acos + l_atan), -rad_to_deg(r_acos - r_atan));

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
