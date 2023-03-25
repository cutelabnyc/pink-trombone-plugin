//
//  Glottis.hpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 8/28/19.
//

#ifndef Glottis_hpp
#define Glottis_hpp

#include <stdio.h>
#include "JuceHeader.h"
#include "SimplexNoise.hpp"

class Glottis {
public:
	Glottis(double sampleRate);
	~Glottis();
	double runStep(double lambda, double noiseSource);
	void finishBlock();
	double getNoiseModulator();
	void setFrequency(double midiNoteInHz);
	void setVoicing(bool voice);
	void setActive(bool active);
	void setBreathFactor(double breathFactor);
	void setSizeOffset(double sizeOffset);
	bool isActive = false;
	bool alwaysVoice;
	
private:
	void setupWaveform(double lambda);
	double normalizedLFWaveform(double t);
	void calculateFrequencyFluctuations();
	SimplexNoise noise;
	double frequency, oldFrequency, newFrequency, UIFrequency, smoothFrequency;
	double sampleRate;
	double timeInWaveform;
	double oldTenseness, newTenseness, UITenseness;
	double waveformLength;
	double Rd;
	double alpha;
	double E0;
	double epsilon;
	double shift;
	double Delta;
	double Te;
	double omega;
	double totalTime;
	double intensity, loudness;
	double vibratoAmount;
	double vibratoFrequency;
	double tenseness;
	double breathFactor = 1;
	double sizeOffset = 0;
	bool autoWobble;
	bool isTouched;
};

#endif /* Glottis_hpp */
