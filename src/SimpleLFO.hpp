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
#include "SimplexNoise.hpp"
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
        juce::String shape;
        juce::String mode;
    };
    
    SimpleLFO(AudioProcessorValueTreeState &state,
           ParameterIdentifiers identifiers);
    ~SimpleLFO() override;
    void advanceOneSample() noexcept;
    void setSampleRate (double newSampleRate) noexcept;
    void setFrequency(float newFrequency);
    void setDuty(float duty);
    float value() override;
    void parameterChanged (const String& parameterID, float newValue) override;
private:
    LFOType typeForNormalizedParameterValue(float value);
    void _recalculatePhaseDelta();

    AudioProcessorValueTreeState *state;
    ParameterIdentifiers _identifiers;
    SimplexNoise noise;
    float _duty = 0.5;
    float _phaseDelta = 0.0;
    float _frequencyHz = 1.0;
    float _phase = 0.0;
    double _totalPhase = 0.0;
    LFOType _type = LFOType::Sine;
    double _sampleRate = 44100.0;
    std::function<void()> _destructor;
    juce::dsp::LookupTableTransform<float> _sineLookupTable;
};

#endif /* PinkTromboneADSR_hpp */