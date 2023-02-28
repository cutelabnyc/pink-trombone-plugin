//
//  ModLookAndFeel.hpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#ifndef ModLookAndFeel_hpp
#define ModLookAndFeel_hpp

#include <stdio.h>
#include "JuceHeader.h"

class ModLookAndFeel : public juce::LookAndFeel_V4 {

public:
    void drawLinearSlider( 	Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle,
        Slider &s
    ) override;
};

#endif /* ModLookAndFeel_hpp */
