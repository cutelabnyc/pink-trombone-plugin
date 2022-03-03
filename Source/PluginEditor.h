/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "TractUI.hpp"

//==============================================================================
/**
*/
class PinkTromboneAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener, private ToggleButton::Listener
{
public:
    PinkTromboneAudioProcessorEditor (PinkTromboneAudioProcessor&);
    ~PinkTromboneAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	void sliderValueChanged (Slider* slider) override;
	void buttonClicked (Button* button) override;
	void buttonStateChanged (Button* button) override;
	
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PinkTromboneAudioProcessor& processor;
	
	Slider tongueX;
	Slider tongueXModVal;
	Slider tongueY;
	Slider tongueYModVal;
	Slider constrictionX;
	Slider constrictionXModVal;
	Slider constrictionY;
	Slider constrictionYModVal;
	
    // Envelope
	Slider attackSlider;
    Label attackLabel;
	Slider attackExp;
    Slider decayLength;
    Slider decayExp;
	Slider sustain;
	Slider releaseLength;
	Slider releaseExp;
	
	ToggleButton muteAudio;
	ToggleButton tongueXMod;
	ToggleButton tongueYMod;
	ToggleButton constrictionXMod;
	ToggleButton constrictionYMod;
	ToggleButton breath;
	
	TractUI tractUI;
	
	SliderParameterAttachment* tongueXAttachment;
	SliderParameterAttachment* tongueYAttachment;
	SliderParameterAttachment* constrictionXAttachment;
	SliderParameterAttachment* constrictionYAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkTromboneAudioProcessorEditor)
};
