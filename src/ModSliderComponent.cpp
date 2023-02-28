//
//  ModSliderComponent.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include "ModSliderComponent.hpp"

ModSliderComponent::ModSliderComponent(AudioProcessorValueTreeState &state, const String &parameterID)
: _attachment(state, parameterID, _slider)
{
    addAndMakeVisible(_slider);
    addAndMakeVisible(_label);
    _label.setInterceptsMouseClicks(false, false);
    _label.setJustificationType(Justification::centred);
    _label.setFont (juce::Font ("Myriad Pro", 9.0f, juce::Font::plain));
    _slider.setLookAndFeel(&_modLookAndFeel);
    _slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    state.addParameterListener(parameterID, this);
    
    auto self = this;
    destructor = [&] { state.removeParameterListener(parameterID, self); };
    
    auto param = state.getParameter(parameterID);
    configureLabelVisibility(param->convertFrom0to1(param->getValue()));
}

ModSliderComponent::~ModSliderComponent()
{
    destructor();
}

void ModSliderComponent::resized()
{
    _slider.setBounds(0, 0, getWidth(), getHeight());
    _label.setBounds(0, 0, getWidth(), getHeight());
}

void ModSliderComponent::parameterChanged(const String &, float newValue)
{
    configureLabelVisibility(newValue);
}

void ModSliderComponent::configureLabelVisibility(float parameterValue)
{
    _label.setVisible(parameterValue != 0.0);
    
    String labelText;
    labelText << ((int) (parameterValue * 100)) << "%";
    _label.setText(labelText, juce::dontSendNotification);
}
