/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "util.h"

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

		if (currentMessage.isNoteOn()){
			auto currentNote = currentMessage.getNoteNumber();
			double midiNoteInHz = juce::MidiMessage::getMidiNoteInHertz(currentNote);
			this->glottis->setFrequency(midiNoteInHz);
			this->voicing = true;
			this->noteOn = true;
			if(this->envelope)
				adsr.noteOn();
		}
		else if (currentMessage.isNoteOff())
		{
			this->noteOn = false;
			if(this->envelope)
			{
				adsr.noteOff();
				*tongueX = this->restTongueX;
				*tongueY = this->restTongueY;
				*constrictionX = this->restConstrictionX;
				*constrictionY = this->restConstrictionY;
			}
			else if (!this->envelope)
			{
				this->voicing = false;
				this->glottis->setVoicing(this->voicing);
				this->voicingCounter = 0;
			}
		}
	}
	
	if(this->envelope && !adsr.isActive())
	{
		this->voicing = false;
		this->glottis->setVoicing(this->voicing);
		this->voicingCounter = 0;
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
		
		double glot = this->glottis->runStep(lambda1, asp);
		double vocalOutput = 0.0;
		this->tract->runStep(glot, fri, lambda1, this->glottis);
		vocalOutput += this->tract->lipOutput + this->tract->noseOutput;
		this->tract->runStep(glot, fri, lambda2, this->glottis);
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
	
	double constrictionIndex;
	double constrictionDiameter;
	double tongueIndex;
	double tongueDiameter;
	double innerTongueControlRadius = 2.05;
	double outerTongueControlRadius = 3.5;
	
	if (adsr.isActive()) {
		constrictionIndex = this->envelopeConstrictionX * (double) this->tract->getTractIndexCount();
		constrictionDiameter = (this->envelopeConstrictionY/2 + 0.5) * (this->constrictionMax - this->constrictionMin) + this->constrictionMin;
		tongueIndex = this->envelopeTongueX * ((double) (this->tract->tongueIndexUpperBound() - this->tract->tongueIndexLowerBound())) + this->tract->tongueIndexLowerBound();
		tongueDiameter = this->envelopeTongueY * (outerTongueControlRadius - innerTongueControlRadius) + innerTongueControlRadius;
		
	}
	else {
		constrictionIndex = *constrictionX * (double) this->tract->getTractIndexCount();
		constrictionDiameter = (*constrictionY/2 + 0.5) * (this->constrictionMax - this->constrictionMin) + this->constrictionMin;
		//(constrictionY/2 + 0.5) is to adjust for diameter range since we are not implementing nasal cavity atm i.e. 0->1 UI range is really 0.5->1
		tongueIndex = *tongueX * ((double) (this->tract->tongueIndexUpperBound() - this->tract->tongueIndexLowerBound())) + this->tract->tongueIndexLowerBound();
		tongueDiameter = *tongueY * (outerTongueControlRadius - innerTongueControlRadius) + innerTongueControlRadius;
	}
	
	this->fricativeIntensity += 0.1; // TODO ex recto
	this->fricativeIntensity = minf(1.0, this->fricativeIntensity);
	
	this->tract->setRestDiameter(tongueIndex, tongueDiameter);
	this->tract->setConstriction(constrictionIndex, constrictionDiameter, this->fricativeIntensity);
	this->glottis->finishBlock();
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

void PinkTromboneAudioProcessor::applyEnvelope(float sampleVal)
{
	if(this->tongueXMod)
		this->envelopeTongueX = this->restTongueX - abs(this->restTongueX - this->tongueXModVal)*sampleVal;
	else
		this->envelopeTongueX = *tongueX;
	
	if(this->tongueYMod)
		this->envelopeTongueY = this->restTongueY - abs(this->restTongueY - this->tongueYModVal)*sampleVal;
	else
		this->envelopeTongueY = *tongueY;
	
	if(this->constrictionXMod)
		this->envelopeConstrictionX = this->restConstrictionX - abs(this->restConstrictionX - this->constrictionXModVal)*sampleVal;
	else
		this->envelopeConstrictionX = *constrictionX;
	
	if(this->constrictionYMod)
		this->envelopeConstrictionY = this->restConstrictionY - abs(this->restConstrictionY - this->constrictionYModVal)*sampleVal;
	else
		this->envelopeConstrictionY = *constrictionY;
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
