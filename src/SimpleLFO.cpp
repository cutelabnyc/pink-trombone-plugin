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
{
    auto freqParam = instate.getParameter(_identifiers.frequency);
    _frequencyHz = freqParam->convertFrom0to1(freqParam->getValue());
    _type = typeForNormalizedParameterValue(
        instate.getParameter(_identifiers.type)->getValue()
    );

    instate.addParameterListener(_identifiers.frequency, this);
    instate.addParameterListener(_identifiers.type, this);

    auto self = this;
    _destructor = [&] {
        instate.removeParameterListener(_identifiers.frequency, self);
        instate.removeParameterListener(_identifiers.type, self);
    };
}

SimpleLFO::~SimpleLFO()
{
    _destructor();
}

void SimpleLFO::advanceOneSample()
{

}

void SimpleLFO::setSampleRate (double newSampleRate)
{

}

float SimpleLFO::value()
{

}

SimpleLFO::LFOType SimpleLFO::typeForNormalizedParameterValue(float value)
{
    int wholeValue = (int) roundf(value * LFOType::LENGTH);
    if (wholeValue >= LFOType::LENGTH) return LFOType::Sine;
    return (LFOType) wholeValue;
}

void SimpleLFO::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == _identifiers.frequency) {
        _frequencyHz = newValue;
    } else if (parameterID == _identifiers.type) {
        _type = typeForNormalizedParameterValue(newValue / ((float) LFOType::LENGTH));
    }
}