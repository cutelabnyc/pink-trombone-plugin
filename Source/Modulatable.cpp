//
//  Modulatable.cpp
//  PinkTrombone - Shared Code
//
//  Created by Samuel Tarakajian on 3/10/22.
//

#include "Modulatable.h"

template <typename RealType>
Modulation<RealType>::Modulation(ModulationSource<RealType> *m)
    : _source(m)
    , active(false)
    , scale(0.0)
{};

template <typename RealType>
ModulationSource<RealType> Modulation<RealType>::source()
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
    RealType modulatedValue = _rootValue;
    for (auto m: _modulationSources) {
        if (m->active) {
            if (m->scale > 0.0) {
                RealType range = (_maximum - modulatedValue);
                modulatedValue += range * m->scale * m->source()->value;
            } else if (m->scale < 0.0) {
                RealType range = (modulatedValue - _minimum);
                modulatedValue -= range * m->scale * m->source()->value;
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
    _modulationSources.push_back(m);
}

template <typename RealType>
ModulationSource<RealType> *Modulatable<RealType>::modulationAtIndex(int i)
{
    return _modulationSources[i];
}

template <typename RealType>
int Modulatable<RealType>::numModulations()
{
    return _modulationSources.size();
}
