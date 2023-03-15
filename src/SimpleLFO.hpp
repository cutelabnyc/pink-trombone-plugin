//
//  SimpleLFO.hpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 3/15/2023.
//

#ifndef SimpleLFO_hpp
#define SimpleLFO_hpp

#include "JuceHeader.h"
#include "ModulationSource.h"
#include <math.h>

class SimpleLFO: public ModulationSource<float>, public juce::AudioProcessorValueTreeState::Listener
{
public:
    enum LFOType {
        Sine = 0,
        Slope,
        Square,
        Triangle,
        Noise,
        LENGTH
    };

    struct ParameterIdentifiers {
        juce::String frequency;
        juce::String type;
    };
    
    SimpleLFO(AudioProcessorValueTreeState &state,
           ParameterIdentifiers identifiers);
    ~SimpleLFO() override;
    void advanceOneSample() noexcept;
    void setSampleRate (double newSampleRate) noexcept;
    float value() override;
    void parameterChanged (const String& parameterID, float newValue) override;
private:
    LFOType typeForNormalizedParameterValue(float value);

    AudioProcessorValueTreeState *state;
    ParameterIdentifiers _identifiers;
    float _frequencyHz = 1.0;
    float _phase = 0.0;
    float _value = 0.0;
    LFOType _type = LFOType::Sine;
    double _sampleRate = 44100.0;
    std::function<void()> _destructor;
};

#endif /* PinkTromboneADSR_hpp */