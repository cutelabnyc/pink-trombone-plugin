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
#include "Glottis.hpp"

struct t_transient;

typedef struct t_tractProps {
	int n;
	int lipStart;
	int bladeStart;
	int tipStart;
	int noseStart;
	int noseLength;
	double noseOffset;
	double tongueIndex;
	double tongueDiameter;
	double *noseDiameter;
	double *tractDiameter;
    double *maxAmplitude;
    double *noseMaxAmplitude;
} t_tractProps;

void initializeTractProps(t_tractProps *props, int n);

class Tract {
public:
	Tract(double sampleRate, double blockSize, t_tractProps *p);
	~Tract();
	void runStep(double glottalOutput, double turbulenceNoise, double lambda, Glottis *glottis);
	void finishBlock();
	void setRestDiameter(double tongueIndex, double tongueDiameter);
	void setConstriction(double cindex, double cdiam, double fricativeIntensity);
	double lipOutput;
	double noseOutput;
	
	long getTractIndexCount();
	long tongueIndexLowerBound();
	long tongueIndexUpperBound();
	
private:
	void init();
	void addTransient(int position);
	void addTurbulenceNoise(double turbulenceNoise, Glottis *glottis);
	void addTurbulenceNoiseAtIndex(double turbulenceNoise, double index, double diameter, Glottis *glottis);
	void calculateReflections();
	void calculateNoseReflections();
	void processTransients();
	void reshapeTract(double deltaTime);
	
	double sampleRate, blockTime;
	t_tractProps *tractProps;
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
	
	double *noseR;
	double *noseL;
	double *noseJunctionOutputR;
	double *noseJunctionOutputL;
	double *noseReflection;
	double *noseDiameter;
	double *noseA;
//	double *noseMaxAmplitude;
	
	double reflectionLeft, reflectionRight, reflectionNose;
	double newReflectionLeft, newReflectionRight, newReflectionNose;
	
	double constrictionIndex;
	double constrictionDiameter;
	double fricativeIntensity = 0.0;
};

#endif /* Tract_hpp */
