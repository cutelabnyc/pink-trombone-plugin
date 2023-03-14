//
//  EnvelopeComponent_hpp.hpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#ifndef EnvelopeComponent_hpp
#define EnvelopeComponent_hpp

#include <stdio.h>
#include "JuceHeader.h"
#include "ADSRUI.hpp"
#include "ModSliderComponent.hpp"
#include "HiddenLookAndFeel.hpp"

class EnvelopeComponent : public Component {
    
public:
    
    EnvelopeComponent(AudioProcessorValueTreeState &state,
           ADSRUI::ParameterIdentifiers identifiers);
    ~EnvelopeComponent() override;
    
    void paint(Graphics &g) override;
    void resized() override;
    
private:
    
    ADSRUI _adsrui;
    Label _titleLabel;

    Label _tongueModXLabel;
    Label _tongueModYLabel;
    Label _constrictionModXLabel;
    Label _constrictionModYLabel;
    Label _pitchModLabel;

    HiddenLookAndFeel _hiddenLookAndFeel;
    Slider _attackSlider;
    Slider _decaySlider;
    Slider _releaseSlider;
    Slider _initialSlider;
    Slider _peakSlider;
    Slider _sustainSlider;

    AudioProcessorValueTreeState::SliderAttachment _attackSliderAttachment;
    AudioProcessorValueTreeState::SliderAttachment _decaySliderAttachment;
    AudioProcessorValueTreeState::SliderAttachment _releaseSliderAttachment;
    AudioProcessorValueTreeState::SliderAttachment _initialSliderAttachment;
    AudioProcessorValueTreeState::SliderAttachment _peakSliderAttachment;
    AudioProcessorValueTreeState::SliderAttachment _sustainSliderAttachment;

    ModSliderComponent _tongueModXSlider;
    ModSliderComponent _tongueModYSlider;
    ModSliderComponent _constrictionModXSlider;
    ModSliderComponent _constrictionModYSlider;
    ModSliderComponent _pitchModSlider;
};

#endif /* EnvelopeComponent_hpp */
