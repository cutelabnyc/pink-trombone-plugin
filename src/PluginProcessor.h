/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Glottis.hpp"
#include "Tract.hpp"
#include "WhiteNoise.hpp"
#include "Biquad.hpp"
#include "PinkTromboneADSR.hpp"
#include "SimpleLFO.hpp"
#include "Modulatable.h"
#include <math.h>
#include <map>

#define NOSE_LENGTH_MIN (5)
#define NOSE_LENGTH_MAX (44)
#define NOSE_ATTACHMENT_MIN (3)
#define NOSE_ATTACHMENT_MAX (40)
#define RESTART_CONSTRICTION_TIME_SEC (0.05)
#define NUM_VOICES (11)

//==============================================================================
/**
*/
class PinkTromboneAudioProcessor  : public AudioProcessor, public MPEInstrument::Listener
{
public:
    
    //==============================================================================
    PinkTromboneAudioProcessor();
    ~PinkTromboneAudioProcessor() override;
    
    //==============================================================================
    // Envelope
    static juce::String initial;
    static juce::String peak;
    static juce::String attack;
    static juce::String decay;
    static juce::String sustain;
    static juce::String release;
    static juce::String envModConstrictionX;
    static juce::String envModConstrictionY;
    static juce::String envModTongueX;
    static juce::String envModTongueY;
    static juce::String envModPitch;

    // LFO
    static juce::String lfoShape;
    static juce::String lfoRate;
    static juce::String lfoMode;
    static juce::String lfoModConstrictionX;
    static juce::String lfoModConstrictionY;
    static juce::String lfoModTongueX;
    static juce::String lfoModTongueY;
    static juce::String lfoModPitch;

    // Vocal
    static juce::String pitchAdjust;

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
	
	void openNose(bool openNose);
	void setNoseLength(float noseLength, int index);
	void setNoseAttachment(float noseAttachment, int index);
	void setExtraNose(bool extraNose);
	void updateBreathFactor(double breathFactor);
	void updateSize(double sizeFactor);
	void setAutoVelum(bool autoVelum);
	void setNoseMode(int selectedId);
    void setIsSettingConstriction(bool tf);
    AudioProcessorValueTreeState& getParametersTree();

    //==============================================================================
    CriticalSection listenerLock;
    void addLFOVisualiser(AudioVisualiserComponent *component);
    void removeLFOVisualiser(AudioVisualiserComponent *component);
	
	//=== Audio Parameters
	float fricativeIntensity = 0.0;
	bool muteAudio = false;
	double constrictionMin = -2.0;
	double constrictionMax = 2.0;
	double sampleRate;
	int voicingCounter = 0;
	int numVoices = NUM_VOICES;
	bool noteOn;
	bool noteOff;
	bool breath = false;
	bool extraNose;
	
	AudioParameterFloat* tongueX;
	AudioParameterFloat* tongueY;
	AudioParameterFloat* constrictionX;
	AudioParameterFloat* constrictionY;
    
    ModulatedAudioParameter* tongueXMod;
    ModulatedAudioParameter* tongueYMod;
    ModulatedAudioParameter* constrictionXMod;
    ModulatedAudioParameter* constrictionYMod;
    ModulatedAudioParameter* pitchMod;
	
	PinkTromboneADSR adsr;
	PinkTromboneADSR::Parameters adsrParams;
	
	t_tractProps *getTractProps();

private:
	MPEInstrument instrument;
    AudioProcessorValueTreeState parameters;
    SimpleLFO _modLFO;
    juce::dsp::LookupTableTransform<float> _mtofLookupTable;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkTromboneAudioProcessor)
	void applyVoicing();

    typedef struct GlottisParams {
        float rootMIDIPitch;
        Glottis *glottis;
    } GlottisParams;
	
	std::map<uint16, GlottisParams*> glottisMap;
	t_tractProps tractProps;
    GlottisParams glottisParams[NUM_VOICES];
	Tract *tract;
	WhiteNoise *whiteNoise;
	Biquad *fricativeFilter;
	Biquad *aspirateFilter;
    bool isSettingConstriction = false;
    bool restartConstrictionEnvelope = false;
    uint32_t restartConstrictionSampleCount = 0;
    uint32_t restartConstrictionSampleMax = 1;
    Array<AudioVisualiserComponent *> _lfoVisualisers;
};
