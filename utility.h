#ifndef UTILITY_H
#define	UTILITY_H

#define SYS_FREQ	(29480000)

extern const int delay_time_long;
extern const int delay_time_short;

static inline long usec_to_PR(int t){
	return (double)(t) * 29.48 - 1;
}

void delay_ms(unsigned int msec);


#endif	/* UTILITY_H */

