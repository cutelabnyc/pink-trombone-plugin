//
//  HiddenLookAndFeel.hpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 3/14/23.
//

#ifndef HiddenLookAndFeel_hpp
#define HiddenLookAndFeel_hpp

#include <stdio.h>
#include "JuceHeader.h"

class HiddenLookAndFeel : public juce::LookAndFeel_V4 {

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

#endif /* HiddenLookAndFeel_hpp */
