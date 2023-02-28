//
//  EnvelopeComponent_hpp.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include "EnvelopeComponent.hpp"
#include "PluginProcessor.h"

EnvelopeComponent::EnvelopeComponent(AudioProcessorValueTreeState &state,
                                     ADSRUI::ParameterIdentifiers identifiers)
: _adsrui(state, identifiers)
, _tongueModXSlider(state, PinkTromboneAudioProcessor::envModTongueX)
{
    addAndMakeVisible(_adsrui);
    
    titleLabel.setText("ENVELOPE", juce::dontSendNotification);
    titleLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(titleLabel);
    
    addAndMakeVisible(_tongueModXSlider);
}

EnvelopeComponent::~EnvelopeComponent()
{
    
}

void EnvelopeComponent::paint(Graphics &)
{
    
}

void EnvelopeComponent::resized()
{
    titleLabel.setBounds(0, 0, 100, 20);
    
    _adsrui.setBounds(0, 26, getWidth() - 50, getHeight() - 26);
    
    _tongueModXSlider.setBounds(getWidth() - 50, 0, 50, 20);
}
