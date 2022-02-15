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
#include <math.h>


class PinkTromboneADSR
// This class is called ADSR but does not implement release as it is not relevant to the pink trombone model
{
public:
	//==============================================================================
	PinkTromboneADSR()
	{
		recalculateRates();
	}

	//==============================================================================
	struct Parameters
	{
		Parameters() = default;

		Parameters (float attackTimeSeconds,
					float decayTimeSeconds,
					float sustainLevel,
					float decayExponent)
			: attack (attackTimeSeconds),
			  decay (decayTimeSeconds),
			  sustain (sustainLevel),
			  decayExp(decayExponent)
		{
		}

		float attack = 0.1f, decay = 0.1f, sustain = 1.0f, decayExp=1.0f;
	};

	void setParameters (const Parameters& newParameters)
	{
		// need to call setSampleRate() first!
		jassert (sampleRate > 0.0);

		parameters = newParameters;
		recalculateRates();
	}

	//==============================================================================
	void setSampleRate (double newSampleRate) noexcept
	{
		jassert (newSampleRate > 0.0);
		sampleRate = newSampleRate;
	}

	//==============================================================================

	void reset() noexcept
	{
		envelopeVal = 0.0f;
		state = State::idle;
	}
	
	void noteOn() noexcept
	{
		if (attackRate > 0.0f)
		{
			state = State::attack;
		}
		else if (decayRate > 0.0f)
		{
			envelopeVal = 1.0f;
			state = State::decay;
		}
		else
		{
			envelopeVal = parameters.sustain;
			state = State::sustain;
		}
	}
	
	void noteOff() noexcept
	{
		if (state != State::idle)
			reset();
	}

	//==============================================================================
	float getNextSample() noexcept
	{
		float scaledEnvelopeVal;
		
		if (state == State::idle)
			return 0.0f;

		if (state == State::attack)
		{
			envelopeVal += attackRate;

			if (envelopeVal >= 1.0f)
			{
				envelopeVal = 1.0f;
				goToNextState();
			}
		}
		else if (state == State::decay)
		{
			envelopeVal -= decayRate;

			if (envelopeVal <= parameters.sustain)
			{
				envelopeVal = parameters.sustain;
				goToNextState();
			}
			if (parameters.decayExp >= 0)
				scaledEnvelopeVal = powf(envelopeVal, parameters.decayExp);
			else
				scaledEnvelopeVal = powf(envelopeVal, (-1/parameters.decayExp));
			scaledEnvelopeVal *= (1-parameters.sustain);
			return scaledEnvelopeVal;
		}
		else if (state == State::sustain)
		{
			envelopeVal = parameters.sustain;
		}

		return envelopeVal;
	}

private:
	//==============================================================================
	void recalculateRates() noexcept
	{
		auto getRate = [] (float distance, float timeInSeconds, double sr)
		{
			return timeInSeconds > 0.0f ? (float) (distance / (timeInSeconds * sr)) : -1.0f;
		};

		attackRate  = getRate (1.0f, parameters.attack, sampleRate);
		decayRate   = getRate (1.0f, parameters.decay, sampleRate);

		if ((state == State::attack && attackRate <= 0.0f)
			|| (state == State::decay && (decayRate <= 0.0f || envelopeVal <= parameters.sustain)))
		{
			goToNextState();
		}
	}

	void goToNextState() noexcept
	{
		if (state == State::attack)
			state = (decayRate > 0.0f ? State::decay : State::sustain);
		else if (state == State::decay)
			state = State::sustain;
	}

	//==============================================================================
	enum class State { idle, attack, decay, sustain };

	State state = State::idle;
	Parameters parameters;

	double sampleRate = 44100.0;
	float envelopeVal = 0.0f, attackRate = 0.0f, decayRate = 0.0f;
};


//==============================================================================
/**
*/
class PinkTromboneAudioProcessor  : public AudioProcessor
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

	
	//=== Audio Parameters
	float tongueX = 0.0;
	float tongueY = 0.0;
	float constrictionX = 0.0;
	float constrictionY = 0.0;
	float fricativeIntensity = 0.0;
	bool muteAudio = false;
	double constrictionMin = -2.0;
	double constrictionMax = 2.0;
	double constrictionEnvelopeMax = 0.57;
	float decayLength = 100;
	float restConstrictionY;
	double VOT;
	int voicingCounter = 0;
	bool envelope = false;
	bool voicing;
	double sampleRate;
	
	
	PinkTromboneADSR adsr;
	PinkTromboneADSR::Parameters adsrParams;
	
	t_tractProps *getTractProps();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkTromboneAudioProcessor)
	void applyConstrictionEnvelope(float sampleVal);
	void setVoicingOn();
	void applyVoicing();
	t_tractProps tractProps;
	Glottis *glottis;
	Tract *tract;
	WhiteNoise *whiteNoise;
	Biquad *fricativeFilter;
	Biquad *aspirateFilter;
};
