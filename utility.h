#ifndef UTILITY_H
#define	UTILITY_H

#define SYS_FREQ	(29480000)

static inline long usec_to_PR(int t){
	return (double)(t) * 29.48 - 1;
}

void delay_ms(unsigned int msec);

#endif	/* UTILITY_H */

