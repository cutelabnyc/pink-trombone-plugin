//
//  Tract.hpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 9/5/19.
//

#ifndef Tract_hpp
#define Tract_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

struct t_transient;

class Tract {
public:
	Tract(double sampleRate, double blockSize);
	~Tract();
	void runStep(double glottalOutput, double turbulenceNoise, double lambda);
	void finishBlock();
	double lipOutput;
	double noseOutput;
	
private:
	void init();
	void addTransient(int position);
	void addTurbulenceNoise(double turbulenceNoise);
	void addTurbulenceNoiseAtIndex(double turbulenceNoise, int index, double diameter);
	void calculateReflections();
	void calculateNoseReflections();
	void processTransients();
	void reshapeTract(double deltaTime);
	
	double sampleRate, blockTime;
	int n;
	int bladeStart;
	int tipStart;
	int lipStart;
	double glottalReflection;
	double lipReflection;
	int lastObstruction;
	double fade;
	double movementSpeed;
	double velumTarget;
	t_transient *transients;
	int transientCount;
	
	double *diameter;
	double *restDiameter;
	double *targetDiameter;
	double *newDiameter;
	
	double *R;
	double *L;
	double *reflection;
	double *newReflection;
	double *junctionOutputR;
	double *junctionOutputL;
	double *A;
	double *maxAmplitude;
	
	int noseLength;
	int noseStart;
	double *noseR;
	double *noseL;
	double *noseJunctionOutputR;
	double *noseJunctionOutputL;
	double *noseReflection;
	double *noseDiameter;
	double *noseA;
	double *noseMaxAmplitude;
	
	double reflectionLeft, reflectionRight, reflectionNose;
	double newReflectionLeft, newReflectionRight, newReflectionNose;
};

#endif /* Tract_hpp */
