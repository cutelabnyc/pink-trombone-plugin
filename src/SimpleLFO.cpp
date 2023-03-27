//
//  SimpleLFO.cpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 3/15/2023.
//

#include "SimpleLFO.hpp"

SimpleLFO::SimpleLFO(AudioProcessorValueTreeState &instate,
               ParameterIdentifiers identifiers)
    : state(&instate)
    , _identifiers(identifiers)
    , _sineLookupTable([] (float x) { return sinf(M_PI * 2.0 * x); }, 0.0f, 1.0f, 5096)
{
    auto freqParam = instate.getParameter(_identifiers.frequency);
    _frequencyHz = freqParam->convertFrom0to1(freqParam->getValue());
    _type = typeForNormalizedParameterValue(
        instate.getParameter(_identifiers.shape)->getValue()
    );

    instate.addParameterListener(_identifiers.frequency, this);
    instate.addParameterListener(_identifiers.shape, this);

    _destructor = [this] {
        state->removeParameterListener(_identifiers.frequency, this);
        state->removeParameterListener(_identifiers.shape, this);
    };
}

SimpleLFO::~SimpleLFO()
{
    _destructor();
}

void SimpleLFO::advanceOneSample() noexcept
{
    _phase += _phaseDelta;
    _totalPhase += _phaseDelta;

    if (_phase >= 1) {
        _phase = fmodf(_phase, 1.0);
    } else if (_phase < 0) {
        _phase = 1.0 + fmodf(_phase, 1.0);
    }
}

void SimpleLFO::setSampleRate (double newSampleRate) noexcept
{
    _sampleRate = fmax(1.0, newSampleRate); // no negative sample rates or anything crazy
    _recalculatePhaseDelta();
}

void SimpleLFO::setFrequency(float newFrequencyHz)
{
    _frequencyHz = newFrequencyHz;
    _recalculatePhaseDelta();
}

void SimpleLFO::setDuty(float duty)
{
    _duty = duty;
}

float SimpleLFO::value()
{
    switch (_type) {
        case Sine:
            return _sineLookupTable.processSampleUnchecked(_phase);
        case Slope:
            return 2.0f * _phase - 1.0f;
        case Square:
            return _phase < _duty ? 1.0f : -1.0f;
        case Triangle:
            if (_duty <= 0.0f) {
                return -1.0f;
            } else if (_duty >= 1.0f || _phase == _duty) {
                return 1.0f;
            } else if (_phase < _duty) {
                return (_phase / _duty) * 2.0f - 1.0;
            } else {
                return (1.0f - ((_phase - _duty) / (1.0f - _duty))) * 2.0 - 1.0;;
            }
        case Noise:
            return noise.simplex1(_totalPhase);
        default:
            return 1.0;
    }
}

void SimpleLFO::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == _identifiers.frequency) {
        _frequencyHz = newValue;
        _recalculatePhaseDelta();
    } else if (parameterID == _identifiers.shape) {
        _type = typeForNormalizedParameterValue(newValue / ((float) LFOType::LENGTH));
    }
}

SimpleLFO::LFOType SimpleLFO::typeForNormalizedParameterValue(float value)
{
    int wholeValue = (int) roundf(value * LFOType::LENGTH);
    if (wholeValue >= LFOType::LENGTH) return LFOType::Sine;
    return (LFOType) wholeValue;
}

void SimpleLFO::_recalculatePhaseDelta()
{
    _phaseDelta = _frequencyHz / _sampleRate;
}
