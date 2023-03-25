//
//  LFOComponent.hpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 3/25/23.
//

#ifndef LFOComponent_hpp
#define LFOComponent_hpp

#include <stdio.h>
#include "JuceHeader.h"
#include "ModSliderComponent.hpp"

class LFOComponent : public Component {
    
public:
    
    LFOComponent(AudioProcessorValueTreeState &state);
    ~LFOComponent() override;
    
    void paint(Graphics &g) override;
    void resized() override;
    
private:
    
    Label _titleLabel;

    Label _tongueModXLabel;
    Label _tongueModYLabel;
    Label _constrictionModXLabel;
    Label _constrictionModYLabel;
    Label _pitchModLabel;

    Slider _rateSlider;
    ComboBox _shapePicker;
    ComboBox _modePicker;

    AudioProcessorValueTreeState::SliderAttachment _rateSliderAttachment;
    AudioProcessorValueTreeState::ComboBoxAttachment _shapePickerAttachment;
    AudioProcessorValueTreeState::ComboBoxAttachment _modePickerAttachment;

    ModSliderComponent _tongueModXSlider;
    ModSliderComponent _tongueModYSlider;
    ModSliderComponent _constrictionModXSlider;
    ModSliderComponent _constrictionModYSlider;
    ModSliderComponent _pitchModSlider;
};

#endif /* LFOComponent_hpp */
