/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

//==============================================================================
PinkTromboneAudioProcessorEditor::PinkTromboneAudioProcessorEditor (PinkTromboneAudioProcessor& p)
: AudioProcessorEditor (&p)
, processor (p)
, tractUI(p)
, envelopeComponent(
         p.getParametersTree(),
         {
             PinkTromboneAudioProcessor::initial,
             PinkTromboneAudioProcessor::attack,
             PinkTromboneAudioProcessor::peak,
             PinkTromboneAudioProcessor::decay,
             PinkTromboneAudioProcessor::sustain,
             PinkTromboneAudioProcessor::release
         })
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
	tractUI.setSize(300, 300);
    tractUI.setBounds(0, 0, tractUI.getWidth(), tractUI.getHeight());
	addAndMakeVisible(&tractUI);
    
    envelopeComponent.setSize(300, getHeight() / 2);
    envelopeComponent.setBounds(300, 0, 300, getHeight() / 2);
    addAndMakeVisible(&envelopeComponent);
    
    // Initialize
    noseLength.setValue(0.59);
    noseAttachment.setValue(0.38);
    breathFactor.setValue(0.5);
    sizeFactor.setValue(1.0);
    extraNose.setToggleState(false, juce::dontSendNotification);
//    processor.setExtraNose(this->extraNose.getToggleState());
//    processor.setNoseLength(0.59, 0);
//    processor.setNoseAttachment(0.38, 0);
//    processor.updateBreathFactor(1.5 * 0.5 + 0.25);
//    processor.updateSize(1.0);
	
	breathFactor.setSliderStyle (Slider::LinearHorizontal);
	breathFactor.setRange(0, 1, 0.01);
	breathFactor.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	breathFactor.setPopupDisplayEnabled (true, true, this);
	breathFactor.setValue(0.5);
	addAndMakeVisible (&breathFactor);
	breathFactor.addListener(this);
	
	breathFactorLabel.setText("Breathiness factor", juce::dontSendNotification);
	breathFactorLabel.attachToComponent (&breathFactor, true);
	addAndMakeVisible (&breathFactorLabel);
	
	sizeFactor.setSliderStyle (Slider::LinearHorizontal);
	sizeFactor.setRange(0, 1, 0.2);
	sizeFactor.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	sizeFactor.setPopupDisplayEnabled (true, true, this);
	sizeFactor.setTextValueSuffix (" Size");
	sizeFactor.setValue(1);
	addAndMakeVisible (&sizeFactor);
	sizeFactor.addListener(this);
	
	sizeFactorLabel.setText("Size", juce::dontSendNotification);
	sizeFactorLabel.attachToComponent (&sizeFactor, true);
	addAndMakeVisible (&sizeFactorLabel);
	
	muteAudio.setButtonText("Mute");
//	addAndMakeVisible(&muteAudio);
	muteAudio.addListener(this);
	
	breath.setButtonText("Always breathe");
	addAndMakeVisible(&breath);
	breath.addListener(this);
	
	noseMode.addItem("Always closed", 1);
	noseMode.addItem("Always open", 2);
	noseMode.addItem("Auto mode", 3);
	noseMode.setSelectedId(1);
	addAndMakeVisible(&noseMode);
	noseMode.addListener(this);
	
	noseModeLabel.setText("Nose mode", juce::dontSendNotification);
	noseModeLabel.attachToComponent (&noseMode, true);
	addAndMakeVisible (&noseModeLabel);
	
	addMouseListener(this, true);

}

PinkTromboneAudioProcessorEditor::~PinkTromboneAudioProcessorEditor()
{
}

//==============================================================================
void PinkTromboneAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
//	this.moveTo(this.tongueLowerIndexBound, this.innerTongueControlRadius);
}

void PinkTromboneAudioProcessorEditor::resized()
{
    auto uiLeftBasis = 300;
    
//    tractUI.setSize(300, 300);
//    tractUI.setBounds(0, 0, tractUI.getWidth(), tractUI.getHeight());
    
	// Local bounds are 0, 0, 800, 600
	noseMode.setBounds(240 + uiLeftBasis, 220, 100, 20);
	breath.setBounds(20 + uiLeftBasis, 330, 80, 20);
    
    // Tract UI
	breathFactor.setBounds (100 + uiLeftBasis, 350, 75, 100);
	sizeFactor.setBounds (100 + uiLeftBasis, 380, 75, 100);
	
	muteAudio.setBounds(170 + uiLeftBasis, 30, 100, 20);
	tractUI.setSize(getWidth() / 2, getHeight() / 2);
    envelopeComponent.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
}

void PinkTromboneAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
//	processor.restTongueX = tongueX.getValue();
//	processor.restTongueY = tongueY.getValue();
//	processor.restConstrictionX = constrictionX.getValue();
//	processor.restConstrictionY = constrictionY.getValue();
	
	processor.adsrParams.attack = attackSlider.getValue()/1000;
	processor.adsrParams.attackExp = attackExp.getValue();
	processor.adsrParams.decay = decaySlider.getValue()/1000;
	processor.adsrParams.decayExp = decayExp.getValue();
	processor.adsrParams.sustain = sustainSlider.getValue()/100;
	processor.adsrParams.release = releaseSlider.getValue()/1000;
	processor.adsrParams.releaseExp = releaseExp.getValue();
	
	processor.setNoseLength(noseLength.getValue(), 0);
    processor.setNoseAttachment(noseAttachment.getValue(), 0);
    if (MAX_NOSES > 1) {
        processor.setNoseLength(extraNoseLength.getValue(), 1);
        processor.setNoseAttachment(extraNoseAttachment.getValue(), 1);
    }
	processor.updateBreathFactor(1.5 * breathFactor.getValue() + 0.25);
	processor.updateSize(sizeFactor.getValue());
}

void PinkTromboneAudioProcessorEditor::buttonClicked(Button *button) { }

void PinkTromboneAudioProcessorEditor::buttonStateChanged(Button *button)
{
	processor.muteAudio = this->muteAudio.getToggleState();
	
	processor.breath = this->breath.getToggleState();
    
#if (MAX_NOSES >= 2)
	processor.setExtraNose(this->extraNose.getToggleState());
	
	extraNoseLength.setEnabled(this->extraNose.getToggleState());
	extraNoseAttachment.setEnabled(this->extraNose.getToggleState());
	
	processor.setNoseAttachment(extraNoseAttachment.getValue(), 1);
#else
    processor.setExtraNose(false);
    extraNose.setEnabled(false);
#endif
}

void PinkTromboneAudioProcessorEditor::comboBoxChanged(ComboBox *comboBox) {
	processor.setNoseMode(noseMode.getSelectedId());
}
