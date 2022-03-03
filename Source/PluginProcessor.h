/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Glottis.hpp"
#include "Tract.hpp"
#include "WhiteNoise.hpp"
#include "Biquad.hpp"
#include "PinkTromboneADSR.hpp"
#include <math.h>
#include <map>





//==============================================================================
/**
*/
class PinkTromboneAudioProcessor  : public AudioProcessor, public MPEInstrument::Listener
{
public:
    //==============================================================================
    PinkTromboneAudioProcessor();
    ~PinkTromboneAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
	void noteAdded (MPENote newNote) override;
	void noteReleased (MPENote finishedNote) override;

	
	//=== Audio Parameters
	float fricativeIntensity = 0.0;
	bool muteAudio = false;
	double constrictionMin = -2.0;
	double constrictionMax = 2.0;
	double VOT;
	double sampleRate;
	int voicingCounter = 0;
	int numVoices = 11;
	bool noteOn;
	bool noteOff;
	bool envelope = false;
	bool breath = false;
	
	bool tongueXMod;
	bool tongueYMod;
	bool constrictionXMod;
	bool constrictionYMod;
	
	float tongueXModulation;
	float tongueYModulation;
	float constrictionXModulation;
	float constrictionYModulation;
	
	float tongueXModVal;
	float tongueYModVal;
	float constrictionXModVal;
	float constrictionYModVal;
	
	float restTongueX;
	float restTongueY;
	float restConstrictionX;
	float restConstrictionY;
	
	AudioParameterFloat* tongueX;
	AudioParameterFloat* tongueY;
	AudioParameterFloat* constrictionX;
	AudioParameterFloat* constrictionY;
	
	PinkTromboneADSR adsr;
	PinkTromboneADSR::Parameters adsrParams;
	
	t_tractProps *getTractProps();

private:
	MPEInstrument instrument;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkTromboneAudioProcessor)
	void applyEnvelope(float sampleVal);
	void applyVoicing();
	
	std::map<uint16, Glottis*> glottisMap;
	t_tractProps tractProps;
	Glottis *glottis;
	Glottis **glottises = new Glottis *[numVoices];
	Tract *tract;
	WhiteNoise *whiteNoise;
	Biquad *fricativeFilter;
	Biquad *aspirateFilter;
};
