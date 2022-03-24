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
    setSize (800, 600);
	
	tractUI.setSize(400, 300);
    tractUI.setBounds(400, 0, 400, 450);
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
	
    // Attack
	attackSlider.setSliderStyle (Slider::LinearHorizontal);
	attackSlider.setRange(0, 2000, 1);
    attackSlider.setTextBoxStyle (Slider::TextBoxLeft, false, 90, 14);
	attackSlider.setTextValueSuffix (" ms");
	attackSlider.setValue(100);
	addAndMakeVisible (&attackSlider);
	attackSlider.addListener(this);
	
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.attachToComponent (&attackSlider, true);
    addAndMakeVisible (&attackLabel);
    
	attackExp.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
	attackExp.setRange(-5, 5, 0.1);
	attackExp.setTextBoxStyle (Slider::TextBoxRight, false, 40, 14);
	attackExp.setValue(0);
	addAndMakeVisible (&attackExp);
	attackExp.addListener(this);
	
    // Decay
	decaySlider.setSliderStyle (Slider::LinearHorizontal);
	decaySlider.setRange(0, 2000, 10);
    decaySlider.setTextBoxStyle (Slider::TextBoxLeft, false, 90, 14);
	decaySlider.setTextValueSuffix (" ms");
	decaySlider.setValue(100);
	addAndMakeVisible (&decaySlider);
	decaySlider.addListener(this);
    
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.attachToComponent (&decaySlider, true);
    addAndMakeVisible (&decayLabel);
	
    decayExp.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    decayExp.setRange(-5, 5, 0.1);
    decayExp.setTextBoxStyle (Slider::TextBoxRight, false, 40, 14);
    decayExp.setValue(0);
    addAndMakeVisible (&decayExp);
    decayExp.addListener(this);
	
    // Sustain
	sustainSlider.setSliderStyle (Slider::LinearHorizontal);
	sustainSlider.setRange(0.0, 100.0, 1);
    sustainSlider.setTextBoxStyle (Slider::TextBoxLeft, false, 90, 14);
	sustainSlider.setTextValueSuffix ("%");
	sustainSlider.setValue(0);
	addAndMakeVisible (&sustainSlider);
	sustainSlider.addListener(this);
    
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.attachToComponent (&sustainSlider, true);
    addAndMakeVisible (&sustainLabel);
	
    // Release
    releaseSlider.setSliderStyle (Slider::LinearHorizontal);
    releaseSlider.setRange(0, 2000, 10);
    releaseSlider.setTextBoxStyle (Slider::TextBoxLeft, false, 90, 14);
    releaseSlider.setTextValueSuffix (" ms");
    releaseSlider.setValue(100);
    addAndMakeVisible (&releaseSlider);
    releaseSlider.addListener(this);
    
    releaseLabel.setText("Decay", juce::dontSendNotification);
    releaseLabel.attachToComponent (&releaseSlider, true);
    addAndMakeVisible (&releaseLabel);
    
    releaseExp.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    releaseExp.setRange(-5, 5, 0.1);
    releaseExp.setTextBoxStyle (Slider::TextBoxRight, false, 40, 14);
    releaseExp.setValue(0);
    addAndMakeVisible (&releaseExp);
    releaseExp.addListener(this);
	
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
	
	noseLength.setSliderStyle (Slider::LinearHorizontal);
	noseLength.setRange(0, 1, 0.01);
	noseLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	noseLength.setPopupDisplayEnabled (true, true, this);
	noseLength.setValue(0.59);  //noseLength = 28
	addAndMakeVisible (&noseLength);
	noseLength.addListener(this);
	
	noseLengthLabel.setText("Nose length", juce::dontSendNotification);
	noseLengthLabel.attachToComponent (&noseLength, true);
	addAndMakeVisible (&noseLengthLabel);
	
	noseAttachment.setSliderStyle (Slider::LinearHorizontal);
	noseAttachment.setRange(0, 1, 0.01);
	noseAttachment.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	noseAttachment.setPopupDisplayEnabled (true, true, this);
	noseAttachment.setValue(0.38);  //noseStart = 17
	addAndMakeVisible (&noseAttachment);
	noseAttachment.addListener(this);
	
	noseAttachmentLabel.setText("Nose attachment", juce::dontSendNotification);
	noseAttachmentLabel.attachToComponent (&noseAttachment, true);
	addAndMakeVisible (&noseAttachmentLabel);
	
	extraNose.setButtonText("Extra nose");
	addAndMakeVisible(&extraNose);
	extraNose.addListener(this);
	
	extraNoseLength.setSliderStyle (Slider::LinearHorizontal);
	extraNoseLength.setRange(0, 1, 0.01);
	extraNoseLength.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	extraNoseLength.setPopupDisplayEnabled (true, true, this);
	extraNoseLength.setValue(0.59);  //noseLength = 28
	addAndMakeVisible (&extraNoseLength);
	extraNoseLength.addListener(this);
	extraNoseLength.setEnabled(this->extraNose.getToggleState());
	
	extraNoseLengthLabel.setText("Extra nose length", juce::dontSendNotification);
	extraNoseLengthLabel.attachToComponent (&extraNoseLength, true);
	addAndMakeVisible (&extraNoseLengthLabel);
	
	extraNoseAttachment.setSliderStyle (Slider::LinearHorizontal);
	extraNoseAttachment.setRange(0, 1, 0.01);
	extraNoseAttachment.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	extraNoseAttachment.setPopupDisplayEnabled (true, true, this);
	extraNoseAttachment.setValue(0.38);  //noseStart = 17
	addAndMakeVisible (&extraNoseAttachment);
	extraNoseAttachment.addListener(this);
	extraNoseAttachment.setEnabled(this->extraNose.getToggleState());
	
	extraNoseAttachmentLabel.setText("Extra nose length", juce::dontSendNotification);
	extraNoseAttachmentLabel.attachToComponent (&extraNoseAttachment, true);
	addAndMakeVisible (&extraNoseAttachmentLabel);
	
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
	// Local bounds are 0, 0, 800, 600
	noseMode.setBounds(240, 220, 100, 20);
	extraNose.setBounds(170, 250, 60, 20);
	breath.setBounds(20, 330, 80, 20);
    
    int sliderLeftMargin = 50;
    int sliderHeight = 20;
    int sliderWidth = 200;
    int sliderTopMargin = 14;
    int sliderSpacing = 6;
    int exponentDialLeftMargin = -10;
    int exponentDialTopMargin = 6;
    int exponentDialHeight = 40;
    int exponentDialWidth = 75;
	
    // Envelope
	attackSlider.setBounds (sliderLeftMargin,
                            sliderTopMargin,
                            sliderWidth,
                            sliderHeight);
	attackExp.setBounds (sliderLeftMargin + sliderWidth + exponentDialLeftMargin,
                         exponentDialTopMargin,
                         exponentDialWidth,
                         exponentDialHeight);
    
    decaySlider.setBounds (sliderLeftMargin,
                            sliderTopMargin + (sliderHeight + sliderSpacing),
                            sliderWidth,
                            sliderHeight);
    decayExp.setBounds (sliderLeftMargin + sliderWidth + exponentDialLeftMargin,
                         exponentDialTopMargin +  + (sliderHeight + sliderSpacing),
                         exponentDialWidth,
                         exponentDialHeight);
    
    sustainSlider.setBounds (sliderLeftMargin,
                            sliderTopMargin + 2 * (sliderHeight + sliderSpacing),
                            sliderWidth,
                            sliderHeight);
    
    releaseSlider.setBounds (sliderLeftMargin,
                            sliderTopMargin + 3 * (sliderHeight + sliderSpacing),
                            sliderWidth,
                            sliderHeight);
    releaseExp.setBounds (sliderLeftMargin + sliderWidth + exponentDialLeftMargin,
                         exponentDialTopMargin +  + 3 * (sliderHeight + sliderSpacing),
                         exponentDialWidth,
                         exponentDialHeight);
    
    // Tract UI
	
	breathFactor.setBounds (100, 350, 75, 100);
	sizeFactor.setBounds (100, 380, 75, 100);
	
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
	
	noseLength.setBounds(280, 150, 70, 20);
	noseAttachment.setBounds(280, 180, 70, 20);
	
	extraNoseLength.setBounds(280, 280, 70, 20);
	extraNoseAttachment.setBounds(280, 310, 70, 20);
	
	muteAudio.setBounds(170, 30, 100, 20);
	tractUI.setSize(getWidth(), getHeight());
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

    processor.tongueXMod->modulationAtIndex(0)->scale = tongueXModVal.getValue() / 100;
    processor.tongueYMod->modulationAtIndex(0)->scale = tongueYModVal.getValue() / 100;
    processor.constrictionXMod->modulationAtIndex(0)->scale = constrictionXModVal.getValue() / 100;
    processor.constrictionYMod->modulationAtIndex(0)->scale = constrictionYModVal.getValue() / 100;
	
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
    
    // ADSR Modulation
    processor.tongueXMod->modulationAtIndex(0)->active = this->tongueXMod.getToggleState();
    processor.tongueYMod->modulationAtIndex(0)->active = this->tongueYMod.getToggleState();
    processor.constrictionXMod->modulationAtIndex(0)->active = this->constrictionXMod.getToggleState();
    processor.constrictionYMod->modulationAtIndex(0)->active = this->constrictionYMod.getToggleState();
	
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
