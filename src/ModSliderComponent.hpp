//
//  ModSliderComponent.hpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#ifndef ModSliderComponent_hpp
#define ModSliderComponent_hpp

#include <stdio.h>
#include "JuceHeader.h"
#include "ModLookAndFeel.hpp"

class ModSliderComponent : public juce::Component, juce::AudioProcessorValueTreeState::Listener {

public:
    ModSliderComponent(AudioProcessorValueTreeState &state, const String &parameterID);
    ~ModSliderComponent() override;
    
    void resized() override;
    void parameterChanged(const String &parameterId, float newValue) override;
    
private:
    void configureLabelVisibility(float parameterValue);
    
    Slider  _slider;
    AudioProcessorValueTreeState::SliderAttachment _attachment;
    Label _label;
    ModLookAndFeel _modLookAndFeel;
    std::function<void()> destructor;
};

#endif /* ModSliderComponent_hpp */
