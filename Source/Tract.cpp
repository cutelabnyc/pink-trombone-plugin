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
	props->maxAmplitude = (double *) calloc(n, sizeof(double));
    
    props->noseProps = (t_noseProps *) calloc(MAX_NOSES, sizeof(t_noseProps));
    for (int i = 0; i < MAX_NOSES; i++) {
        props->noseProps[i].length = (int) floor(28.0 * (double) props->n / 44.0);
        props->noseProps[i].diameter = (double *) calloc(props->n, sizeof(double));
        props->noseProps[i].start = props->n - props->noseProps->length + 1; // =17 for n =44
        props->noseProps[i].maxAmplitude = (double *) calloc(props->n, sizeof(double));
		props->noseProps[i].noseOffset = 0.8;
    }
}

Tract::Tract(double sampleRate, double blockTime, t_tractProps *props):
	lipOutput(0),
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
    this->blockTimeSeconds = blockTime / sampleRate;
	this->transients = (t_transient *) calloc(MAX_TRANSIENTS, sizeof(t_transient));
	this->transientCount = 0;
	this->tractProps = props;
    this->noseOutputs = (double *) calloc(MAX_NOSES, sizeof(double));
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
	// a more granular area function below. Numbers can be played with.
//	for (int i = 0; i < this->tractProps->n; i++)
//	{
//		double diameter = 0;
//
//		if (i < 5 * (double) this->tractProps->n / 44.0 -0.5) diameter = 0.6;
//		else if (i < 9 * (double) this->tractProps->n / 44.0) diameter = 1.6;
//		else if (i < 12 * (double) this->tractProps->n / 44.0) diameter = 1.3;
//		else if (i < 14 * (double) this->tractProps->n / 44.0) diameter = 1.4;
//		else if (i < 22 * (double) this->tractProps->n / 44.0) diameter = 1.1;
//		else if (i < 28 * (double) this->tractProps->n / 44.0) diameter = 1.6;
//		else if (i < 29 * (double) this->tractProps->n / 44.0) diameter = 1.9;
//		else if (i < 30 * (double) this->tractProps->n / 44.0) diameter = 2.1;
//		else if (i < 31 * (double) this->tractProps->n / 44.0) diameter = 2.7;
//		else if (i < 33 * (double) this->tractProps->n / 44.0) diameter = 3.2;
//		else if (i < 37 * (double) this->tractProps->n / 44.0) diameter = 3.5;
//		else if (i < 38 * (double) this->tractProps->n / 44.0) diameter = 3.4;
//		else if (i < 40 * (double) this->tractProps->n / 44.0) diameter = 3.6;
//		else if (i < 41 * (double) this->tractProps->n / 44.0) diameter = 3.1;
//		else if (i < 42 * (double) this->tractProps->n / 44.0) diameter = 2.6;
//		else diameter = 2.1;
//		this->diameter[i] = this->restDiameter[i] = this->targetDiameter[i] = this->newDiameter[i] = diameter;
//	}
	this->R = (double *) calloc(this->tractProps->n, sizeof(double));
	this->L = (double *) calloc(this->tractProps->n, sizeof(double));
	this->reflection = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->newReflection = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->junctionOutputR = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->junctionOutputL = (double *) calloc(this->tractProps->n + 1, sizeof(double));
	this->A = (double *) calloc(this->tractProps->n, sizeof(double));
	
    this->noses = (t_nose *) calloc(MAX_NOSES, sizeof(t_nose));
    for (int j = 0; j < MAX_NOSES; j++) {
        this->noses[j].noseR = (double *) calloc(this->tractProps->n, sizeof(double));
        this->noses[j].noseL = (double *) calloc(this->tractProps->n, sizeof(double));
        this->noses[j].noseJunctionOutputR = (double *) calloc(this->tractProps->n + 1, sizeof(double));
        this->noses[j].noseJunctionOutputL = (double *) calloc(this->tractProps->n + 1, sizeof(double));
        this->noses[j].noseReflection = (double *) calloc(this->tractProps->n + 1, sizeof(double));
        this->noses[j].noseDiameter = (double *) calloc(this->tractProps->n, sizeof(double));
        this->noses[j].noseA = (double *) calloc(this->tractProps->n, sizeof(double));
        
        for (int i = 0; i < this->tractProps->noseProps[j].length; i++)
        {
            double diameter;
            double d = 2.0 * ((double) i / (double) this->tractProps->noseProps[j].length);
            if (d < 1.0) diameter = 0.4 + 1.6 * d;
            else diameter = 0.5 + 1.5 * (2.0 - d);
            diameter = fmin(diameter, 1.9);
            this->noses[j].noseDiameter[i] = diameter;
        }
    }
    
    this->reflections = (t_reflections *) calloc(MAX_NOSES, sizeof(t_reflections));
    for (int j = 0; j < MAX_NOSES; j++) {
        this->reflections[j].newReflectionLeft = this->reflections[j].newReflectionRight = this->reflections[j].newReflectionNose = 0.0;
        this->noses[j].noseDiameter[0] = this->velumTarget;
        memcpy(this->tractProps->noseProps[j].diameter, this->noses[j].noseDiameter, sizeof(double) * this->tractProps->n);
    }
    memcpy(this->tractProps->tractDiameter, this->diameter, sizeof(double) * this->tractProps->n);
    
    this->calculateReflections();
    this->calculateNoseReflections();
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
    for (int j = 0; j < MAX_NOSES; j++) {
        this->reflections[j].reflectionLeft = this->reflections[j].newReflectionLeft;
        this->reflections[j].reflectionRight = this->reflections[j].newReflectionRight;
        this->reflections[j].reflectionNose = this->reflections[j].newReflectionNose;
		double sum = this->A[this->tractProps->noseProps[j].start] + this->A[this->tractProps->noseProps[j].start + 1] + this->noses[j].noseA[0];
		this->reflections[j].newReflectionLeft = (2.0 * this->A[this->tractProps->noseProps[j].start] - sum) / sum;
		this->reflections[j].newReflectionRight = (2 * this->A[this->tractProps->noseProps[j].start + 1] - sum) / sum;
        this->reflections[j].newReflectionNose = (2 * this->noses[j].noseA[0] - sum) / sum;
    }
}

void Tract::calculateNoseReflections()
{
    for (int j = 0; j < MAX_NOSES; j++) {
        for (int i = 0; i < this->tractProps->noseProps[j].length; i++)
        {
            this->noses[j].noseA[i] = this->noses[j].noseDiameter[i] * this->noses[j].noseDiameter[i];
        }
        for (int i = 1; i < this->tractProps->noseProps[j].length; i++)
        {
            this->noses[j].noseReflection[i] = (this->noses[j].noseA[i - 1] - this->noses[j].noseA[i]) / (this->noses[j].noseA[i - 1] + this->noses[j].noseA[i]);
        }
    }
}

void Tract::finishBlock()
{
	this->reshapeTract(this->blockTimeSeconds);
	this->calculateReflections();
	this->calculateNoseReflections();
	memcpy(this->tractProps->tractDiameter, this->diameter, sizeof(double) * this->tractProps->n);
    for (int j = 0; j < MAX_NOSES; j++) {
        memcpy(this->tractProps->noseProps[j].diameter, this->noses[j].noseDiameter, sizeof(double) * this->tractProps->noseProps[j].length);
    }
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
	
	if (this->constrictionDiameter < -1.5 - this->tractProps->noseProps[0].noseOffset) {  //-1.5 ex recto (needed to fix bug) (-0.85 originally)
		return;
	}
	
	double diameter = this->constrictionDiameter - 0.3;
	if (diameter < 0) diameter = 0;
	if (this->autoVelum) {
		if (diameter == 0 && (this->constrictionIndex > this->tractProps->noseProps[0].start || this->constrictionIndex > this->tractProps->noseProps[1].start)) this->velumTarget = 0.4;
		else this->velumTarget = 0.01;
	}
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

void Tract::setNoseLength(double noseLength, int index)
{
    this->tractProps->noseProps[index].length = (int) floor(noseLength * (double) this->tractProps->n / 44.0);
    for (int i = 0; i < this->tractProps->noseProps[index].length; i++)
    {
        double diameter;
        double d = 2.0 * ((double) i / (double) this->tractProps->noseProps[index].length);
        if (d < 1.0) {
            diameter = 0.4 + 1.6 * d;
        }
        else {
            diameter = 0.5 + 1.5 * (2.0 - d);
        }
        diameter = fmin(diameter, 1.9);
        this->noses[index].noseDiameter[i] = diameter;
    }
}

void Tract::setNoseAttachment(double noseAttachment, int index)
{
	this->tractProps->noseProps[index].start = (int) floor(noseAttachment * (double) this->tractProps->n / 44.0);
}

void Tract::setExtraNose(bool extraNose)
{
	this->extraNose = extraNose;
	this->tractProps->noseProps[1].noseOffset = this->tractProps->noseProps[0].noseOffset;
}

void Tract::setAutoVelum(bool autoVelum)
{
	this->autoVelum = autoVelum;
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
	double amount = deltaTime * this->movementSpeed;
	int newLastObstruction = -1;
	for (int i = 0; i < this->tractProps->n; i++)
	{
		double diameter = this->diameter[i];
		double targetDiameter = this->targetDiameter[i];
		if (diameter <= 0) newLastObstruction = i;
		double slowReturn;
		if (i < this->tractProps->noseProps[0].start) slowReturn = 0.6;
		else if (i >= this->tractProps->tipStart) slowReturn = 1.0;
		else slowReturn = 0.6 + 0.4 * (i - this->tractProps->noseProps[0].start) / (this->tractProps->tipStart - this->tractProps->noseProps[0].start);
		this->diameter[i] = moveTowards(diameter, targetDiameter, slowReturn*amount, 2*amount);
	}
	if (this->lastObstruction > -1 && newLastObstruction == -1 && this->noses[0].noseA[0]<0.05)
	{
		this->addTransient(this->lastObstruction);
	}
	this->lastObstruction = newLastObstruction;

	if(this->autoVelum && diameter == 0) amount = deltaTime * this->movementSpeed/100000;
	else amount = deltaTime * this->movementSpeed;
    
    for (int j = 0; j < MAX_NOSES; j++) {
        this->noses[j].noseDiameter[0] = moveTowards(this->noses[j].noseDiameter[0], this->velumTarget, amount * 0.5, amount * 0.1);
        this->tractProps->noseProps[j].diameter[0] = this->noses[j].noseDiameter[0];
        this->noses[j].noseA[0] = this->noses[j].noseDiameter[0] * this->noses[j].noseDiameter[0];
    }
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
	
    //now at junction with nose(s)
    for (int j = 0; j < (this->extraNose ? 2 : 1); j++) {
        int i = this->tractProps->noseProps[j].start;
        double r = this->reflections[j].newReflectionLeft * (1 - lambda) + this->reflections[j].reflectionLeft * lambda;
		this->junctionOutputL[i] = r * this->R[i - 1] + (1 + r) * (this->noses[j].noseL[0] + this->L[i]);
		r = this->reflections[j].newReflectionRight * (1 - lambda) + this->reflections[j].reflectionRight * lambda;
		this->junctionOutputR[i] = r * this->L[i] + (1 + r) * (this->R[i - 1] + this->noses[j].noseL[0]);
		r = this->reflections[j].newReflectionNose * (1 - lambda) + this->reflections[j].reflectionNose * lambda;
		this->noses[j].noseJunctionOutputR[0] = r * this->noses[j].noseL[0] + (1 + r) * (this->L[i] + this->R[i - 1]);
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
            amplitude = clamp(amplitude, -5, 5);
			if (amplitude > this->tractProps->maxAmplitude[i]) this->tractProps->maxAmplitude[i] = amplitude;
			else this->tractProps->maxAmplitude[i] *= 0.999;
		}
	}
	
	this->lipOutput = this->R[this->tractProps->n - 1];
	
	//noses
    for (int j = 0; j < (extraNose ? 2 : 1); j++) {
        this->noses[j].noseJunctionOutputL[this->tractProps->noseProps[j].length] = this->noses[j].noseR[this->tractProps->noseProps[j].length - 1] * this->lipReflection;
        
        for (int i=1; i < this->tractProps->noseProps[j].length; i++)
        {
            int w = this->noses[j].noseReflection[i] * (this->noses[j].noseR[i - 1] + this->noses[j].noseL[i]);
            this->noses[j].noseJunctionOutputR[i] = this->noses[j].noseR[i - 1] - w;
            this->noses[j].noseJunctionOutputL[i] = this->noses[j].noseL[i] + w;
        }
        
        for (int i = 0; i < this->tractProps->noseProps[j].length; i++)
        {
            this->noses[j].noseR[i] = this->noses[j].noseJunctionOutputR[i] * this->fade;
            this->noses[j].noseL[i] = this->noses[j].noseJunctionOutputL[i + 1] * this->fade;
            
            //this->noseR[i] = Math.clamp(this->noseJunctionOutputR[i] * this->fade, -1, 1);
            //this->noseL[i] = Math.clamp(this->noseJunctionOutputL[i+1] * this->fade, -1, 1);
            
            if (updateAmplitudes)
            {
                double amplitude = fabs(this->noses[j].noseR[i] + this->noses[j].noseL[i]);
                amplitude = clamp(amplitude, -5, 5);
                if (amplitude > this->tractProps->noseProps[j].maxAmplitude[i]) this->tractProps->noseProps[j].maxAmplitude[i] = amplitude;
                else this->tractProps->noseProps[j].maxAmplitude[i] *= 0.999;
            }
        }
        
        this->noseOutputs[j] = this->noses[j].noseR[this->tractProps->noseProps[j].length - 1];
    }
}

void Tract::updateTractLength(int tractLength){
	this->tractProps->n = tractLength;
}
