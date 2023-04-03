/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "util.h"
#include <map>

juce::String PinkTromboneAudioProcessor::initial                ("initial");
juce::String PinkTromboneAudioProcessor::peak                   ("peak");
juce::String PinkTromboneAudioProcessor::attack                 ("attack");
juce::String PinkTromboneAudioProcessor::decay                  ("decay");
juce::String PinkTromboneAudioProcessor::sustain                ("sustain");
juce::String PinkTromboneAudioProcessor::release                ("release");
juce::String PinkTromboneAudioProcessor::envModConstrictionX    ("envModConstrictionX");
juce::String PinkTromboneAudioProcessor::envModConstrictionY    ("envModConstrictionY");
juce::String PinkTromboneAudioProcessor::envModTongueX          ("envModTongueX");
juce::String PinkTromboneAudioProcessor::envModTongueY          ("envModTongueY");
juce::String PinkTromboneAudioProcessor::envModPitch            ("envModPitch");
juce::String PinkTromboneAudioProcessor::lfoShape               ("lfoShape");
juce::String PinkTromboneAudioProcessor::lfoRate                ("lfoRate");
juce::String PinkTromboneAudioProcessor::lfoMode                ("lfoMode");
juce::String PinkTromboneAudioProcessor::lfoModConstrictionX    ("lfoModConstrictionX");
juce::String PinkTromboneAudioProcessor::lfoModConstrictionY    ("lfoModConstrictionY");
juce::String PinkTromboneAudioProcessor::lfoModTongueX          ("lfoModTongueX");
juce::String PinkTromboneAudioProcessor::lfoModTongueY          ("lfoModTongueY");
juce::String PinkTromboneAudioProcessor::lfoModPitch            ("lfoModPitch");
juce::String PinkTromboneAudioProcessor::pitchAdjust            ("pitchAdjust");


static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::AudioProcessorParameterGroup>> params;
    
    // ADSR
    {
        auto initial = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::initial,
                                                                  TRANS ("Initial"),
                                                                  juce::NormalisableRange<float> (-70.0f, 0.0f, 0.1f),
                                                                  -70.0f,
                                                                  TRANS ("Initial value"),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  [](float value, int) {return juce::String (value) + " dB";},
                                                                  [](juce::String text) {return (text.getFloatValue());}
                                                                  );
        auto peak = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::peak,
                                                                  TRANS ("Peak"),
                                                                  juce::NormalisableRange<float> (-70.0f, 0.0f, 0.1f),
                                                                  0.0f,
                                                                  TRANS ("Peak value"),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  [](float value, int) {return juce::String (value) + " dB";},
                                                                  [](juce::String text) {return (text.getFloatValue());}
                                                                  );
        auto attack = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::attack,
                                                                  TRANS ("Attack"),
                                                                  juce::NormalisableRange<float> (0.0f, 1000.0f, 0.1f, 1.0f/3.0f),
                                                                  5.0f,
                                                                  TRANS ("Attack time"),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  nullptr,
                                                                  nullptr
                                                                  );
        auto decay = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::decay,
                                                                  TRANS ("Decay"),
                                                                  juce::NormalisableRange<float> (1.0f, 1000.0f, 0.1f, 1.0f/3.0f),
                                                                  100.0f,
                                                                  TRANS ("Decay time"),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  nullptr,
                                                                  nullptr
                                                                  );
        auto sustain = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::sustain,
                                                                  TRANS ("Sustain"),
                                                                  juce::NormalisableRange<float> (-70.0f, 0.0f, 0.1f),
                                                                  -20.0f,
                                                                  TRANS ("Sustain"),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  [](float value, int) {return juce::String (value) + " dB";},
                                                                  [](juce::String text) {return (text.getFloatValue());}
                                                                  );
        auto release = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::release,
                                                                  TRANS ("Release"),
                                                                  juce::NormalisableRange<float> (1.0f, 1000.0f, 0.1f, 3.0f),
                                                                  100.0f,
                                                                  TRANS ("Release time"),
                                                                  juce::AudioProcessorParameter::genericParameter,
                                                                  nullptr,
                                                                  nullptr
                                                                  );
        auto constrictionXEnvMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::envModConstrictionX,
                                                                                TRANS ("Envelope Mod Constriction X"),
                                                                                juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
                                                                                0.0f,
                                                                                TRANS ("Envelope Modulation Constriction X"),
                                                                                juce::AudioProcessorParameter::genericParameter,
                                                                                nullptr,
                                                                                nullptr
                                                                                );
        auto constrictionYEnvMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::envModConstrictionY,
                                                                                TRANS ("Envelope Mod Constriction Y"),
                                                                                juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
                                                                                0.0f,
                                                                                TRANS ("Envelope Modulation Constriction Y"),
                                                                                juce::AudioProcessorParameter::genericParameter,
                                                                                nullptr,
                                                                                nullptr
                                                                                );
        auto tongueXEnvMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::envModTongueX,
                                                                                TRANS ("Envelope Mod Tongue X"),
                                                                                juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
                                                                                0.0f,
                                                                                TRANS ("Envelope Modulation Tongue X"),
                                                                                juce::AudioProcessorParameter::genericParameter,
                                                                                nullptr,
                                                                                nullptr
                                                                                );
        auto tongueYEnvMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::envModTongueY,
                                                                                TRANS ("Envelope Mod Tongue Y"),
                                                                                juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
                                                                                0.0f,
                                                                                TRANS ("Envelope Modulation Tongue Y"),
                                                                                juce::AudioProcessorParameter::genericParameter,
                                                                                nullptr,
                                                                                nullptr
                                                                                );
        auto pitchEnvMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::envModPitch,
                                                                                TRANS ("Envelope Mod Pitch"),
                                                                                juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
                                                                                0.0f,
                                                                                TRANS ("Envelope Modulation Pitch"),
                                                                                juce::AudioProcessorParameter::genericParameter,
                                                                                nullptr,
                                                                                nullptr
                                                                                );

        auto group = std::make_unique<juce::AudioProcessorParameterGroup> ("envelope",
                                                                           TRANS ("Envelope"),
                                                                           "|",
                                                                           std::move(initial),
                                                                           std::move(peak),
                                                                           std::move(attack),
                                                                           std::move(decay),
                                                                           std::move(sustain),
                                                                           std::move(release),
                                                                           std::move(constrictionXEnvMod),
                                                                           std::move(constrictionYEnvMod),
                                                                           std::move(tongueXEnvMod),
                                                                           std::move(tongueYEnvMod),
                                                                           std::move(pitchEnvMod));
        params.push_back (std::move (group));
    }

    // LFO
    {
        auto shape = std::make_unique<juce::AudioParameterChoice> (
            PinkTromboneAudioProcessor::lfoShape,
            TRANS ("Shape"),
            StringArray { "Sine", "Slope", "Square", "Triangle", "Noise" },
            1
        );
        auto rate = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::lfoRate,
            TRANS ("Rate"),
            juce::NormalisableRange<float> (0.001, 10.0f, 0.01f, 1.0f / 3.0f),
            1.0f,
            TRANS ("LFORate"),
            juce::AudioProcessorParameter::genericParameter,
            nullptr,
            nullptr
        );
        auto mode = std::make_unique<juce::AudioParameterChoice> (
            PinkTromboneAudioProcessor::lfoMode,
            TRANS ("Mode"),
            StringArray { "Free", "Locked" },
            1
        );
        auto constrictionXLFOMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::lfoModConstrictionX,
            TRANS ("LFO Mod Constriction X"),
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
            0.0f,
            TRANS ("LFO Modulation Constriction X"),
            juce::AudioProcessorParameter::genericParameter,
            nullptr,
            nullptr
        );
        auto constrictionYLFOMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::lfoModConstrictionY,
            TRANS ("LFO Mod Constriction Y"),
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
            0.0f,
            TRANS ("LFO Modulation Constriction Y"),
            juce::AudioProcessorParameter::genericParameter,
            nullptr,
            nullptr
        );
        auto tongueXLFOMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::lfoModTongueX,
            TRANS ("LFO Mod Tongue X"),
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
            0.0f,
            TRANS ("LFO Modulation Tongue X"),
            juce::AudioProcessorParameter::genericParameter,
            nullptr,
            nullptr
        );
        auto tongueYLFOMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::lfoModTongueY,
            TRANS ("LFO Mod Tongue Y"),
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
            0.0f,
            TRANS ("LFO Modulation Tongue Y"),
            juce::AudioProcessorParameter::genericParameter,
            nullptr,
            nullptr
        );
        auto pitchLFOMod = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::lfoModPitch,
            TRANS ("LFO Mod Pitch"),
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
            0.0f,
            TRANS ("LFO Modulation Pitch"),
            juce::AudioProcessorParameter::genericParameter,
            nullptr,
            nullptr
        );

        auto group = std::make_unique<juce::AudioProcessorParameterGroup> ("lfo",
            TRANS ("LFO"),
            "|",
            std::move(shape),
            std::move(rate),
            std::move(mode),
            std::move(constrictionXLFOMod),
            std::move(constrictionYLFOMod),
            std::move(tongueXLFOMod),
            std::move(tongueYLFOMod),
            std::move(pitchLFOMod)
        );
        params.push_back (std::move (group));
    }

    // Vocal
    {
        auto pitchAdjust = std::make_unique<juce::AudioParameterFloat> (PinkTromboneAudioProcessor::pitchAdjust,
            TRANS ("Pitch Adjust"),
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
            0.0f,
            TRANS ("Pitch Adjust"),
            juce::AudioProcessorParameter::genericParameter,
            nullptr,
            nullptr
        );

        auto group = std::make_unique<juce::AudioProcessorParameterGroup> ("vocal",
            TRANS ("Vocal"),
            "|",
            std::move(pitchAdjust)
        );

        params.push_back (std::move (group));
    }
    
    return { params.begin(), params.end() };
}

//==============================================================================
PinkTromboneAudioProcessor::PinkTromboneAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
    #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
    #endif
                       )
    , parameters(*this, nullptr, juce::Identifier("PinkTrombone"), createParameterLayout())
    , _modLFO(parameters, {
        PinkTromboneAudioProcessor::lfoRate,
        PinkTromboneAudioProcessor::lfoShape,
        PinkTromboneAudioProcessor::lfoMode
    })
    , _mtofLookupTable([] (float x) { return 440.0f * std::powf (2.0f, (x - 69.0f) / 12.0f); }, 0.0f, 127.0f, 5096)
#endif
{
	addParameter (tongueX = new AudioParameterFloat ("tonguex", // parameter ID
													 	"Tongue X", // parameter name
													 	0.0f,   // minimum value
													 	1.0f,   // maximum value
													 	1.0f)); // default value
	addParameter (tongueY = new AudioParameterFloat ("tonguey", // parameter ID
													 "Tongue Y", // parameter name
													 0.0f,   // minimum value
													 1.0f,   // maximum value
													 1.0f)); // default value
	addParameter (constrictionX = new AudioParameterFloat ("constrictionx", // parameter ID
													 "Constriction X", // parameter name
													 0.0f,   // minimum value
													 1.0f,   // maximum value
													 1.0f)); // default value
	addParameter (constrictionY = new AudioParameterFloat ("constrictiony", // parameter ID
													 "Constriction Y", // parameter name
													 0.0f,   // minimum value
													 1.0f,   // maximum value
													 1.0f)); // default value

    tongueXMod = new ModulatedAudioParameter(tongueX);
    tongueYMod = new ModulatedAudioParameter(tongueY);
    constrictionXMod = new ModulatedAudioParameter(constrictionX);
    constrictionYMod = new ModulatedAudioParameter(constrictionY);
    pitchMod = new ModulatedAudioParameter(parameters.getParameter(PinkTromboneAudioProcessor::pitchAdjust));
    
    // ADSR Modulation
    tongueXMod->appendModulationStage(&adsr, parameters.getParameter(PinkTromboneAudioProcessor::envModTongueX));
    tongueYMod->appendModulationStage(&adsr, parameters.getParameter(PinkTromboneAudioProcessor::envModTongueY));
    constrictionXMod->appendModulationStage(&adsr, parameters.getParameter(PinkTromboneAudioProcessor::envModConstrictionX));
    constrictionYMod->appendModulationStage(&adsr, parameters.getParameter(PinkTromboneAudioProcessor::envModConstrictionY));
    pitchMod->appendModulationStage(&adsr, parameters.getParameter(PinkTromboneAudioProcessor::envModPitch));

    // LFO Modulation
    tongueXMod->appendModulationStage(&_modLFO, parameters.getParameter(PinkTromboneAudioProcessor::lfoModTongueX));
    tongueYMod->appendModulationStage(&_modLFO, parameters.getParameter(PinkTromboneAudioProcessor::lfoModTongueY));
    constrictionXMod->appendModulationStage(&_modLFO, parameters.getParameter(PinkTromboneAudioProcessor::lfoModConstrictionX));
    constrictionYMod->appendModulationStage(&_modLFO, parameters.getParameter(PinkTromboneAudioProcessor::lfoModConstrictionY));
    pitchMod->appendModulationStage(&_modLFO, parameters.getParameter(PinkTromboneAudioProcessor::lfoModPitch));
	
	initializeTractProps(&this->tractProps, 44);
	
	instrument.enableLegacyMode (24);
	instrument.addListener(this);
}

PinkTromboneAudioProcessor::~PinkTromboneAudioProcessor()
{
    delete tongueXMod;
    delete tongueYMod;
    delete constrictionXMod;
    delete constrictionYMod;

    for (int i = 0; i < NUM_VOICES; i++) {
        delete glottisParams[i].glottis;
    }
}

//==============================================================================
const String PinkTromboneAudioProcessor::getName() const
{
#if defined(JucePlugin_Name)
    return JucePlugin_Name;
#elif defined(JUCE_APPLICATION_NAME_STRING)
    return JUCE_APPLICATION_NAME_STRING;
#else
    return "JUCE Application";
#endif
}

bool PinkTromboneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PinkTromboneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PinkTromboneAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PinkTromboneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PinkTromboneAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PinkTromboneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PinkTromboneAudioProcessor::setCurrentProgram (int index)
{
}

const String PinkTromboneAudioProcessor::getProgramName (int index)
{
    return {};
}

void PinkTromboneAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PinkTromboneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	this->sampleRate = sampleRate;
	this->adsr.setSampleRate(sampleRate);
    _modLFO.setSampleRate(sampleRate);
    restartConstrictionSampleMax = RESTART_CONSTRICTION_TIME_SEC * sampleRate;
	
	for (int i=0; i<this->numVoices; i++)
	{
        this->glottisParams[i].glottis = new Glottis(sampleRate);
	}
	
	this->tract = new Tract(sampleRate, samplesPerBlock, &this->tractProps);
	this->whiteNoise = new WhiteNoise(sampleRate * 2.0);
	this->aspirateFilter = new Biquad(sampleRate);
	this->fricativeFilter = new Biquad(sampleRate);
	
	this->aspirateFilter->setGain(1.0);
	this->aspirateFilter->setQ(0.5);
	this->aspirateFilter->setFrequency(500);
	this->fricativeFilter->setGain(1.0);
	this->fricativeFilter->setQ(0.5);
	this->fricativeFilter->setFrequency(1000);
}

void PinkTromboneAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PinkTromboneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PinkTromboneAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	auto N = buffer.getNumSamples();
	
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Update the ADSR with our parameters
    RangedAudioParameter *param = parameters.getParameter(PinkTromboneAudioProcessor::initial);
    adsrParams.initial = param->getValue();
    param = parameters.getParameter(PinkTromboneAudioProcessor::attack);
    adsrParams.attack = param->convertFrom0to1(param->getValue());
    param = parameters.getParameter(PinkTromboneAudioProcessor::peak);
    adsrParams.peak = param->getValue();
    param = parameters.getParameter(PinkTromboneAudioProcessor::decay);
    adsrParams.decay = param->convertFrom0to1(param->getValue());
    param = parameters.getParameter(PinkTromboneAudioProcessor::sustain);
    adsrParams.sustain = param->getValue();
    param = parameters.getParameter(PinkTromboneAudioProcessor::release);
    adsrParams.release = param->convertFrom0to1(param->getValue());
	adsr.setParameters(adsrParams);
	
	for (auto meta : midiMessages) {
		auto currentMessage = meta.getMessage();
		instrument.processNextMidiEvent(currentMessage);
	}
	
	if(!adsr.isActive() && this->noteOff)
	{
		this->voicingCounter = 0;
		this->noteOff = false;
		for (int i=0; i<this->numVoices; i++)
		{
            glottisParams->glottis->setVoicing(false);
            glottisParams->glottis->setActive(false);
		}
	}

    // Apply pitch bend to all the glottises
    float bend = pitchMod->value();
    for (int i = 0; i < NUM_VOICES; i++) {
        GlottisParams p = glottisParams[i];
        if (p.glottis->isActive) {
            float frequency = _mtofLookupTable.processSample(bend + p.rootMIDIPitch);
            p.glottis->setFrequency(frequency);
        }
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	
	auto* channelData = buffer.getWritePointer (0);
	for (int j = 0; j < N; j++) {
		
		// Noise
		double purenoise;
		if (this->breath) purenoise = this->whiteNoise->runStep();
		else {
			if (this->noteOn) purenoise = this->whiteNoise->runStep();
			else purenoise = 0.0;
		}
		double asp = this->aspirateFilter->runStep(purenoise);
		double fri = this->fricativeFilter->runStep(purenoise);
		
		// Glottis
		double lambda1 = (double) j / (double) N;
		double lambda2 = ((double) j + 0.5) / (double) N;
		
		double glotSum = 0;
        double glotModulator = glottisParams[0].glottis->getNoiseModulator();
		for (int i=0; i<this->numVoices; i++)
		{
			double glot = glottisParams[i].glottis->runStep(lambda1, asp);
			glotSum += glot;
		}
        
        // Collect upsampled output from two different lambdas
		double vocalOutput = 0.0;
		this->tract->runStep(glotSum, fri, lambda1, glotModulator);
        vocalOutput += this->tract->lipOutput;
        for (int k = 0; k < (this->extraNose ? 2 : 1); k++) {
            vocalOutput += this->tract->noseOutputs[k];
        }
		
		this->tract->runStep(glotSum, fri, lambda2, glotModulator);
        vocalOutput += this->tract->lipOutput;
        for (int k = 0; k < (this->extraNose ? 2 : 1); k++) {
            vocalOutput += this->tract->noseOutputs[k];
        }


        adsr.advanceOneSample();
        _modLFO.advanceOneSample();
        float sample = _modLFO.value();
        for (auto i = 0; i < _lfoVisualisers.size(); i++) {
            _lfoVisualisers[i]->pushSample(&sample, 1);
        }

		this->applyVoicing();
		
		channelData[j] = vocalOutput * 0.125;
	}
	
	// Mono
	for (int channel = 1; channel < totalNumOutputChannels; ++channel) {
		auto *readChannel = buffer.getReadPointer(0);
		auto *writeChannel = buffer.getWritePointer(channel);
		for (int j = 0; j < N; j++) {
			writeChannel[j] = readChannel[j];
		}
	}
	
	double innerTongueControlRadius = 2.05;
	double outerTongueControlRadius = 3.5;
	
    double tongueIndex =
        this->tongueXMod->value() *
        ((double) (this->tract->tongueIndexUpperBound() - this->tract->tongueIndexLowerBound())) + this->tract->tongueIndexLowerBound();
    double tongueDiameter =
        this->tongueYMod->value() *
        (outerTongueControlRadius - innerTongueControlRadius) + innerTongueControlRadius;
	double constrictionIndex = this->constrictionXMod->value() * (double) this->tract->getTractIndexCount();
	double constrictionDiameter = (this->constrictionYMod->value()/2 + 0.5) * (this->constrictionMax - this->constrictionMin) + this->constrictionMin;
	//(constrictionY/2 + 0.5) is to adjust for diameter range since we are not implementing nasal cavity atm i.e. 0->1 UI range is really 0.5->1
	
	this->fricativeIntensity += 0.1; // TODO ex recto
	this->fricativeIntensity = minf(1.0, this->fricativeIntensity);
	
	this->tract->setRestDiameter(tongueIndex, tongueDiameter);
    
    // Only activate the constriction if there's a note on, or the mouse is down
    if (restartConstrictionEnvelope) {
        restartConstrictionEnvelope = false;
        restartConstrictionSampleCount = 0;
    }
    bool envelopeIsRestarting = (restartConstrictionSampleCount < restartConstrictionSampleMax);
    if (isSettingConstriction || (!glottisMap.empty() && !envelopeIsRestarting)) {
        this->tract->setConstriction(constrictionIndex, constrictionDiameter, this->fricativeIntensity);
    }
    restartConstrictionSampleCount += N;
	for (int i=0; i<this->numVoices; i++)
	{
        glottisParams[i].glottis->finishBlock();
	}
	
	this->tract->finishBlock();
	
	if (this->muteAudio) {
		for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
			auto *writeChannel = buffer.getWritePointer(channel);
			for (int j = 0; j < N; j++) {
				writeChannel[j] = 0.0;
			}
		}
	}
}

void PinkTromboneAudioProcessor::noteAdded(MPENote newNote)
{
	bool firstNote;
	this->noteOn = true;
    if(this->glottisMap.empty()) {
        firstNote = true;
        restartConstrictionSampleCount = restartConstrictionSampleMax + 1;
    } else {
        restartConstrictionEnvelope = true;
        restartConstrictionSampleCount = 0;
    }
	
	for (int i = 0; i < this->numVoices; i++)
	{
		if( !this->glottisParams[i].glottis->isActive)
		{
			this->glottisMap.emplace(newNote.noteID, &glottisParams[i]);
            glottisParams[i].rootMIDIPitch = newNote.initialNote;

            float bend = pitchMod->value();
            float frequency = _mtofLookupTable.processSample(newNote.initialNote + (bend * 2.0f - 1.0f));

			glottisParams[i].glottis->setFrequency(frequency);
			glottisParams[i].glottis->setActive(true);
			if (firstNote) {
				this->voicingCounter = 0;
			}
			break;
		}
	}
    adsr.noteOn();
}

void PinkTromboneAudioProcessor::noteReleased(MPENote finishedNote)
{
	std::map<uint16, GlottisParams*>::iterator glotOff = this->glottisMap.find(finishedNote.noteID);

    if (glotOff != glottisMap.end()) {
        Glottis *g = glotOff->second->glottis;
        g->setActive(false);
        g->setVoicing(false);
    }

	this->glottisMap.erase(finishedNote.noteID);
	
	if(this->glottisMap.empty()) {
		this->noteOn = false;
        this->noteOff = true;
        adsr.noteOff();
	}
}

void PinkTromboneAudioProcessor::applyVoicing()
{
	if (this->voicingCounter == 0)
	{
		std::map<uint16, GlottisParams*>::iterator it = glottisMap.begin();
		for (; it != glottisMap.end(); it++)
		{
			it->second->glottis->setVoicing(true);
		}
		this->voicingCounter = 0;
	}
	else
		this->voicingCounter += 1;
}

void PinkTromboneAudioProcessor::openNose(bool openNose)
{
	this->tract->openNose(openNose);
}

void PinkTromboneAudioProcessor::setNoseLength(float noseLength, int index)
{
	double noseLengthValue = NOSE_LENGTH_MIN + (NOSE_LENGTH_MAX - NOSE_LENGTH_MIN) * noseLength;
	this->tract->setNoseLength(noseLengthValue, index);
}

void PinkTromboneAudioProcessor::setNoseAttachment(float noseAttachment, int index)
{
	double noseAttachmentValue = NOSE_ATTACHMENT_MIN + (NOSE_ATTACHMENT_MAX - NOSE_ATTACHMENT_MIN) * noseAttachment;
	this->tract->setNoseAttachment(noseAttachmentValue, index);
}

void PinkTromboneAudioProcessor::setExtraNose(bool extraNose)
{
	this->extraNose = extraNose;
	this->tract->setExtraNose(extraNose);
}

void PinkTromboneAudioProcessor::updateBreathFactor(double breathFactor)
{
	
	for (int i=0; i<this->numVoices; i++)
	{
        glottisParams[i].glottis->setBreathFactor(breathFactor);
	}
}

void PinkTromboneAudioProcessor::updateSize(double sizeFactor)
{
	int tractLength = 39 + 5 * sizeFactor;
	double sizeOffset = 0.15 - sizeFactor/10;
	
	this->tract->updateTractLength(tractLength);
	for (int i=0; i<this->numVoices; i++)
	{
        glottisParams[i].glottis->setSizeOffset(sizeOffset);
	}
}

void PinkTromboneAudioProcessor::setAutoVelum(bool autoVelum)
{
	this->tract->setAutoVelum(autoVelum);
}

void PinkTromboneAudioProcessor::setNoseMode(int selectedId)
{
	if (selectedId == 1) {
		this->openNose(false);
		this->setAutoVelum(false);
	}
	else if (selectedId == 2){
		this->openNose(true);
		this->setAutoVelum(false);
	}
	else if (selectedId == 3) this->setAutoVelum(true);
}

void PinkTromboneAudioProcessor::setIsSettingConstriction(bool tf)
{
    isSettingConstriction = tf;
}

AudioProcessorValueTreeState& PinkTromboneAudioProcessor::getParametersTree()
{
    return parameters;
}

//==============================================================================
void PinkTromboneAudioProcessor::addLFOVisualiser(AudioVisualiserComponent *component)
{
    const ScopedLock sl (listenerLock);
    _lfoVisualisers.addIfNotAlreadyThere (component);
}

void PinkTromboneAudioProcessor::removeLFOVisualiser(AudioVisualiserComponent *component)
{
    const ScopedLock sl (listenerLock);
    _lfoVisualisers.removeFirstMatchingValue (component);
}

//==============================================================================
bool PinkTromboneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PinkTromboneAudioProcessor::createEditor()
{
    return new PinkTromboneAudioProcessorEditor (*this);
}

//==============================================================================
t_tractProps *PinkTromboneAudioProcessor::getTractProps()
{
	return &this->tractProps;
}

//==============================================================================
void PinkTromboneAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PinkTromboneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
