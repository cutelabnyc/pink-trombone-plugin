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
    : AudioProcessorEditor (&p), processor (p), tractUI(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
	
	tractUI.setSize(400, 300);
	addAndMakeVisible(&tractUI);
	
	tongueX.setSliderStyle (Slider::Rotary);
	tongueX.setRotaryParameters(2*M_PI, 0.1, true);
	tongueX.setRange(0.0, 1.0, 0.01);
	tongueX.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueX.setPopupDisplayEnabled (true, true, this);
	tongueX.setTextValueSuffix (" Tongue Index");
	tongueX.setValue(1.0);
	addAndMakeVisible (&tongueX);
	tongueX.addListener(this);
	
	tongueY.setSliderStyle (Slider::Rotary);
	tongueY.setRotaryParameters(2*M_PI, 0.1, true);
	tongueY.setRange(0.0, 1.0, 0.01);
	tongueY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueY.setPopupDisplayEnabled (true, true, this);
	tongueY.setTextValueSuffix (" Tongue Diameter");
	tongueY.setValue(1.0);
	addAndMakeVisible (&tongueY);
	tongueY.addListener(this);
	
	constrictionX.setSliderStyle (Slider::Rotary);
	constrictionX.setRotaryParameters(2*M_PI, 0.1, true);
	constrictionX.setRange(0.0, 1.0, 0.01);
	constrictionX.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionX.setPopupDisplayEnabled (true, true, this);
	constrictionX.setTextValueSuffix (" Constriction Index");
	constrictionX.setValue(1.0);
	addAndMakeVisible (&constrictionX);
	constrictionX.addListener(this);
	
	constrictionY.setSliderStyle (Slider::Rotary);
	constrictionY.setRotaryParameters(2*M_PI, 0.1, true);
	constrictionY.setRange(0.0, 1.0, 0.01);
	constrictionY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionY.setPopupDisplayEnabled (true, true, this);
	constrictionY.setTextValueSuffix (" Constriction Diameter");
	constrictionY.setValue(1.0);
	addAndMakeVisible (&constrictionY);
	constrictionY.addListener(this);
	
	constrictionEnvelopeMax.setSliderStyle (Slider::Rotary);
	constrictionEnvelopeMax.setRotaryParameters(2*M_PI, 0.1, true);
	constrictionEnvelopeMax.setRange(0.0, 1.0, 0.01);
	constrictionEnvelopeMax.setSkewFactor(0.7);
	constrictionEnvelopeMax.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionEnvelopeMax.setPopupDisplayEnabled (true, true, this);
	constrictionEnvelopeMax.setTextValueSuffix (" Constriction Max");
	constrictionEnvelopeMax.setValue(0.0);
	addAndMakeVisible (&constrictionEnvelopeMax);
	constrictionEnvelopeMax.addListener(this);
	
	VOT.setSliderStyle (Slider::Rotary);
	VOT.setRotaryParameters(2*M_PI, 0.1, true);
	VOT.setRange(0, 0.3, 0.01);
	VOT.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	VOT.setPopupDisplayEnabled (true, true, this);
	VOT.setTextValueSuffix (" Voice Onset Time (s)");
	VOT.setValue(0.0);
	addAndMakeVisible (&VOT);
	VOT.addListener(this);
	
	attackLength.setSliderStyle (Slider::LinearHorizontal);
	attackLength.setRange(100, 2000, 10);
	attackLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	attackLength.setPopupDisplayEnabled (true, true, this);
	attackLength.setTextValueSuffix (" Attack length (ms)");
	attackLength.setValue(100);
	addAndMakeVisible (&attackLength);
	attackLength.addListener(this);
	
	decayLength.setSliderStyle (Slider::LinearHorizontal);
	decayLength.setRange(100, 2000, 10);
	decayLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	decayLength.setPopupDisplayEnabled (true, true, this);
	decayLength.setTextValueSuffix (" Decay length (ms)");
	decayLength.setValue(100);
	addAndMakeVisible (&decayLength);
	decayLength.addListener(this);
	
	constrictionActive.setButtonText("Constriction Active");
	addAndMakeVisible(&constrictionActive);
	constrictionActive.addListener(this);
	
	muteAudio.setButtonText("Mute");
//	addAndMakeVisible(&muteAudio);
	muteAudio.addListener(this);
	
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
	// Local bounds are 0,0,400,300
	tongueX.setBounds (0, 20, 70, 50);
	tongueY.setBounds (70, 20, 70, 50);
	constrictionX.setBounds (0, 80, 70, 50);
	constrictionY.setBounds (70, 80, 70, 50);
	constrictionEnvelopeMax.setBounds (0, 140, 70, 50);
	VOT.setBounds (70, 140, 70, 50);
	attackLength.setBounds (15, 190, 110, 50);
	decayLength.setBounds (15, 220, 110, 50);
	muteAudio.setBounds(170, 30, 100, 20);
	constrictionActive.setBounds(150, 30, 100, 20);
	tractUI.setSize(getWidth(), getHeight());
}

void PinkTromboneAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	processor.tongueX = tongueX.getValue();
	processor.tongueY = tongueY.getValue();
	processor.constrictionX = constrictionX.getValue();
	processor.constrictionY = constrictionY.getValue();
	processor.constrictionEnvelopeMax = constrictionEnvelopeMax.getValue()/2 + 0.5;
	processor.attackLength = attackLength.getValue()/1000;
	processor.decayLength = decayLength.getValue()/1000;
	processor.UIConstrictionY = constrictionY.getValue();
	processor.VOT = VOT.getValue();
	processor.adsrParams.attack = attackLength.getValue()/1000;
	processor.adsrParams.decay = decayLength.getValue()/1000;
}

void PinkTromboneAudioProcessorEditor::buttonClicked(Button *button) { }

void PinkTromboneAudioProcessorEditor::buttonStateChanged(Button *button)
{
	processor.muteAudio = this->muteAudio.getToggleState();
	processor.constrictionActive = this->constrictionActive.getToggleState();
}

void PinkTromboneAudioProcessorEditor::updateSliders()
{
	tongueX.setValue(processor.tongueX);
	tongueY.setValue(processor.tongueY);
	constrictionX.setValue(processor.constrictionX);
	constrictionY.setValue(processor.constrictionY);
}

void PinkTromboneAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
	this->updateSliders();
}

void PinkTromboneAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
	this->updateSliders();
}

void PinkTromboneAudioProcessorEditor::mouseUp(const juce::MouseEvent& e)
{
	this->updateSliders();
}
