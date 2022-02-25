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
	initializeTractProps(&this->tractProps, 44);
	
	instrument.enableLegacyMode (24);
	instrument.addListener(this);
}

PinkTromboneAudioProcessor::~PinkTromboneAudioProcessor()
{
}

//==============================================================================
const String PinkTromboneAudioProcessor::getName() const
{
    return JucePlugin_Name;
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
	
	this->glottis = new Glottis(sampleRate);
	this->glottis1 = new Glottis(sampleRate);
	this->glottis2 = new Glottis(sampleRate);
	this->glottis3 = new Glottis(sampleRate);
	this->glottis4 = new Glottis(sampleRate);
	this->glottis5 = new Glottis(sampleRate);
	this->glottis6 = new Glottis(sampleRate);
	this->glottis7 = new Glottis(sampleRate);
	
	this->glotList[0] = this->glottis;
	this->glotList[1] = this->glottis1;
	this->glotList[2] = this->glottis2;
	this->glotList[3] = this->glottis3;
	this->glotList[4] = this->glottis4;
	this->glotList[5] = this->glottis5;
	this->glotList[6] = this->glottis6;
	this->glotList[7] = this->glottis7;
	
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
	
	adsr.setParameters(adsrParams);
	
	for (auto meta : midiMessages) {
		auto currentMessage = meta.getMessage();
		instrument.processNextMidiEvent(currentMessage);
	}
	
	if(this->envelope && !adsr.isActive())
	{
		this->voicing = false;
		this->glottis->setVoicing(this->voicing);
		this->voicingCounter = 0;
		this->glottis->isActive = false;
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
		double glotModulatorSum = 0;
		for (int i=0; i<8; i++)
		{
			double glot = glotList[i]->runStep(lambda1, asp);
			double glotModulator = glotList[i]->getNoiseModulator();
			glotSum += glot;
			glotModulatorSum += glotModulator;
		}
		double vocalOutput = 0.0;
		this->tract->runStep(glotSum, fri, lambda1, glotModulatorSum);
		vocalOutput += this->tract->lipOutput + this->tract->noseOutput;
		this->tract->runStep(glotSum, fri, lambda2, glotModulatorSum);
		vocalOutput += this->tract->lipOutput + this->tract->noseOutput;

		this->applyEnvelope(adsr.getNextSample());
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
	
	double tongueIndex = (*tongueX + this->tongueXModulation) * ((double) (this->tract->tongueIndexUpperBound() - this->tract->tongueIndexLowerBound())) + this->tract->tongueIndexLowerBound();
	double tongueDiameter = (*tongueY + this->tongueYModulation) * (outerTongueControlRadius - innerTongueControlRadius) + innerTongueControlRadius;
	double constrictionIndex = (*constrictionX + this->constrictionXModulation) * (double) this->tract->getTractIndexCount();
	double constrictionDiameter = ((*constrictionY + this->constrictionYModulation)/2 + 0.5) * (this->constrictionMax - this->constrictionMin) + this->constrictionMin;
	//(constrictionY/2 + 0.5) is to adjust for diameter range since we are not implementing nasal cavity atm i.e. 0->1 UI range is really 0.5->1
	
	this->fricativeIntensity += 0.1; // TODO ex recto
	this->fricativeIntensity = minf(1.0, this->fricativeIntensity);
	
	this->tract->setRestDiameter(tongueIndex, tongueDiameter);
	this->tract->setConstriction(constrictionIndex, constrictionDiameter, this->fricativeIntensity);
	for (int i=0; i<8; i++)
	{
		glotList[i]->finishBlock();
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
	double midiNoteInHz = newNote.getFrequencyInHertz();
	this->voicing = true;
	this->noteOn = true;
	
	for (int i=0; i<8; i++)
	{
		if(!this->glotList[i]->isActive)
		{
			glotList[i]->setFrequency(midiNoteInHz);
			glotList[i]->setActive(true);
			glotList[i]->setVoicing(true);
			this->glottisMap.emplace(newNote.noteID, glotList[i]);
			break;
		}
	}
	
	if(this->envelope)
		adsr.noteOn();
}

void PinkTromboneAudioProcessor::noteReleased(MPENote finishedNote)
{
	std::map<uint16, Glottis*>::iterator glotOff = this->glottisMap.find(finishedNote.noteID);
	glotOff->second->setVoicing(false);
	glotOff->second->setActive(false);
	this->glottisMap.erase(finishedNote.noteID);
	
	if(this->envelope && this->glottisMap.empty())
	{
		this->noteOn = false;
		adsr.noteOff();
	}
	
	else if (!this->envelope && this->glottisMap.empty())
	{
		this->noteOn = false;
		this->voicing = false;
		this->glottis->setVoicing(this->voicing);
		this->voicingCounter = 0;
	}
	
}

void PinkTromboneAudioProcessor::applyEnvelope(float sampleVal)
{
	if(this->tongueXMod) this->tongueXModulation = (this->tongueXModVal - this->restTongueX)*sampleVal;
	else this->tongueXModulation = 0.0;

	if(this->tongueYMod) this->tongueYModulation = (this->tongueYModVal - this->restTongueY)*sampleVal;
	else this->tongueYModulation = 0.0;

	if(this->constrictionXMod) this->constrictionXModulation = (this->constrictionXModVal - this->restConstrictionX)*sampleVal;
	else this->constrictionXModulation = 0.0;

	if(this->constrictionYMod) this->constrictionYModulation = (this->constrictionYModVal - this->restConstrictionY)*sampleVal;
	else this->constrictionYModulation = 0.0;
}

void PinkTromboneAudioProcessor::applyVoicing()
{
	if(this->voicing)
	{
		if (this->voicingCounter == this->VOT*this->sampleRate)
		{
			this->glottis->setVoicing(true);
			this->voicingCounter = 0;
		}
		else
			this->voicingCounter += 1;
	}
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

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PinkTromboneAudioProcessor();
}
