//
//  ADSRUI.hpp
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 2/8/23.
//

#ifndef ADSRUI_hpp
#define ADSRUI_hpp

#include <stdio.h>
#include "JuceHeader.h"

#define HANDLES_COUNT   (4)
#define HANDLE_WIDTH    (8)

class ADSRUI : public Component, public juce::AudioProcessorValueTreeState::Listener {
    
public:
    struct ParameterIdentifiers {
        juce::String initial;
        juce::String attack;
        juce::String peak;
        juce::String decay;
        juce::String sustain;
        juce::String release;
    };
    
    ADSRUI(AudioProcessorValueTreeState &state,
           ParameterIdentifiers identifiers);
    ~ADSRUI() override;
    
    void paint(Graphics &g) override;
    
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    
    
    void parameterChanged (const String& parameterID, float newValue) override;
    
private:
    void changeValuesForEvent(const MouseEvent &e);
    void repositionHandles();
    
    int highlightedHandleIndex = -1;
    int activeHandleIndex = -1;
    
    AudioProcessorValueTreeState *state;
    RangedAudioParameter *initialParam;
    RangedAudioParameter *peakParam;
    RangedAudioParameter *attackParam;
    RangedAudioParameter *decayParam;
    RangedAudioParameter *sustainParam;
    RangedAudioParameter *releaseParam;
    
    float attackTimeNormalized = 0.5;
    float decayTimeNormalized = 0.5;
    float sustainRatio = 0.5;
    float releaseTimeNormalized = 1.0;
    float initial = 0.0;
    float peak = 1.0;
    float end = 0.0;
    
    juce::Point<int> handles[HANDLES_COUNT];
};

#endif /* ADSRUI_hpp */
