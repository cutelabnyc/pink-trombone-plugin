//
//  EnvelopeComponent_hpp.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include "EnvelopeComponent.hpp"

EnvelopeComponent::EnvelopeComponent(AudioProcessorValueTreeState &state,
                                     ADSRUI::ParameterIdentifiers identifiers)
: _adsrui(state, identifiers)
{
    addAndMakeVisible(_adsrui);
    
    titleLabel.setText("ENVELOPE", juce::dontSendNotification);
    titleLabel.setFont (juce::Font ("Myriad Pro", 8.0f, juce::Font::plain));
    addAndMakeVisible(titleLabel);
}

EnvelopeComponent::~EnvelopeComponent()
{
    
}

void EnvelopeComponent::paint(Graphics &g)
{
    
}

void EnvelopeComponent::resized()
{
    titleLabel.setBounds(0, 0, 100, 20);
    _adsrui.setBounds(0, 26, getWidth(), getHeight() - 26);
}
