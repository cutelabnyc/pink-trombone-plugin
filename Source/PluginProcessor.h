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

//==============================================================================
/**
*/
class PinkTromboneAudioProcessor  : public AudioProcessor, public Timer
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
	void timerCallback() override;
	
	void setTractEnvelope(double constrictionEnvelopeMax, int attackLength, int decayLength);
	
	//=== Audio Parameters
	float tongueX = 0.0;
	float tongueY = 0.0;
	float constrictionX = 0.0;
	float constrictionY = 0.0;
	float fricativeIntensity = 0.0;
	bool muteAudio = false;
	bool constrictionActive = false;
	double constrictionMin = -2.0;
	double constrictionMax = 2.0;
	double constrictionEnvelopeMax = 0.0;
	float attackLength = 100;
	float decayLength = 100;
	int attackCounter;
	int decayCounter;
	double attackStep;
	double decayStep;
	float UIConstrictionY;
	
	t_tractProps *getTractProps();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkTromboneAudioProcessor)
	void envelope();
	t_tractProps tractProps;
	Glottis *glottis;
	Tract *tract;
	WhiteNoise *whiteNoise;
	Biquad *fricativeFilter;
	Biquad *aspirateFilter;
};
