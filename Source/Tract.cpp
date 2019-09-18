//
//  Tract
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 9/5/19.
//

#include "Tract.hpp"
#include <math.h>
#include "util.h"

#define MAX_TRANSIENTS (20)

typedef struct t_transient {
	int position;
	double timeAlive;
	double lifeTime;
	double strength;
	double exponent;
	bool living;
} t_transient;

Tract::Tract(double sampleRate, double blockTime):
	lipOutput(0),
	noseOutput(0),
	n(44),
	bladeStart(10),
	tipStart(32),
	lipStart(39),
	glottalReflection(0.75),
	lipReflection(-0.85),
	lastObstruction(-1),
	fade(1.0), //0.9999,
	movementSpeed(15), //cm per second
	velumTarget(0.01),
	constrictionIndex(3.0), // TODO values ex recto
	constrictionDiameter(1.0) // TODO values ex recto
{
	this->sampleRate = sampleRate;
	this->blockTime = blockTime;
	this->transients = (t_transient *) calloc(MAX_TRANSIENTS, sizeof(t_transient));
	this->transientCount = 0;
	this->init();
}

void Tract::init() {
	this->bladeStart = (int) floor(this->bladeStart * (double) this->n / 44.0);
	this->tipStart = (int) floor(this->tipStart * (double) this->n / 44.0);
	this->lipStart = (int) floor(this->lipStart * (double) this->n / 44.0);
	this->diameter = (double *) calloc(this->n, sizeof(double));
	this->restDiameter = (double *) calloc(this->n, sizeof(double));
	this->targetDiameter = (double *) calloc(this->n, sizeof(double));
	this->newDiameter = (double *) calloc(this->n, sizeof(double));
	for (int i = 0; i < this->n; i++)
	{
		double diameter = 0;
		if (i < 7 * (double) this->n / 44.0 -0.5) diameter = 0.6;
		else if (i < 12 * (double) this->n / 44.0) diameter = 1.1;
		else diameter = 1.5;
		this->diameter[i] = this->restDiameter[i] = this->targetDiameter[i] = this->newDiameter[i] = diameter;
	}
	this->R = (double *) calloc(this->n, sizeof(double));
	this->L = (double *) calloc(this->n, sizeof(double));
	this->reflection = (double *) calloc(this->n + 1, sizeof(double));
	this->newReflection = (double *) calloc(this->n + 1, sizeof(double));
	this->junctionOutputR = (double *) calloc(this->n + 1, sizeof(double));
	this->junctionOutputL = (double *) calloc(this->n + 1, sizeof(double));
	this->A = (double *) calloc(this->n, sizeof(double));
	this->maxAmplitude = (double *) calloc(this->n, sizeof(double));
	
	this->noseLength = (int) floor(28.0 * (double) this->n / 44.0);
	this->noseStart = this->n - this->noseLength + 1;
	this->noseR = (double *) calloc(this->noseLength, sizeof(double));
	this->noseL = (double *) calloc(this->noseLength, sizeof(double));
	this->noseJunctionOutputR = (double *) calloc(this->noseLength + 1, sizeof(double));
	this->noseJunctionOutputL = (double *) calloc(this->noseLength + 1, sizeof(double));
	this->noseReflection = (double *) calloc(this->noseLength + 1, sizeof(double));
	this->noseDiameter = (double *) calloc(this->noseLength, sizeof(double));
	this->noseA = (double *) calloc(this->noseLength, sizeof(double));
	this->noseMaxAmplitude = (double *) calloc(this->noseLength, sizeof(double));
	for (int i = 0; i < this->noseLength; i++)
	{
		double diameter;
		double d = 2.0 * ((double) i / (double) this->noseLength);
		if (d < 1.0) diameter = 0.4 + 1.6 * d;
		else diameter = 0.5 + 1.5 * (2.0 - d);
		diameter = fmin(diameter, 1.9);
		this->noseDiameter[i] = diameter;
	}
	this->newReflectionLeft = this->newReflectionRight = this->newReflectionNose = 0.0;
	this->calculateReflections();
	this->calculateNoseReflections();
	this->noseDiameter[0] = this->velumTarget;
}

long Tract::getTractIndexCount()
{
	return this->n;
}

long Tract::tongueIndexLowerBound()
{
	return this->bladeStart + 2;
}

long Tract::tongueIndexUpperBound()
{
	return this->tipStart - 3;
}

void Tract::addTransient(int position)
{
	if (this->transientCount < MAX_TRANSIENTS) {
		t_transient *trans = nullptr;
		for (int i = 0; i < MAX_TRANSIENTS; i++) {
			trans = this->transients + i;
			if (trans->living == false) break;
		}
		if (trans) {
			trans->position = position;
			trans->timeAlive = 0;
			trans->lifeTime = 0.2;
			trans->strength = 0.3;
			trans->exponent = 200;
			trans->living = true;
		}
		this->transientCount++;
	}
}

void Tract::addTurbulenceNoise(double turbulenceNoise, Glottis *glottis)
{
	if (this->constrictionIndex < 2.0 || this->constrictionIndex > (double) this->n) {
		return;
	}
	if (this->constrictionDiameter <= 0.0) return;
	double intensity = this->fricativeIntensity;
	this->addTurbulenceNoiseAtIndex(0.66 * turbulenceNoise * intensity, this->constrictionIndex, this->constrictionDiameter, glottis);
}

void Tract::addTurbulenceNoiseAtIndex(double turbulenceNoise, double index, double diameter, Glottis *glottis)
{
	long i = (long) floor(index);
	double delta = index - (double) i;
	turbulenceNoise *= glottis->getNoiseModulator();
	double thinness0 = clamp(8.0 * (0.7 - diameter), 0.0, 1.0);
	double openness = clamp(30.0 * (diameter - 0.3), 0.0, 1.0);
	double noise0 = turbulenceNoise * (1.0 - delta) * thinness0 * openness;
	double noise1 = turbulenceNoise * delta * thinness0 * openness;
	this->R[i + 1] += noise0 / 2.0;
	this->L[i + 1] += noise0 / 2.0;
	this->R[i + 2] += noise1 / 2.0;
	this->L[i + 2] += noise1 / 2.0;
}

void Tract::calculateReflections()
{
	for (int i = 0; i < this->n; i++)
	{
		this->A[i] = this->diameter[i] * this->diameter[i]; //ignoring PI etc.
	}
	for (int i=1; i<this->n; i++)
	{
		this->reflection[i] = this->newReflection[i];
		if (this->A[i] == 0) this->newReflection[i] = 0.999; //to prevent some bad behaviour if 0
		else this->newReflection[i] = (this->A[i-1]-this->A[i]) / (this->A[i-1]+this->A[i]);
	}
	
	//now at junction with nose
	this->reflectionLeft = this->newReflectionLeft;
	this->reflectionRight = this->newReflectionRight;
	this->reflectionNose = this->newReflectionNose;
	double sum = this->A[this->noseStart] + this->A[this->noseStart + 1] + this->noseA[0];
	this->newReflectionLeft = (2.0 * this->A[this->noseStart] - sum) / sum;
	this->newReflectionRight = (2 * this->A[this->noseStart + 1] - sum) / sum;
	this->newReflectionNose = (2 * this->noseA[0] - sum) / sum;
}

void Tract::calculateNoseReflections()
{
	for (int i = 0; i < this->noseLength; i++)
	{
		this->noseA[i] = this->noseDiameter[i] * this->noseDiameter[i];
	}
	for (int i = 1; i < this->noseLength; i++)
	{
		this->noseReflection[i] = (this->noseA[i - 1] - this->noseA[i]) / (this->noseA[i - 1] + this->noseA[i]);
	}
}

void Tract::finishBlock()
{
	this->reshapeTract(this->blockTime);
	this->calculateReflections();
}

void Tract::setRestDiameter(long tongueIndex, double tongueDiameter)
{
	for (long i = this->bladeStart; i < this->lipStart; i++)
	{
		double t = 1.1 * M_PI * (double) (tongueIndex - i) / (double) (this->tipStart - this->bladeStart);
		double fixedTongueDiameter = 2 + (tongueDiameter - 2) / 1.5;
		double curve = (1.5 - fixedTongueDiameter + 1.7) * cos(t);
		if (i == this->bladeStart-2 || i == this->lipStart-1) curve *= 0.8;
		if (i == this->bladeStart || i == this->lipStart-2) curve *= 0.94;
		this->restDiameter[i] = 1.5 - curve;
	}
	for (long i = 0; i < this->n; i++) {
		this->targetDiameter[i] = this->restDiameter[i];
	}
}

void Tract::setConstriction(double cindex, double cdiam, double fricativeIntensity)
{
	this->constrictionIndex = cindex;
	this->constrictionDiameter = cdiam;
	this->fricativeIntensity = fricativeIntensity;
	
	// This is basically the Tract touch handling code
	this->velumTarget = 0.01;
	if (this->constrictionIndex > this->noseStart && this->constrictionDiameter < -this->noseOffset)
	{
		this->velumTarget = 0.4;
	}
	if (this->constrictionDiameter < -0.85 - this->noseOffset) {
		return;
	}
	
	double diameter = this->constrictionDiameter - 0.3;
	if (diameter < 0) diameter = 0;
	long width = 2;
	if (this->constrictionIndex < 25) width = 10;
	else if (this->constrictionIndex >= this->tipStart) width= 5;
	else width = 10.0 - 5 * (this->constrictionIndex - 25) / ((double) this->tipStart - 25.0);
	if (this->constrictionIndex >= 2 && this->constrictionIndex < this->n && diameter < 3)
	{
		long intIndex = round(this->constrictionIndex);
		for (long i = -ceil(width) - 1; i < width + 1; i++)
		{
			if (intIndex + i < 0 || intIndex + i >= this->n) continue;
			double relpos = (intIndex + i) - this->constrictionIndex;
			relpos = abs(relpos) - 0.5;
			double shrink;
			if (relpos <= 0) shrink = 0;
			else if (relpos > width) shrink = 1;
			else shrink = 0.5 * (1 - cos(M_PI * relpos / (double) width));
			if (diameter < this->targetDiameter[intIndex + i])
			{
				this->targetDiameter[intIndex + i] = diameter + (this->targetDiameter[intIndex + i] - diameter) * shrink;
			}
		}
	}
}

void Tract::processTransients()
{
	for (int i = 0; i < this->transientCount; i++)
	{
		t_transient *trans = this->transients + i;
		double amplitude = trans->strength * pow(2.0, -trans->exponent * trans->timeAlive);
		this->R[trans->position] += amplitude / 2.0;
		this->L[trans->position] += amplitude / 2.0;
		trans->timeAlive += 1.0 / (this->sampleRate * 2.0);
	}
	for (int i = this->transientCount - 1; i >= 0; i--)
	{
		t_transient *trans = this->transients + i;
		if (trans->timeAlive > trans->lifeTime)
		{
			trans->living = false;
		}
	}
}

void Tract::reshapeTract(double deltaTime)
{
	double amount = deltaTime * this->movementSpeed; ;
	int newLastObstruction = -1;
	for (int i = 0; i < this->n; i++)
	{
		double diameter = this->diameter[i];
		double targetDiameter = this->targetDiameter[i];
		if (diameter <= 0) newLastObstruction = i;
		double slowReturn;
		if (i < this->noseStart) slowReturn = 0.6;
		else if (i >= this->tipStart) slowReturn = 1.0;
		else slowReturn = 0.6 + 0.4 * (i - this->noseStart) / (this->tipStart - this->noseStart);
		this->diameter[i] = moveTowards(diameter, targetDiameter, slowReturn*amount, 2*amount);
	}
	if (this->lastObstruction > -1 && newLastObstruction == -1 && this->noseA[0]<0.05)
	{
		this->addTransient(this->lastObstruction);
	}
	this->lastObstruction = newLastObstruction;
	
	amount = deltaTime * this->movementSpeed;
	this->noseDiameter[0] = moveTowards(this->noseDiameter[0], this->velumTarget, amount * 0.25, amount * 0.1);
	this->noseA[0] = this->noseDiameter[0] * this->noseDiameter[0];
}

void Tract::runStep(double glottalOutput, double turbulenceNoise, double lambda, Glottis *glottis)
{
	double updateAmplitudes = ((double) rand() / (double) RAND_MAX) < 0.1;
	
	//mouth
	this->processTransients();
	this->addTurbulenceNoise(turbulenceNoise, glottis);
	
	//this->glottalReflection = -0.8 + 1.6 * Glottis.newTenseness;
	this->junctionOutputR[0] = this->L[0] * this->glottalReflection + glottalOutput;
	this->junctionOutputL[this->n] = this->R[this->n-1] * this->lipReflection;
	
	for (int i = 1; i < this->n; i++)
	{
		double r = this->reflection[i] * (1-lambda) + this->newReflection[i]*lambda;
		double w = r * (this->R[i - 1] + this->L[i]);
		this->junctionOutputR[i] = this->R[i - 1] - w;
		this->junctionOutputL[i] = this->L[i] + w;
	}
	
	//now at junction with nose
	int i = this->noseStart;
	double r = this->newReflectionLeft * (1 - lambda) + this->reflectionLeft * lambda;
	this->junctionOutputL[i] = r * this->R[i - 1] + (1 + r) * (this->noseL[0] + this->L[i]);
	r = this->newReflectionRight * (1 - lambda) + this->reflectionRight * lambda;
	this->junctionOutputR[i] = r * this->L[i] + (1 + r) * (this->R[i - 1] + this->noseL[0]);
	r = this->newReflectionNose * (1 - lambda) + this->reflectionNose * lambda;
	this->noseJunctionOutputR[0] = r * this->noseL[0] + (1 + r) * (this->L[i] + this->R[i - 1]);
	
	for (int i=0; i < this->n; i++)
	{
		this->R[i] = this->junctionOutputR[i] * 0.999;
		this->L[i] = this->junctionOutputL[i + 1] * 0.999;
		
		//this->R[i] = Math.clamp(this->junctionOutputR[i] * this->fade, -1, 1);
		//this->L[i] = Math.clamp(this->junctionOutputL[i+1] * this->fade, -1, 1);
		
		if (updateAmplitudes)
		{
			double amplitude = fabs(this->R[i] + this->L[i]);
			if (amplitude > this->maxAmplitude[i]) this->maxAmplitude[i] = amplitude;
			else this->maxAmplitude[i] *= 0.999;
		}
	}
	
	this->lipOutput = this->R[this->n - 1];
	
	//nose
	this->noseJunctionOutputL[this->noseLength] = this->noseR[this->noseLength - 1] * this->lipReflection;
	
	for (int i=1; i < this->noseLength; i++)
	{
		int w = this->noseReflection[i] * (this->noseR[i - 1] + this->noseL[i]);
		this->noseJunctionOutputR[i] = this->noseR[i - 1] - w;
		this->noseJunctionOutputL[i] = this->noseL[i] + w;
	}
	
	for (int i=0; i < this->noseLength; i++)
	{
		this->noseR[i] = this->noseJunctionOutputR[i] * this->fade;
		this->noseL[i] = this->noseJunctionOutputL[i + 1] * this->fade;
		
		//this->noseR[i] = Math.clamp(this->noseJunctionOutputR[i] * this->fade, -1, 1);
		//this->noseL[i] = Math.clamp(this->noseJunctionOutputL[i+1] * this->fade, -1, 1);
		
		if (updateAmplitudes)
		{
			double amplitude = fabs(this->noseR[i] + this->noseL[i]);
			if (amplitude > this->noseMaxAmplitude[i]) this->noseMaxAmplitude[i] = amplitude;
			else this->noseMaxAmplitude[i] *= 0.999;
		}
	}
	
	this->noseOutput = this->noseR[this->noseLength - 1];
}
