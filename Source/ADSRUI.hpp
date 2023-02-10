//
//  ADSRUI.hpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#ifndef ADSRUI_hpp
#define ADSRUI_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class ADSRUI : public Component {
    
    void paint(Graphics &g) override;
    
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    
private:
    int highlightedHandleIndex = -1;
    
    float attackTimeNormalized = 0.0;
    float decayTimeNormalized = 0.0;
    float sustainRatio = 1.0;
    float releaseTimeNormalized = 0.0;
    float initial = 0.0;
    float peak = 1.0;
    float end = 0.0;
    
};

#endif /* ADSRUI_hpp */
