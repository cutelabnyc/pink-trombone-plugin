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
	void mouseDown(const MouseEvent& e) override;
	void mouseDrag(const MouseEvent& e) override;
	void mouseUp(const MouseEvent& e) override;

private:
	void sliderValueChanged (Slider* slider) override;
	void buttonClicked (Button* button) override;
	void buttonStateChanged (Button* button) override;
	void updateSliders();
	
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PinkTromboneAudioProcessor& processor;
	
	Slider tongueX;
	Slider tongueY;
	Slider constrictionX;
	Slider constrictionY;
	Slider constrictionMax;
	ToggleButton constrictionActive;
	ToggleButton muteAudio;
	TractUI tractUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkTromboneAudioProcessorEditor)
};
