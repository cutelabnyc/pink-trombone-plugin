//
//  Biquad.cpp
//  PinkTrombone - All
//
//  Created by Samuel Tarakajian on 8/30/19.
//

#include "Biquad.hpp"
#include <math.h>

Biquad::Biquad(double sampleRate) :
	frequency(200),
	q(0.5),
	gain(1.0),
	xm1(0.0),
	xm2(0.0),
	ym1(0.0),
	ym2(0.0)
{
	this->sampleRate = sampleRate;
	this->twopiOverSampleRate = M_PI * 2.0 / this->sampleRate;
	this->updateCoefficients();
}

void Biquad::updateCoefficients() {
	double omega = this->frequency * this->twopiOverSampleRate;
	double sn = sin(omega);
	double cs = cos(omega);
	double one_over_Q = 1./this->q;
	double alpha = sn * 0.5 * one_over_Q;
	
	// Bandpass only, for now
	double b0 = 1./(1. + alpha);
	this->a0 = alpha * b0;
	this->a1 = 0.;
	this->a2 = -alpha * b0;
	this->b1 = -2. * cs * b0;
	this->b2 = (1. - alpha) * b0;
}

void Biquad::setFrequency(double f) {
	this->frequency = f;
	this->updateCoefficients();
}

void Biquad::setGain(double g) {
	this->gain = g;
	this->updateCoefficients();
}

void Biquad::setQ(double q) {
	this->q = q;
	this->updateCoefficients();
}

double Biquad::runStep(double xn) {
	double yn = (this->a0 * xn) + (this->a1 * this->xm1) + (this->a2 * this->xm2) - (this->b1 * this->ym1) - (this->b2 * this->ym2);
	this->ym2 = this->ym1;
	this->ym1 = yn;
	this->xm2 = this->xm1;
	this->xm1 = xn;
	return yn;
}
