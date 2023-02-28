//
//  ModLookAndFeel.cpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#include <stdio.h>
#include "ModLookAndFeel.hpp"

void ModLookAndFeel::drawLinearSlider(
    Graphics &g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const Slider::SliderStyle style,
    Slider &s)
{
    if (s.getValue() == 0.0) {
        g.setColour(Colours::grey);
        g.fillRect(x, y, width, height);
    }
    else if (s.getValue() > 0) {
        g.setColour(Colours::green);
        g.fillRect((int) (x + 0.5 * width), y, width * (s.getValue() / 2.0), height);
    } else {
        g.setColour(Colours::green);
        g.fillRect((int) sliderPos, y, -((int) ((s.getValue() / 2.0) * width)), height);
    }
    
    g.setColour(Colours::white);
    g.drawRect(x, y, width, height);
}
