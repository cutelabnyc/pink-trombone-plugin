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
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
	
	tongueX.setSliderStyle (Slider::LinearBarVertical);
	tongueX.setRange(0.0, 1.0, 0.01);
	tongueX.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueX.setPopupDisplayEnabled (true, false, this);
	tongueX.setTextValueSuffix (" Tongue X");
	tongueX.setValue(1.0);
	addAndMakeVisible (&tongueX);
	tongueX.addListener(this);
	
	tongueY.setSliderStyle (Slider::LinearBarVertical);
	tongueY.setRange(0.0, 1.0, 0.01);
	tongueY.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
	tongueY.setPopupDisplayEnabled (true, false, this);
	tongueY.setTextValueSuffix (" Tongue Y");
	tongueY.setValue(1.0);
	addAndMakeVisible (&tongueY);
	tongueY.addListener(this);
}

PinkTromboneAudioProcessorEditor::~PinkTromboneAudioProcessorEditor()
{
}

//==============================================================================
void PinkTromboneAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void PinkTromboneAudioProcessorEditor::resized()
{
	tongueX.setBounds (40, 30, 20, getHeight() - 60);
	tongueY.setBounds (70, 30, 20, getHeight() - 60);
}

void PinkTromboneAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	processor.tongueX = tongueX.getValue();
	processor.tongueY = tongueY.getValue();
}
