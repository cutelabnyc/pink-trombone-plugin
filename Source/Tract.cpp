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

void initializeTractProps(t_tractProps *props, int n)
{
	props->n = n;
	props->bladeStart = 10;
	props->tipStart = 32;
	props->lipStart = 39;
	props->bladeStart = (int) floor(props->bladeStart * (double) n / 44.0);
	props->tipStart = (int) floor(props->tipStart * (double) n / 44.0);
	props->lipStart = (int) floor(props->lipStart * (double) n / 44.0);
	props->tongueIndex = props->bladeStart;
	props->tongueDiameter = 3.5;
	props->tractDiameter = (double *) calloc(n, sizeof(double));
	props->noseLength = (int) floor(28.0 * (double) props->n / 44.0);
	props->noseDiameter = (double *) calloc(props->n, sizeof(double));
	props->noseStart = props->n - props->noseLength + 1; // =17 for n =44
	props->noseOffset = 0.8;
	props->maxAmplitude = (double *) calloc(n, sizeof(double));
	props->noseMaxAmplitude = (double *) calloc(props->n, sizeof(double));
	
	props->extraNoseLength = (int) floor(28.0 * (double) props->n / 44.0);
	props->extraNoseDiameter = (double *) calloc(props->n, sizeof(double));
	props->extraNoseStart = props->n - props->noseLength + 1; // =17 for n =44
	props->extraNoseMaxAmplitude = (double *) calloc(props->n, sizeof(double));
}

Tract::Tract(double sampleRate, double blockTime, t_tractProps *props):
	lipOutput(0),
	noseOutput(0),
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
	this->tractProps = props;
	this->init();
}

void Tract::init() {
	this->diameter = (double *) calloc(this->tractProps->n, sizeof(double));
	this->restDiameter = (double *) calloc(this->tractProps->n, sizeof(double));
	this->targetDiameter = (double *) calloc(this->tractProps->n, sizeof(double));
	this->newDiameter = (double *) calloc(this->tractProps->n, sizeof(double));
	for (int i = 0; i < this->tractProps->n; i++)
	{
		double diameter = 0;
		if (i < 7 * (double) this->tractProps->n / 44.0 -0.5) diameter = 0.6;
		else if (i < 12 * (double) this->tractProps->n / 44.0) diameter = 1.1;
		else diameter = 1.5;
		this->diameter[i] = this->restDiameter[i] = this->targetDiameter[i] = this->newDiameter[i] = diameter;
	}
	this->R = (double *) calloc(this->tractProps->n, sizeof(double));
	this->L = (double *) calloc(this->tractProps->n, sizeof(double));
	this->reflection = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->newReflection = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->junctionOutputR = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->junctionOutputL = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->A = (double *) calloc(this->tractProps->n, sizeof(double));
	
	this->noseR = (double *) calloc(this->tractProps->n, sizeof(double));
	this->noseL = (double *) calloc(this->tractProps->n, sizeof(double));
	this->noseJunctionOutputR = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->noseJunctionOutputL = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->noseReflection = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->noseDiameter = (double *) calloc(this->tractProps->n, sizeof(double));
	this->noseA = (double *) calloc(this->tractProps->n, sizeof(double));
	for (int i = 0; i < this->tractProps->noseLength; i++)
	{
		double diameter;
		double d = 2.0 * ((double) i / (double) this->tractProps->noseLength);
		if (d < 1.0) diameter = 0.4 + 1.6 * d;
		else diameter = 0.5 + 1.5 * (2.0 - d);
		diameter = fmin(diameter, 1.9);
		this->noseDiameter[i] = diameter;
	}
	
	this->extraNoseR = (double *) calloc(this->tractProps->n, sizeof(double));
	this->extraNoseL = (double *) calloc(this->tractProps->n, sizeof(double));
	this->extraNoseJunctionOutputR = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->extraNoseJunctionOutputL = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->extraNoseReflection = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->extraNoseDiameter = (double *) calloc(this->tractProps->n, sizeof(double));
	this->extraNoseA = (double *) calloc(this->tractProps->n, sizeof(double));
	for (int i = 0; i < this->tractProps->extraNoseLength; i++)
	{
		double diameter;
		double d = 2.0 * ((double) i / (double) this->tractProps->extraNoseLength);
		if (d < 1.0) diameter = 0.4 + 1.6 * d;
		else diameter = 0.5 + 1.5 * (2.0 - d);
		diameter = fmin(diameter, 1.9);
		this->extraNoseDiameter[i] = diameter;
	}
	
	this->newReflectionLeft = this->newReflectionRight = this->newReflectionNose = 0.0;
	this->newExtraReflectionLeft = this->newExtraReflectionRight = this->newReflectionExtraNose = 0.0;
	this->calculateReflections();
	this->calculateNoseReflections();
	this->noseDiameter[0] = this->velumTarget;
	this->extraNoseDiameter[0] = this->velumTarget;
	memcpy(this->tractProps->tractDiameter, this->diameter, sizeof(double) * this->tractProps->n);
	memcpy(this->tractProps->noseDiameter, this->noseDiameter, sizeof(double) * this->tractProps->n);
	memcpy(this->tractProps->extraNoseDiameter, this->extraNoseDiameter, sizeof(double) * this->tractProps->n);
}

long Tract::getTractIndexCount()
{
	return this->tractProps->n;
}

long Tract::tongueIndexLowerBound()
{
	return this->tractProps->bladeStart + 2;
}

long Tract::tongueIndexUpperBound()
{
	return this->tractProps->tipStart - 3;
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

void Tract::addTurbulenceNoise(double turbulenceNoise, double glotModulator)
{
	if (this->constrictionIndex < 2.0 || this->constrictionIndex > (double) this->tractProps->n) {
		return;
	}
	if (this->constrictionDiameter <= 0.0) return;
    
	double intensity = this->fricativeIntensity;
	this->addTurbulenceNoiseAtIndex(0.66 * turbulenceNoise * intensity, this->constrictionIndex, this->constrictionDiameter, glotModulator);
}

void Tract::addTurbulenceNoiseAtIndex(double turbulenceNoise, double index, double diameter, double glotModulator)
{
	long i = (long) floor(index);
	double delta = index - (double) i;
	turbulenceNoise *= glotModulator;
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
	for (int i = 0; i < this->tractProps->n; i++)
	{
		this->A[i] = this->diameter[i] * this->diameter[i]; //ignoring PI etc.
	}
	for (int i=1; i<this->tractProps->n; i++)
	{
		this->reflection[i] = this->newReflection[i];
		if (this->A[i] == 0) this->newReflection[i] = 0.999; //to prevent some bad behaviour if 0
		else this->newReflection[i] = (this->A[i-1]-this->A[i]) / (this->A[i-1]+this->A[i]);
	}
	
	//now at junction with nose
	this->reflectionLeft = this->newReflectionLeft;
	this->reflectionRight = this->newReflectionRight;
	this->reflectionNose = this->newReflectionNose;
	double sum = this->A[this->tractProps->noseStart] + this->A[this->tractProps->noseStart + 1] + this->noseA[0];
	this->newReflectionLeft = (2.0 * this->A[this->tractProps->noseStart] - sum) / sum;
	this->newReflectionRight = (2 * this->A[this->tractProps->noseStart + 1] - sum) / sum;
	this->newReflectionNose = (2 * this->noseA[0] - sum) / sum;
	
	//now at junction with extra nose
	this->extraReflectionLeft = this->newExtraReflectionLeft;
	this->extraReflectionRight = this->newExtraReflectionRight;
	this->reflectionExtraNose = this->newReflectionExtraNose;
	sum = this->A[this->tractProps->extraNoseStart] + this->A[this->tractProps->extraNoseStart + 1] + this->extraNoseA[0];
	this->newExtraReflectionLeft = (2.0 * this->A[this->tractProps->extraNoseStart] - sum) / sum;
	this->newExtraReflectionRight = (2 * this->A[this->tractProps->extraNoseStart + 1] - sum) / sum;
	this->newReflectionExtraNose = (2 * this->extraNoseA[0] - sum) / sum;
}

void Tract::calculateNoseReflections()
{
	for (int i = 0; i < this->tractProps->noseLength; i++)
	{
		this->noseA[i] = this->noseDiameter[i] * this->noseDiameter[i];
	}
	for (int i = 1; i < this->tractProps->noseLength; i++)
	{
		this->noseReflection[i] = (this->noseA[i - 1] - this->noseA[i]) / (this->noseA[i - 1] + this->noseA[i]);
	}
	
	for (int i = 0; i < this->tractProps->extraNoseLength; i++)
	{
		this->extraNoseA[i] = this->extraNoseDiameter[i] * this->extraNoseDiameter[i];
	}
	for (int i = 1; i < this->tractProps->extraNoseLength; i++)
	{
		this->extraNoseReflection[i] = (this->extraNoseA[i - 1] - this->extraNoseA[i]) / (this->extraNoseA[i - 1] + this->extraNoseA[i]);
	}
}

void Tract::finishBlock()
{
	this->reshapeTract(this->blockTime);
	this->calculateReflections();
	memcpy(this->tractProps->tractDiameter, this->diameter, sizeof(double) * this->tractProps->n);
	memcpy(this->tractProps->noseDiameter, this->noseDiameter, sizeof(double) * this->tractProps->noseLength);
	memcpy(this->tractProps->extraNoseDiameter, this->extraNoseDiameter, sizeof(double) * this->tractProps->extraNoseLength);
}

void Tract::setRestDiameter(double tongueIndex, double tongueDiameter)
{
	this->tractProps->tongueIndex = tongueIndex;
	this->tractProps->tongueDiameter = tongueDiameter;
	for (long i = this->tractProps->bladeStart; i < this->tractProps->lipStart; i++)
	{
		double t = 1.1 * M_PI * (double) (tongueIndex - i) / (double) (this->tractProps->tipStart - this->tractProps->bladeStart);
		double fixedTongueDiameter = 2 + (tongueDiameter - 2) / 1.5;
		double curve = (1.5 - fixedTongueDiameter + 1.7) * cos(t);
		if (i == this->tractProps->bladeStart-2 || i == this->tractProps->lipStart-1) curve *= 0.8;
		if (i == this->tractProps->bladeStart || i == this->tractProps->lipStart-2) curve *= 0.94;
		this->restDiameter[i] = 1.5 - curve;
	}
	for (long i = 0; i < this->tractProps->n; i++) {
		this->targetDiameter[i] = this->restDiameter[i];
	}
}

void Tract::setConstriction(double cindex, double cdiam, double fricativeIntensity)
{
	this->constrictionIndex = cindex;
	this->constrictionDiameter = cdiam;
	this->fricativeIntensity = fricativeIntensity;
	
	if (this->constrictionDiameter < -1.5 - this->tractProps->noseOffset) {  //-1.5 ex recto (needed to fix bug) (-0.85 originally)
		return;
	}
	
	double diameter = this->constrictionDiameter - 0.3;
	if (diameter < 0) diameter = 0;
	long width = 2;
	if (this->constrictionIndex < 25) width = 10;
	else if (this->constrictionIndex >= this->tractProps->tipStart) width= 5;
	else width = 10.0 - 5 * (this->constrictionIndex - 25) / ((double) this->tractProps->tipStart - 25.0);
	if (this->constrictionIndex >= 2 && this->constrictionIndex < this->tractProps->n && diameter < 3)
	{
		long intIndex = round(this->constrictionIndex);
		for (long i = -ceil(width) - 1; i < width + 1; i++)
		{
			if (intIndex + i < 0 || intIndex + i >= this->tractProps->n) continue;
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

void Tract::openNose(bool openNose)
{
	if(openNose) this->velumTarget = 0.4;
	else this->velumTarget = 0.01;
}

void Tract::setNoseLength(double noseLength, double extraNoseLength)
{
	this->tractProps->noseLength = (int) floor(noseLength * (double) this->tractProps->n / 44.0);
	for (int i = 0; i < this->tractProps->noseLength; i++)
	{
		double diameter;
		double d = 2.0 * ((double) i / (double) this->tractProps->noseLength);
		if (d < 1.0) {
			diameter = 0.4 + 1.6 * d;
		}
		else {
			diameter = 0.5 + 1.5 * (2.0 - d);
		}
		diameter = fmin(diameter, 1.9);
		this->noseDiameter[i] = diameter;
	}
	
	if (this->extraNose)
	{
		this->tractProps->extraNoseLength = (int) floor(extraNoseLength * (double) this->tractProps->n / 44.0);
		for (int i = 0; i < this->tractProps->extraNoseLength; i++)
		{
			double diameter;
			double d = 2.0 * ((double) i / (double) this->tractProps->extraNoseLength);
			if (d < 1.0) {
				diameter = 0.4 + 1.6 * d;
			}
			else {
				diameter = 0.5 + 1.5 * (2.0 - d);
			}
			diameter = fmin(diameter, 1.9);
			this->extraNoseDiameter[i] = diameter;
		}
	}
}

void Tract::setNoseAttachment(double noseAttachment, double extraNoseAttachment)
{
	this->tractProps->noseStart = (int) floor(noseAttachment * (double) this->tractProps->n / 44.0);
	this->tractProps->extraNoseStart = (int) floor(extraNoseAttachment * (double) this->tractProps->n / 44.0);
}

void Tract::setExtraNose(bool extraNose)
{
	this->extraNose = extraNose;
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
	for (int i = 0; i < this->tractProps->n; i++)
	{
		double diameter = this->diameter[i];
		double targetDiameter = this->targetDiameter[i];
		if (diameter <= 0) newLastObstruction = i;
		double slowReturn;
		if (i < this->tractProps->noseStart) slowReturn = 0.6;
		else if (i >= this->tractProps->tipStart) slowReturn = 1.0;
		else slowReturn = 0.6 + 0.4 * (i - this->tractProps->noseStart) / (this->tractProps->tipStart - this->tractProps->noseStart);
		this->diameter[i] = moveTowards(diameter, targetDiameter, slowReturn*amount, 2*amount);
	}
	if (this->lastObstruction > -1 && newLastObstruction == -1 && this->noseA[0]<0.05)
	{
		this->addTransient(this->lastObstruction);
	}
	this->lastObstruction = newLastObstruction;

	amount = deltaTime * this->movementSpeed;
	this->noseDiameter[0] = moveTowards(this->noseDiameter[0], this->velumTarget, amount * 0.25, amount * 0.1);
	this->tractProps->noseDiameter[0] = this->noseDiameter[0];
	this->noseA[0] = this->noseDiameter[0] * this->noseDiameter[0];
	
	this->extraNoseDiameter[0] = moveTowards(this->extraNoseDiameter[0], this->velumTarget, amount * 0.25, amount * 0.1);
	this->tractProps->extraNoseDiameter[0] = this->extraNoseDiameter[0];
	this->extraNoseA[0] = this->extraNoseDiameter[0] * this->extraNoseDiameter[0];
}

void Tract::runStep(double glottalOutput, double turbulenceNoise, double lambda, double glotModulator)
{
	double updateAmplitudes = ((double) rand() / (double) RAND_MAX) < 0.1;
	
	//mouth
	this->processTransients();
	this->addTurbulenceNoise(turbulenceNoise, glotModulator);
	
	//this->glottalReflection = -0.8 + 1.6 * Glottis.newTenseness;
	this->junctionOutputR[0] = this->L[0] * this->glottalReflection + glottalOutput;
	this->junctionOutputL[this->tractProps->n] = this->R[this->tractProps->n-1] * this->lipReflection;
	
	for (int i = 1; i < this->tractProps->n; i++)
	{
		double r = this->reflection[i] * (1-lambda) + this->newReflection[i]*lambda;
		double w = r * (this->R[i - 1] + this->L[i]);
		this->junctionOutputR[i] = this->R[i - 1] - w;
		this->junctionOutputL[i] = this->L[i] + w;
	}
	
	//now at junction with nose
	int i = this->tractProps->noseStart;
	double r = this->newReflectionLeft * (1 - lambda) + this->reflectionLeft * lambda;
	this->junctionOutputL[i] = r * this->R[i - 1] + (1 + r) * (this->noseL[0] + this->L[i]);
	r = this->newReflectionRight * (1 - lambda) + this->reflectionRight * lambda;
	this->junctionOutputR[i] = r * this->L[i] + (1 + r) * (this->R[i - 1] + this->noseL[0]);
	r = this->newReflectionNose * (1 - lambda) + this->reflectionNose * lambda;
	this->noseJunctionOutputR[0] = r * this->noseL[0] + (1 + r) * (this->L[i] + this->R[i - 1]);
	
	//now at junction with extra nose
	if(this->extraNose)
	{
		i = this->tractProps->extraNoseStart;
		r = this->newExtraReflectionLeft * (1 - lambda) + this->extraReflectionLeft * lambda;
		this->junctionOutputL[i] = r * this->R[i - 1] + (1 + r) * (this->extraNoseL[0] + this->L[i]);
		r = this->newExtraReflectionRight * (1 - lambda) + this->extraReflectionRight * lambda;
		this->junctionOutputR[i] = r * this->L[i] + (1 + r) * (this->R[i - 1] + this->extraNoseL[0]);
		r = this->newReflectionExtraNose * (1 - lambda) + this->reflectionExtraNose * lambda;
		this->extraNoseJunctionOutputR[0] = r * this->extraNoseL[0] + (1 + r) * (this->L[i] + this->R[i - 1]);
	}
	
	for (int i=0; i < this->tractProps->n; i++)
	{
		this->R[i] = this->junctionOutputR[i] * 0.999;
		this->L[i] = this->junctionOutputL[i + 1] * 0.999;
		
		//this->R[i] = Math.clamp(this->junctionOutputR[i] * this->fade, -1, 1);
		//this->L[i] = Math.clamp(this->junctionOutputL[i+1] * this->fade, -1, 1);
		
		if (updateAmplitudes)
		{
			double amplitude = fabs(this->R[i] + this->L[i]);
			if (amplitude > this->tractProps->maxAmplitude[i]) this->tractProps->maxAmplitude[i] = amplitude;
			else this->tractProps->maxAmplitude[i] *= 0.999;
		}
	}
	
	this->lipOutput = this->R[this->tractProps->n - 1];
	
	//nose
	this->noseJunctionOutputL[this->tractProps->noseLength] = this->noseR[this->tractProps->noseLength - 1] * this->lipReflection;
	
	for (int i=1; i < this->tractProps->noseLength; i++)
	{
		int w = this->noseReflection[i] * (this->noseR[i - 1] + this->noseL[i]);
		this->noseJunctionOutputR[i] = this->noseR[i - 1] - w;
		this->noseJunctionOutputL[i] = this->noseL[i] + w;
	}
	
	for (int i=0; i < this->tractProps->noseLength; i++)
	{
		this->noseR[i] = this->noseJunctionOutputR[i] * this->fade;
		this->noseL[i] = this->noseJunctionOutputL[i + 1] * this->fade;
		
		//this->noseR[i] = Math.clamp(this->noseJunctionOutputR[i] * this->fade, -1, 1);
		//this->noseL[i] = Math.clamp(this->noseJunctionOutputL[i+1] * this->fade, -1, 1);
		
		if (updateAmplitudes)
		{
			double amplitude = fabs(this->noseR[i] + this->noseL[i]);
			if (amplitude > this->tractProps->noseMaxAmplitude[i]) this->tractProps->noseMaxAmplitude[i] = amplitude;
			else this->tractProps->noseMaxAmplitude[i] *= 0.999;
		}
	}
	
	this->noseOutput = this->noseR[this->tractProps->noseLength - 1];
	
	if(extraNose){
		this->extraNoseJunctionOutputL[this->tractProps->extraNoseLength] = this->noseR[this->tractProps->noseLength - 1] * this->lipReflection;
		
		for (int i=1; i < this->tractProps->extraNoseLength; i++)
		{
			int w = this->extraNoseReflection[i] * (this->extraNoseR[i - 1] + this->extraNoseL[i]);
			this->extraNoseJunctionOutputR[i] = this->extraNoseR[i - 1] - w;
			this->extraNoseJunctionOutputL[i] = this->extraNoseL[i] + w;
		}
		
		for (int i=0; i < this->tractProps->extraNoseLength; i++)
		{
			this->extraNoseR[i] = this->extraNoseJunctionOutputR[i] * this->fade;
			this->extraNoseL[i] = this->extraNoseJunctionOutputL[i + 1] * this->fade;
			
			//this->noseR[i] = Math.clamp(this->noseJunctionOutputR[i] * this->fade, -1, 1);
			//this->noseL[i] = Math.clamp(this->noseJunctionOutputL[i+1] * this->fade, -1, 1);
			
			if (updateAmplitudes)
			{
				double amplitude = fabs(this->extraNoseR[i] + this->extraNoseL[i]);
				if (amplitude > this->tractProps->extraNoseMaxAmplitude[i]) this->tractProps->extraNoseMaxAmplitude[i] = amplitude;
				else this->tractProps->extraNoseMaxAmplitude[i] *= 0.999;
			}
		}
		this->extraNoseOutput = this->extraNoseR[this->tractProps->noseLength - 1];
	}
}
