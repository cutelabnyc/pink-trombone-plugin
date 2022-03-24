//
//  Glottis.cpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 8/28/19.
//

#include "Glottis.hpp"
#include <math.h>
#include "noise.hpp"
#include "util.h"

Glottis::Glottis(double sampleRate) :
	timeInWaveform(0),
//	oldFrequency(140),
//	newFrequency(140),
//	smoothFrequency(140),
//	UIFrequency(140),
	oldTenseness(0.6),
	newTenseness(0.6),
	UITenseness(0.6),
	totalTime(0.0),
	intensity(0),
	loudness(1),
	vibratoAmount(0.005),
	vibratoFrequency(6),
	autoWobble(false),
	isTouched(false),
	alwaysVoice(false)
{
	this->sampleRate = sampleRate;
	this->setupWaveform(0);
}

Glottis::~Glottis() { }

void Glottis::setupWaveform(double lambda)
{
	//this->frequency = this->oldFrequency * (1 - lambda) + this->newFrequency * lambda;
	double tenseness = this->oldTenseness * (1 - lambda) + this->newTenseness * lambda;
	this->Rd = 3 * (1 - tenseness);
	this->waveformLength = 1.0 / this->frequency;
	
	double Rd = this->Rd;
	if (Rd < 0.5) Rd = 0.5;
	if (Rd > 2.7) Rd = 2.7;
	// normalized to time = 1, Ee = 1
	double Ra = -0.01 + 0.048 * Rd;
	double Rk = 0.224 + 0.118 * Rd;
	double Rg = (Rk / 4) * (0.5 + 1.2 * Rk) / (0.11 * Rd - Ra * (0.5 + 1.2 * Rk));
	
	double Ta = Ra;
	double Tp = 1 / (2.0 * Rg);
	double Te = Tp + Tp * Rk;
	
	double epsilon = 1 / Ta;
	double shift = exp(-epsilon * (1 - Te));
	double Delta = 1 - shift; //divide by this to scale RHS
	
	double RHSIntegral = (1 / epsilon) * (shift - 1) + (1 - Te) * shift;
	RHSIntegral = RHSIntegral / Delta;
	
	double totalLowerIntegral = - (Te-Tp) / 2.0 + RHSIntegral;
	double totalUpperIntegral = -totalLowerIntegral;
	
	double omega = M_PI / Tp;
	double s = sin(omega * Te);
	// need E0*e^(alpha*Te)*s = -1 (to meet the return at -1)
	// and E0*e^(alpha*Tp/2) * Tp*2/pi = totalUpperIntegral
	//             (our approximation of the integral up to Tp)
	// writing x for e^alpha,
	// have E0*x^Te*s = -1 and E0 * x^(Tp/2) * Tp*2/pi = totalUpperIntegral
	// dividing the second by the first,
	// letting y = x^(Tp/2 - Te),
	// y * Tp*2 / (pi*s) = -totalUpperIntegral;
	double y = -M_PI * s * totalUpperIntegral / (Tp * 2.0);
	double z = log(y);
	double alpha = z / (Tp / 2.0 - Te);
	double E0 = -1.0 / (s * exp(alpha*Te));
	this->alpha = alpha;
	this->E0 = E0;
	this->epsilon = epsilon;
	this->shift = shift;
	this->Delta = Delta;
	this->Te = Te;
	this->omega = omega;
}

double Glottis::getNoiseModulator()
{
	double voiced = 0.1 + 0.2 * fmax(0.0, sin(M_PI * 2 * this->timeInWaveform / this->waveformLength));
	//return 0.3;
	return this->UITenseness * this->intensity * voiced + (1 - this->UITenseness * this->intensity) * 0.3;
}

void Glottis::finishBlock()
{
	//  calculateFrequencyFluctuations() can be called here
	this->oldTenseness = this->newTenseness;
	this->newTenseness = this->UITenseness +
		0.1 * simplex1(this->totalTime * 0.46) + 0.05 * simplex1(this->totalTime * 0.36);
	if (!this->isTouched && alwaysVoice) this->newTenseness += (3-this->UITenseness)*(1-this->intensity);
    if (this->isTouched || alwaysVoice) this->intensity += 0.26; //0.13; make the intensity jump a bit faster
	else this->intensity -= 0.05;
	this->intensity = clamp(this->intensity, 0.0, 1.0);
}

double Glottis::normalizedLFWaveform(double t)
{
	double output;
	if (t > this->Te) output = (-exp(-this->epsilon * (t - this->Te)) + this->shift) / this->Delta;
	else output = this->E0 * exp(this->alpha * t) * sin(this->omega * t);
	
	return output * this->intensity * this->loudness;
}

double Glottis::runStep(double lambda, double noiseSource)
{
	double timeStep = 1.0 / this->sampleRate;
	
	this->waveformLength = 1.0 / this->frequency;
	this->timeInWaveform += timeStep;
	this->totalTime += timeStep;
	
	if (this->timeInWaveform > this->waveformLength)
	{
		this->timeInWaveform -= this->waveformLength;
		this->setupWaveform(lambda);
	}

	double out = this->normalizedLFWaveform(this->timeInWaveform / this->waveformLength);
	double aspiration = this->intensity * (1 - sqrt(this->UITenseness)) * this->getNoiseModulator() * noiseSource;
	aspiration *= 0.2 + 0.02 * simplex1(this->totalTime * 1.99);
	out += aspiration;
	return out;
}

void Glottis::calculateFrequencyFluctuations()
{
	double vibrato = 0;
	vibrato += this->vibratoAmount * sin(2 * M_PI * this->totalTime * this->vibratoFrequency);
	vibrato += 0.02 * simplex1(this->totalTime * 4.07);
	vibrato += 0.04 * simplex1(this->totalTime * 2.15);
	if (this->autoWobble)
	{
		vibrato += 0.2 * simplex1(this->totalTime * 0.98);
		vibrato += 0.4 * simplex1(this->totalTime * 0.5);
	}
	if (this->UIFrequency > this->smoothFrequency)
		this->smoothFrequency = fmin(this->smoothFrequency * 1.1, this->UIFrequency);
	if (this->UIFrequency < this->smoothFrequency)
		this->smoothFrequency = fmax(this->smoothFrequency / 1.1, this->UIFrequency);
	this->oldFrequency = this->newFrequency;
	this->newFrequency = this->smoothFrequency * (1 + vibrato);
}

void Glottis::setFrequency(double midiNoteInHz)
{
	this->frequency = midiNoteInHz;
}

void Glottis::setVoicing(bool voice)
{
	this->alwaysVoice = voice;
}

void Glottis::setActive(bool active)
{
	this->isActive = active;
}
