/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
	constrictionX.setTextValueSuffix (" Constriction X");
	constrictionX.setValue(1.0);
	addAndMakeVisible (&constrictionX);
	constrictionX.addListener(this);
	
	constrictionY.setSliderStyle (Slider::Rotary);
	constrictionY.setRotaryParameters(2*M_PI, 0.1, true);
	constrictionY.setRange(0.0, 1.0, 0.01);
	constrictionY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionY.setPopupDisplayEnabled (true, true, this);
	constrictionY.setTextValueSuffix (" Constriction Y");
	constrictionY.setValue(1.0);
	addAndMakeVisible (&constrictionY);
	constrictionY.addListener(this);
	
	constrictionMax.setSliderStyle (Slider::Rotary);
	constrictionMax.setRotaryParameters(2*M_PI, 0.1, true);
	constrictionMax.setRange(-2.0, 2.0, 0.1);
	constrictionMax.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionMax.setPopupDisplayEnabled (true, true, this);
	constrictionMax.setTextValueSuffix (" Constriction Max");
	constrictionMax.setValue(2.0);
	addAndMakeVisible (&constrictionMax);
	constrictionMax.addListener(this);
	
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
	constrictionX.setBounds (0, 100, 70, 50);
	constrictionY.setBounds (70, 100, 70, 50);
	constrictionMax.setBounds (0, 180, 70, 50);
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
	processor.constrictionMax = constrictionMax.getValue();
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
