//
//  PinkTromboneADSR.cpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 3/3/22.
//

#include "PinkTromboneADSR.hpp"

PinkTromboneADSR::PinkTromboneADSR()
{
    setSampleRate (44100.0);
    setParameters (Parameters());
}

void PinkTromboneADSR::setParameters (const Parameters& newParameters)
{
    parameters = newParameters;
    
    if (state == State::ATTACK) {
        calculateRate(envelopeVal, 1.0f);
    } else if (state == State::DECAY || state == State::RELEASE) {
        calculateRate(envelopeVal, 0.0f);
    }
}
    
bool PinkTromboneADSR::isActive() const noexcept  { return state != State::IDLE; }

void PinkTromboneADSR::setSampleRate (double newSampleRate) noexcept
{
    jassert (newSampleRate > 0.0);
    sampleRate = newSampleRate;
    invSampleRate = 1.0 / sampleRate;
}

void PinkTromboneADSR::reset() noexcept
{
    envelopeVal = 0.0f;
    scaledEnvelopeVal = 0.0f;
    state = State::IDLE;
}
    
void PinkTromboneADSR::noteOn() noexcept
{
    reset();
    trigger = true;
    advanceState();
}
    
void PinkTromboneADSR::noteOff() noexcept
{
    trigger = false;
    advanceState();
}

void PinkTromboneADSR::advanceOneSample() noexcept
{
    if (state == State::IDLE) {
        this->_value = 0.0f;
        return;
    }
    
    if (state == State::SUSTAIN) {
        this->_value =  parameters.sustain;
        return;
    }
    
    if (state == State::ATTACK || state == State::DECAY || state == State::RELEASE) {
        float exponent = 1.0f, scale = 1.0f, offset = 0.0f;
        progress += invSampleRate;
        envelopeVal += rate;
        
        if (state == State::ATTACK) {
            exponent = parameters.attackExp;
            offset = parameters.initial;
            scale = (parameters.peak - parameters.initial);
        } else if (state == State::DECAY) {
            exponent = parameters.decayExp;
            scale = (parameters.peak - parameters.sustain);
            offset = parameters.sustain;
        } else if (state == State::RELEASE) {
            exponent = parameters.releaseExp;
            scale = parameters.sustain;
        }
        
        scaledEnvelopeVal = envelopeVal;
        if (exponent != 0.0f) {
            if (exponent > 0.0f) scaledEnvelopeVal = powf(envelopeVal, exponent + 1);
            else scaledEnvelopeVal = powf(envelopeVal, (-1.0f / (-1.0f + exponent)));
        }
        
        scaledEnvelopeVal *= scale;
        scaledEnvelopeVal += offset;
        
        if (progress >= duration)
            advanceState();
    }
    
    this->_value = scaledEnvelopeVal;
}

float PinkTromboneADSR::value() {
    return this->_value;
}

void PinkTromboneADSR::calculateRate(float start, float end) noexcept
{
    switch (state) {
        case State::ATTACK:
            duration = parameters.attack / 1000.0f;
            break;
        case State::DECAY:
            duration = parameters.decay / 1000.0f;
            break;
        case State::RELEASE:
            duration = parameters.release / 1000.0f;
            break;
        default:
            duration = -1.0f;
    }
    duration *= fabs(end - start);
    rate = duration > 0.0f ? (float) ((end - start) / (duration * sampleRate)) : -1.0f;
    progress = 0.0f;
}

void PinkTromboneADSR::advanceState() noexcept
{
    switch (state) {
        case State::IDLE:
            if (trigger) {
                state = State::ATTACK;
                envelopeVal = 0.0f;
                calculateRate(0.0f, 1.0f);
                if (progress >= duration) advanceState();
            }
            break;
        case State::ATTACK:
            if (progress >= duration) {
                state = State::DECAY;
                envelopeVal = 1.0f;
                calculateRate(1.0f, 0.0f);
                if (progress >= duration) advanceState();
            }
            break;
        case State::DECAY:
            if (progress >= duration) {
                state = State::SUSTAIN;
                rate = 0.0f;
                advanceState();
            }
            break;
        case State::SUSTAIN:
            if (!trigger) {
                state = State::RELEASE;
                envelopeVal = 1.0f;
                calculateRate(1.0f, 0.0f);
                if (progress >= duration) advanceState();
            }
            break;
        case State::RELEASE:
            if (progress >= duration) {
                state = State::IDLE;
                rate = 0.0f;
                advanceState();
            }
            break;
    }
}
