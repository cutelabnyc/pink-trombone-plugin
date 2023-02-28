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

class EnvelopeComponent : public Component {
    
public:
    
    EnvelopeComponent(AudioProcessorValueTreeState &state,
           ADSRUI::ParameterIdentifiers identifiers);
    ~EnvelopeComponent() override;
    
    void paint(Graphics &g) override;
    void resized() override;
    
private:
    
    ADSRUI _adsrui;
    Label titleLabel;
    
    ModSliderComponent _tongueModXSlider;
};

#endif /* EnvelopeComponent_hpp */
