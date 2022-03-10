//
//  PinkTromboneADSR.hpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 3/3/22.
//

#ifndef PinkTromboneADSR_hpp
#define PinkTromboneADSR_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "ModulationSource.h"
#include <math.h>

class PinkTromboneADSR: ModulationSource<float>
{
public:
    PinkTromboneADSR();
    virtual ~PinkTromboneADSR() = default;
    
    enum class State { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE };

    struct Parameters
    {
        float attack        = 0.1f;     /**< Attack time in seconds */
        float decay         = 0.1f;     /**< Decay time in seconds */
        float sustain       = 1.0f;     /**< Sustain height. 1.0 is full height */
        float release       = 0.1f;     /**< Release time in seconds */
        float attackExp     = 0.0f;     /**<  Curvature of the attack phase */
        float decayExp      = 0.0f;     /**< Curvature of the decay phase */
        float releaseExp    = 0.0f;     /**< Curvature of the release phase */
    };

    void setParameters (const Parameters& newParameters);
    bool isActive() const noexcept;
    void setSampleRate (double newSampleRate) noexcept;
    void reset() noexcept;
    void noteOn() noexcept;
    void noteOff() noexcept;
    void advanceOneSample() noexcept;
    float value() override;

private:
    void advanceState() noexcept;
    void calculateRate(float start, float end) noexcept;

    State state = State::IDLE;
    Parameters parameters;
    
    float rate = 0.0f;
    float duration = 0.0f;
    float progress = 0.0f;

    bool trigger = false;
    double sampleRate = 44100.0;
    double invSampleRate = 1.0 / 44100.0;
    float envelopeVal = 0.0f;
    float scaledEnvelopeVal = 0.0f;
    float attackRate = 0.0f;
    float decayRate = 0.0f;
    float releaseRate = 0.0f;
    float _value = 0.0f;
};

#endif /* PinkTromboneADSR_hpp */
