//
//  Glottis.hpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 8/28/19.
//

#ifndef Glottis_hpp
#define Glottis_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class Glottis {
public:
	Glottis(double sampleRate);
	~Glottis();
	double runStep(double lambda, double noiseSource);
	void finishBlock();
	double getNoiseModulator();
	void setFrequency(double midiNoteInHz);
	void setVoicing(bool voice);
	
private:
	void setupWaveform(double lambda);
	double normalizedLFWaveform(double t);
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
	bool autoWobble;
	bool isTouched;
	bool alwaysVoice;
};

#endif /* Glottis_hpp */
