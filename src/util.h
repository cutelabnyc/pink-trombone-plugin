//
//  util.h
//  PinkTrombone - All
//
//  Created by Samuel Tarakajian on 8/28/19.
//

#ifndef util_h
#define util_h

static double maxf(double a, double b) {
	if (a > b) return a;
	return b;
}

static double minf(double a, double b) {
	if (a < b) return a;
	return b;
}

static inline double clamp(double number, double min, double max) {
	if (number < min) return min;
	else if (number > max) return max;
	else return number;
}

static inline double moveTowards(double current, double target, double amount)
{
	if (current < target) return minf(current + amount, target);
	else return maxf(current - amount, target);
}

static inline double moveTowards(double current, double target, double amountUp, double amountDown)
{
	if (current < target) return minf(current + amountUp, target);
	else return maxf(current - amountDown, target);
}

static inline double gaussian()
{
	double s = 0;
	for (int c = 0; c < 16; c++) s += (double) rand() / (double) RAND_MAX;
	return (s - 8.0) / 4.0;
}

#endif /* util_h */
