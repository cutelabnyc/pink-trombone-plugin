//
//  Modulatable.cpp
//  PinkTrombone - Shared Code
//
//  Created by Samuel Tarakajian on 3/10/22.
//

#include "Modulatable.h"

template <typename RealType>
Modulation<RealType>::Modulation(ModulationSource<RealType> *m)
    : scale(0.0)
    , active(false)
    , _source(m)
{};

template <typename RealType>
ModulationSource<RealType> *Modulation<RealType>::source()
{
    return _source;
}

template <typename RealType>
Modulatable<RealType>::Modulatable(RealType initial, RealType minimum, RealType maximum)
    : _rootValue(initial)
    , _minimum(minimum)
    , _maximum(maximum)
{};

template <typename RealType>
RealType Modulatable<RealType>::value() {
    RealType modulatedValue = rootValue();
    for (auto m: _modulationSources) {
        if (m->active) {
            if (m->scale > 0.0) {
                RealType range = (_maximum - modulatedValue);
                modulatedValue += range * m->scale * m->source()->value();
            } else if (m->scale < 0.0) {
                RealType range = (modulatedValue - _minimum);
                modulatedValue += range * m->scale * m->source()->value();
            }
        }
    }
    return modulatedValue;
}

template <typename RealType>
RealType Modulatable<RealType>::rootValue() {
    return _rootValue;
}

template <typename RealType>
void Modulatable<RealType>::setRootValue(RealType value)
{
    _rootValue = value;
}

template <typename RealType>
void Modulatable<RealType>::appendModulationSource(ModulationSource<RealType> *m)
{
    _modulationSources.push_back(new Modulation<RealType>(m));
}

template <typename RealType>
Modulation<RealType> *Modulatable<RealType>::modulationAtIndex(int i)
{
    return _modulationSources[i];
}

template <typename RealType>
unsigned long Modulatable<RealType>::numModulations()
{
    return _modulationSources.size();
}

ModulatableAudioParameter::ModulatableAudioParameter(AudioParameterFloat *f)
    : Modulatable<float>(f->get(), f->range.start, f->range.end)
    , _parameter(f)
{}

float ModulatableAudioParameter::value()
{
    float modulatedValue = this->Modulatable::value();
    
    return _parameter->convertFrom0to1(modulatedValue);
}

float ModulatableAudioParameter::rootValue()
{
    return _parameter->convertTo0to1(_parameter->get());
}

void ModulatableAudioParameter::setRootValue(float value)
{
    _parameter->setValueNotifyingHost(_parameter->convertFrom0to1(value));
}


template class Modulatable<float>;
template class Modulatable<double>;
