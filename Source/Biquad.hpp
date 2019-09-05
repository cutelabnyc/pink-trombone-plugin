//
//  Biquad.hpp
//  PinkTrombone - All
//
//  Created by Samuel Tarakajian on 8/30/19.
//

#ifndef Biquad_hpp
#define Biquad_hpp

#include <stdio.h>

class Biquad {
public:
	Biquad(double sampleRate);
	~Biquad();
	void setFrequency(double f);
	void setQ(double f);
	void setGain(double g);
	double runStep(double xn);
private:
	void updateCoefficients();
	
	double frequency, q, gain;
	double a0, a1, a2, b1, b2;
	double xm1, xm2, ym1, ym2;
	double sampleRate, twopiOverSampleRate;
};

#endif /* Biquad_hpp */
