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
	
	tongueXAttachment = new SliderParameterAttachment (*(processor.tongueX), tongueX);
	
	tongueXModVal.setSliderStyle (Slider::RotaryVerticalDrag);
	tongueXModVal.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	tongueXModVal.setRange(-100.0, 100.0, 1.0);
	tongueXModVal.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueXModVal.setPopupDisplayEnabled (true, true, this);
	tongueXModVal.setTextValueSuffix ("% Tongue Index Modulation");
	tongueXModVal.setValue(0.0);
	addAndMakeVisible (&tongueXModVal);
	tongueXModVal.addListener(this);
	
	tongueY.setSliderStyle (Slider::RotaryVerticalDrag);
	tongueY.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	tongueY.setRange(0.0, 1.0, 0.01);
	tongueY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueY.setPopupDisplayEnabled (true, true, this);
	tongueY.setTextValueSuffix (" Tongue Diameter");
	tongueY.setValue(1.0);
	addAndMakeVisible (&tongueY);
	tongueY.addListener(this);
	
	tongueYAttachment = new SliderParameterAttachment (*(processor.tongueY), tongueY);
	
	tongueYModVal.setSliderStyle (Slider::RotaryVerticalDrag);
	tongueYModVal.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	tongueYModVal.setRange(-100.0, 100.0, 1.0);
	tongueYModVal.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueYModVal.setPopupDisplayEnabled (true, true, this);
	tongueYModVal.setTextValueSuffix ("% Tongue Diameter Modulation");
	tongueYModVal.setValue(0.0);
	addAndMakeVisible (&tongueYModVal);
	tongueYModVal.addListener(this);
	
	constrictionX.setSliderStyle (Slider::RotaryVerticalDrag);
	constrictionX.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	constrictionX.setRange(0.0, 1.0, 0.01);
	constrictionX.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionX.setPopupDisplayEnabled (true, true, this);
	constrictionX.setTextValueSuffix (" Constriction Index");
	constrictionX.setValue(1.0);
	addAndMakeVisible (&constrictionX);
	constrictionX.addListener(this);
	
	constrictionXAttachment = new SliderParameterAttachment (*(processor.constrictionX), constrictionX);
	
	constrictionXModVal.setSliderStyle (Slider::RotaryVerticalDrag);
	constrictionXModVal.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	constrictionXModVal.setRange(-100.0, 100.0, 1.0);
	constrictionXModVal.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionXModVal.setPopupDisplayEnabled (true, true, this);
	constrictionXModVal.setTextValueSuffix ("% Constriction Index Modulation");
	constrictionXModVal.setValue(0.0);
	addAndMakeVisible (&constrictionXModVal);
	constrictionXModVal.addListener(this);
	
	constrictionY.setSliderStyle (Slider::RotaryVerticalDrag);
	constrictionY.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	constrictionY.setRange(0.0, 1.0, 0.01);
	constrictionY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionY.setPopupDisplayEnabled (true, true, this);
	constrictionY.setTextValueSuffix (" Constriction Diameter");
	constrictionY.setValue(1.0);
	addAndMakeVisible (&constrictionY);
	constrictionY.addListener(this);
	
	constrictionYAttachment = new SliderParameterAttachment (*(processor.constrictionY), constrictionY);
	
	constrictionYModVal.setSliderStyle (Slider::RotaryVerticalDrag);
	constrictionYModVal.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	constrictionYModVal.setRange(-100.0, 100.0, 1.0);
	constrictionYModVal.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	constrictionYModVal.setPopupDisplayEnabled (true, true, this);
	constrictionYModVal.setTextValueSuffix ("% Constriction Diameter Modulation");
	constrictionYModVal.setValue(0.0);
	addAndMakeVisible (&constrictionYModVal);
	constrictionYModVal.addListener(this);
	
	VOT.setSliderStyle (Slider::RotaryVerticalDrag);
	VOT.setRotaryParameters(M_PI, 3*M_PI + 0.1, true);
	VOT.setRange(0, 0.3, 0.01);
	VOT.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	VOT.setPopupDisplayEnabled (true, true, this);
	VOT.setTextValueSuffix (" Voice Onset Time (s)");
	VOT.setValue(0.0);
	addAndMakeVisible (&VOT);
	VOT.addListener(this);
	
	attackLength.setSliderStyle (Slider::LinearHorizontal);
	attackLength.setRange(0, 2000, 10);
	attackLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	attackLength.setPopupDisplayEnabled (true, true, this);
	attackLength.setTextValueSuffix (" Attack length (ms)");
	attackLength.setValue(100);
	addAndMakeVisible (&attackLength);
	attackLength.addListener(this);
	
	attackExp.setSliderStyle (Slider::LinearHorizontal);
	attackExp.setRange(-5, 5, 0.1);
	attackExp.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	attackExp.setPopupDisplayEnabled (true, true, this);
	attackExp.setTextValueSuffix (" Attack exponent");
	attackExp.setValue(1);
	addAndMakeVisible (&attackExp);
	attackExp.addListener(this);
	
	decayLength.setSliderStyle (Slider::LinearHorizontal);
	decayLength.setRange(0, 2000, 10);
	decayLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	decayLength.setPopupDisplayEnabled (true, true, this);
	decayLength.setTextValueSuffix (" Decay length (ms)");
	decayLength.setValue(100);
	addAndMakeVisible (&decayLength);
	decayLength.addListener(this);
	
	decayExp.setSliderStyle (Slider::LinearHorizontal);
	decayExp.setRange(-5, 5, 0.1);
	decayExp.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	decayExp.setPopupDisplayEnabled (true, true, this);
	decayExp.setTextValueSuffix (" Decay exponent");
	decayExp.setValue(1);
	addAndMakeVisible (&decayExp);
	decayExp.addListener(this);
	
	sustain.setSliderStyle (Slider::LinearHorizontal);
	sustain.setRange(0.0, 100.0, 1);
	sustain.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	sustain.setPopupDisplayEnabled (true, true, this);
	sustain.setTextValueSuffix ("% Sustain");
	sustain.setValue(0);
	addAndMakeVisible (&sustain);
	sustain.addListener(this);
	
	releaseLength.setSliderStyle (Slider::LinearHorizontal);
	releaseLength.setRange(0, 2000, 10);
	releaseLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	releaseLength.setPopupDisplayEnabled (true, true, this);
	releaseLength.setTextValueSuffix (" Release length (ms)");
	releaseLength.setValue(100);
	addAndMakeVisible (&releaseLength);
	releaseLength.addListener(this);
	
	releaseExp.setSliderStyle (Slider::LinearHorizontal);
	releaseExp.setRange(-5, 5, 0.1);
	releaseExp.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	releaseExp.setPopupDisplayEnabled (true, true, this);
	releaseExp.setTextValueSuffix (" Release exponent");
	releaseExp.setValue(1);
	addAndMakeVisible (&releaseExp);
	releaseExp.addListener(this);
	
	muteAudio.setButtonText("Mute");
//	addAndMakeVisible(&muteAudio);
	muteAudio.addListener(this);
	
	envelope.setButtonText("Constriction envelope");
	addAndMakeVisible(&envelope);
	envelope.addListener(this);
	
	tongueXMod.setButtonText("Modulate tongue index");
	addAndMakeVisible(&tongueXMod);
	tongueXMod.addListener(this);
	
	tongueYMod.setButtonText("Modulate tongue diam.");
	addAndMakeVisible(&tongueYMod);
	tongueYMod.addListener(this);
	
	constrictionXMod.setButtonText("Modulate constriction index");
	addAndMakeVisible(&constrictionXMod);
	constrictionXMod.addListener(this);
	
	constrictionYMod.setButtonText("Modulate constriction diam.");
	addAndMakeVisible(&constrictionYMod);
	constrictionYMod.addListener(this);
	
	breath.setButtonText("Always breathe");
	addAndMakeVisible(&breath);
	breath.addListener(this);
	
	addMouseListener(this, true);

}

PinkTromboneAudioProcessorEditor::~PinkTromboneAudioProcessorEditor()
{
	delete tongueXAttachment;
	delete tongueYAttachment;
	delete constrictionXAttachment;
	delete constrictionYAttachment;
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
	envelope.setBounds(20, 20, 80, 20);
	VOT.setBounds (140, 5, 65, 45);
	breath.setBounds(220, 20, 80, 20);
	
	attackLength.setBounds (15, 30, 75, 50);
	attackExp.setBounds (80, 30, 75, 50);
	
	decayLength.setBounds (15, 55, 75, 50);
	decayExp.setBounds (80, 55, 75, 50);
	
	sustain.setBounds (30, 80, 110, 50);
	
	releaseLength.setBounds (15, 105, 75, 50);
	releaseExp.setBounds (80, 105, 75, 50);
	
	tongueX.setBounds (0, 140, 65, 45);
	tongueXMod.setBounds (50, 152.5, 80, 20);
	tongueXModVal.setBounds (110, 140, 65, 45);
	
	tongueY.setBounds (0, 180, 65, 45);
	tongueYMod.setBounds (50, 192.5, 80, 20);
	tongueYModVal.setBounds (110, 180, 65, 45);
	
	constrictionX.setBounds (0, 220, 65, 45);
	constrictionXMod.setBounds (50, 225, 80, 30);
	constrictionXModVal.setBounds (110, 220, 65, 45);
	
	constrictionY.setBounds (0, 260, 65, 45);
	constrictionYMod.setBounds (50, 265, 80, 30);
	constrictionYModVal.setBounds (110, 260, 65, 45);
	
	muteAudio.setBounds(170, 30, 100, 20);
	tractUI.setSize(getWidth(), getHeight());
}

void PinkTromboneAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	processor.restTongueX = tongueX.getValue();
	processor.restTongueY = tongueY.getValue();
	processor.restConstrictionX = constrictionX.getValue();
	processor.restConstrictionY = constrictionY.getValue();
	
	processor.VOT = VOT.getValue();
	
	processor.adsrParams.attack = attackLength.getValue()/1000;
	processor.adsrParams.attackExp = attackExp.getValue();
	processor.adsrParams.decay = decayLength.getValue()/1000;
	processor.adsrParams.decayExp = decayExp.getValue();
	processor.adsrParams.sustain = sustain.getValue()/100;
	processor.adsrParams.release = releaseLength.getValue()/1000;
	processor.adsrParams.releaseExp = releaseExp.getValue();
	
	if(tongueXModVal.getValue() <= 0) processor.tongueXModVal = processor.restTongueX + (tongueXModVal.getValue()/100)*processor.restTongueX;
	else if (tongueXModVal.getValue() > 0) processor.tongueXModVal = processor.restTongueX + (tongueXModVal.getValue()/100)*(1 - processor.restTongueX);
	
	if(tongueYModVal.getValue() <= 0) processor.tongueYModVal = processor.restTongueY + (tongueYModVal.getValue()/100)*processor.restTongueY;
	else if (tongueYModVal.getValue() > 0) processor.tongueYModVal = processor.restTongueY + (tongueYModVal.getValue()/100)*(1 - processor.restTongueY);
	
	if(constrictionXModVal.getValue() <= 0) processor.constrictionXModVal = processor.restConstrictionX + (constrictionXModVal.getValue()/100)*processor.restConstrictionX;
	else if (constrictionXModVal.getValue() > 0) processor.constrictionXModVal = processor.restConstrictionX + (constrictionXModVal.getValue()/100)*(1 - processor.restConstrictionX);
	
	if(constrictionYModVal.getValue() <= 0) processor.constrictionYModVal = processor.restConstrictionY + (constrictionYModVal.getValue()/100)*processor.restConstrictionY;
	else if (constrictionYModVal.getValue() > 0) processor.constrictionYModVal = processor.restConstrictionY + (constrictionYModVal.getValue()/100)*(1 - processor.restConstrictionY);
}

void PinkTromboneAudioProcessorEditor::buttonClicked(Button *button) { }

void PinkTromboneAudioProcessorEditor::buttonStateChanged(Button *button)
{
	processor.muteAudio = this->muteAudio.getToggleState();
	processor.envelope = this->envelope.getToggleState();
	
	processor.tongueXMod = this->tongueXMod.getToggleState();
	processor.tongueYMod = this->tongueYMod.getToggleState();
	processor.constrictionXMod = this->constrictionXMod.getToggleState();
	processor.constrictionYMod = this->constrictionYMod.getToggleState();
	processor.breath = this->breath.getToggleState();
}
