#include <p30F4013.h>
#include <math.h>

#include "plotter.h"
#include "utility.h"

#define M_PI	(3.14159265358979323846264338327950288)

const double limiting_angles = 90.0;
const double neutral_usec = 1540.0;
const double limiting_usec = 1000.0;

const double L1 = 35.0;
const double L2 = 50.0;
const double L3 = 33.4 / 2.0;

const double offset_x = -40.0;
const double offset_y = 21.0;

const int hand_up_deg = 10;
const int hand_down_deg = 3;

static inline int deg_to_usec(int deg){
	return ((limiting_usec / limiting_angles) * (double)deg) +neutral_usec;
}

static inline int deg_to_servo_PR(int deg){
	return usec_to_PR(deg_to_usec(deg)) / 64;
}

void hand_up(void){
	for(int i = hand_down_deg; i <= hand_up_deg; i++){
		OC3RS = deg_to_servo_PR(i);
		delay_ms(delay_time_short);
	}
}

void hand_down(void){
	for(int i = hand_up_deg; i >= hand_down_deg; i--){
		OC3RS = deg_to_servo_PR(i);
		delay_ms(delay_time_short);
	}
}

void set_hand(int left_deg, int right_deg){
	OC1RS = deg_to_servo_PR(right_deg - 30);
	OC2RS = deg_to_servo_PR(-left_deg + 30);
}

static inline int rad_to_deg(double rad){
	return (180.0 / M_PI) * rad;
}

void set_xy(double x, double y){

	x += offset_x;
	y += offset_y;

	double l_atan = atan2(y, x + L3);
	double r_atan = atan2(y, x - L3);
	double l_acos = acos((pow(x + L3, 2) + pow(L1, 2) - pow(L2, 2) + pow(y, 2))
			/ (2.0 * L1 * sqrt(pow(y, 2) + pow(x + L3, 2))));
	double r_acos = acos((pow(x - L3, 2) + pow(L1, 2) - pow(L2, 2) + pow(y, 2))
			/ (2.0 * L1 * sqrt(pow(y, 2) + pow(x - L3, 2))));

	set_hand(180 - rad_to_deg(l_acos + l_atan), -rad_to_deg(r_acos - r_atan));

}

void set_home_position(void){
	OC3RS = usec_to_PR(deg_to_usec(hand_up_deg)) / 64;
	set_xy(0, 0);
}
