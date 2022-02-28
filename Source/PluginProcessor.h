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
#include <map>


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
					float releaseTimeSeconds,
					float attackExponent,
					float decayExponent,
					float releaseExponent)
			: attack (attackTimeSeconds),
			  decay (decayTimeSeconds),
			  sustain (sustainLevel),
		      release (releaseTimeSeconds),
			  attackExp(attackExponent),
			  decayExp(decayExponent),
			  releaseExp(releaseExponent)
		{
		}

		float attack = 0.1f, decay = 0.1f, sustain = 1.0f, release = 0.1f, attackExp=1.0f, decayExp=1.0f, releaseExp=1.0f;
	};

	void setParameters (const Parameters& newParameters)
	{
		// need to call setSampleRate() first!
		jassert (sampleRate > 0.0);

		parameters = newParameters;
		recalculateRates();
	}
	
	bool isActive() const noexcept  { return state != State::idle; }

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
		scaledEnvelopeVal = 0.0f;
		state = State::idle;
	}
	
	void noteOn() noexcept
	{
		if (attackRate > 0.0f)
		{
			if (envelopeVal != 0.0) envelopeVal = scaledEnvelopeVal;
			state = State::attack;
		}
		else if (decayRate > 0.0f)
		{
			envelopeVal = 1.0f;
			scaledEnvelopeVal = 1.0f;
			state = State::decay;
		}
		else
		{
			envelopeVal = 1.0f;
			scaledEnvelopeVal = parameters.sustain;
			state = State::sustain;
		}
	}
	
	void noteOff() noexcept
	{
		{
			if (parameters.release > 0.0f)
			{
				releaseRate = (float) (scaledEnvelopeVal / (parameters.release * sampleRate));
				state = State::release;
			}
			else
			{
				reset();
			}
		}
	}

	//==============================================================================
	float getNextSample() noexcept
	{
		
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
			
			if (parameters.attackExp >= 0) scaledEnvelopeVal = powf(envelopeVal, parameters.attackExp);
			else scaledEnvelopeVal = powf(envelopeVal, (-1/parameters.attackExp));
		}
		
		else if (state == State::decay)
		{
			envelopeVal -= decayRate;
			
			if (parameters.decayExp >= 0) scaledEnvelopeVal = powf(envelopeVal, parameters.decayExp);
			else {
				if (envelopeVal >= 0) scaledEnvelopeVal = powf(envelopeVal, (-1/parameters.decayExp));
				else scaledEnvelopeVal = 0;
			}
			
			scaledEnvelopeVal = parameters.sustain + (1 - parameters.sustain)*scaledEnvelopeVal;

			if (scaledEnvelopeVal <= parameters.sustain)
			{
				envelopeVal = parameters.sustain;
				scaledEnvelopeVal = parameters.sustain;
				goToNextState();
			}
		}
		
		else if (state == State::sustain)
		{
			envelopeVal = 1.0;
			scaledEnvelopeVal = parameters.sustain;
		}
		
		else if (state == State::release)
		{
			envelopeVal -= releaseRate;
			
			if (parameters.releaseExp >= 0) scaledEnvelopeVal = powf(envelopeVal, parameters.releaseExp);
			else {
				if (envelopeVal >= 0) scaledEnvelopeVal = powf(envelopeVal, (-1/parameters.releaseExp));
				else scaledEnvelopeVal = 0;
			}
			
			scaledEnvelopeVal = parameters.sustain * scaledEnvelopeVal;

			if (envelopeVal <= 0.0f)
				goToNextState();
		}

		return scaledEnvelopeVal;
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
		releaseRate = getRate (1.0f, parameters.release, sampleRate);

		if ((state == State::attack && attackRate <= 0.0f)
			|| (state == State::decay && (decayRate <= 0.0f || envelopeVal <= 0.0))
			|| (state == State::release && releaseRate <= 0.0f))
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
		else if (state == State::release)
			reset();
	}

	//==============================================================================
	enum class State { idle, attack, decay, sustain, release };

	State state = State::idle;
	Parameters parameters;

	double sampleRate = 44100.0;
	float envelopeVal = 0.0f, scaledEnvelopeVal = 0.0f, attackRate = 0.0f, decayRate = 0.0f, releaseRate = 0.0f;
};


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
	bool envelope = false;
	bool voicing;
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
