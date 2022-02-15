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
	
	tongueX.setSliderStyle (Slider::RotaryVerticalDrag);
	tongueX.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	tongueX.setRange(0.0, 1.0, 0.01);
	tongueX.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueX.setPopupDisplayEnabled (true, true, this);
	tongueX.setTextValueSuffix (" Tongue Index");
	tongueX.setValue(1.0);
	addAndMakeVisible (&tongueX);
	tongueX.addListener(this);
	
	tongueY.setSliderStyle (Slider::RotaryVerticalDrag);
	tongueY.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	tongueY.setRange(0.0, 1.0, 0.01);
	tongueY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueY.setPopupDisplayEnabled (true, true, this);
	tongueY.setTextValueSuffix (" Tongue Diameter");
	tongueY.setValue(1.0);
	addAndMakeVisible (&tongueY);
	tongueY.addListener(this);
	
	constrictionX.setSliderStyle (Slider::RotaryVerticalDrag);
	constrictionX.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	constrictionX.setRange(0.0, 1.0, 0.01);
	constrictionX.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionX.setPopupDisplayEnabled (true, true, this);
	constrictionX.setTextValueSuffix (" Constriction Index");
	constrictionX.setValue(1.0);
	addAndMakeVisible (&constrictionX);
	constrictionX.addListener(this);
	
	constrictionY.setSliderStyle (Slider::RotaryVerticalDrag);
	constrictionY.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	constrictionY.setRange(0.0, 1.0, 0.01);
	constrictionY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionY.setPopupDisplayEnabled (true, true, this);
	constrictionY.setTextValueSuffix (" Rest Diameter");
	constrictionY.setValue(1.0);
	addAndMakeVisible (&constrictionY);
	constrictionY.addListener(this);
	
	VOT.setSliderStyle (Slider::RotaryVerticalDrag);
	VOT.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	VOT.setRange(0, 0.3, 0.01);
	VOT.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	VOT.setPopupDisplayEnabled (true, true, this);
	VOT.setTextValueSuffix (" Voice Onset Time (s)");
	VOT.setValue(0.0);
	addAndMakeVisible (&VOT);
	VOT.addListener(this);
	
	decayLength.setSliderStyle (Slider::LinearHorizontal);
	decayLength.setRange(0, 2000, 10);
	decayLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	decayLength.setPopupDisplayEnabled (true, true, this);
	decayLength.setTextValueSuffix (" Decay length (ms)");
	decayLength.setValue(100);
	addAndMakeVisible (&decayLength);
	decayLength.addListener(this);
	
	decayExp.setSliderStyle (Slider::LinearHorizontal);
	decayExp.setRange(-5, 5, 1);
	decayExp.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	decayExp.setPopupDisplayEnabled (true, true, this);
	decayExp.setTextValueSuffix (" Decay exponent");
	decayExp.setValue(1);
	addAndMakeVisible (&decayExp);
	decayExp.addListener(this);
	
	muteAudio.setButtonText("Mute");
//	addAndMakeVisible(&muteAudio);
	muteAudio.addListener(this);
	
	envelope.setButtonText("Constriction envelope");
	addAndMakeVisible(&envelope);
	envelope.addListener(this);
	
	partialConstriction.setButtonText("Partial constriction");
	addAndMakeVisible(&partialConstriction);
	partialConstriction.addListener(this);
	
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
	VOT.setBounds (140, 20, 70, 50);
	decayLength.setBounds (15, 130, 110, 50);
	decayExp.setBounds (15, 160, 110, 50);
	envelope.setBounds(20, 200, 80, 20);
	partialConstriction.setBounds(20, 230, 100, 20);
	muteAudio.setBounds(170, 30, 100, 20);
	tractUI.setSize(getWidth(), getHeight());
}

void PinkTromboneAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	processor.tongueX = tongueX.getValue();
	processor.tongueY = tongueY.getValue();
	processor.constrictionX = constrictionX.getValue();
	processor.constrictionY = constrictionY.getValue();
	processor.decayLength = decayLength.getValue()/1000;
	processor.restConstrictionY = constrictionY.getValue();
	processor.VOT = VOT.getValue();
	processor.adsrParams.decay = decayLength.getValue()/1000;
	processor.adsrParams.decayExp = decayExp.getValue();
}

void PinkTromboneAudioProcessorEditor::buttonClicked(Button *button) { }

void PinkTromboneAudioProcessorEditor::buttonStateChanged(Button *button)
{
	processor.muteAudio = this->muteAudio.getToggleState();
	processor.envelope = this->envelope.getToggleState();
	if (partialConstriction.getToggleState())
		processor.constrictionEnvelopeMax = 0.17;
	else if (!partialConstriction.getToggleState())
		processor.constrictionEnvelopeMax = 0.14;
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
