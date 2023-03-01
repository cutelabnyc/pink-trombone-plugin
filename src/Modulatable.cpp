//
//  Modulatable.cpp
//  PinkTrombone - Shared Code
//
//  Created by Samuel Tarakajian on 3/10/22.
//

#include "Modulatable.h"

template <typename RealType>
ModulationStage<RealType>::ModulationStage(ModulationSource<RealType> *source): _source(source) { }

template <typename RealType>
AttenuvertedModulationStage<RealType>::AttenuvertedModulationStage(
    ModulationSource<RealType> *source,
    RangedAudioParameter *avParameter)
: ModulationStage<RealType>(source)
, _avParameter(avParameter) { }

template <typename RealType>
RealType AttenuvertedModulationStage<RealType>::modulatedValue(RealType normalizedInputValue)
{
    auto avValue = (_avParameter->getValue() - 0.5) * 2;
    auto sourceValue = this->_source->value();
    if (avValue == 0) {
        return normalizedInputValue;
    } else if (avValue > 0) {
        RealType upperRange = fminf(0.5f, 1.0f - normalizedInputValue);
        return normalizedInputValue + upperRange * avValue * sourceValue;
    } else {
        RealType lowerRange = fminf(0.5f, normalizedInputValue);
        return normalizedInputValue + lowerRange * avValue * sourceValue;
    }
}

ModulatedAudioParameter::ModulatedAudioParameter(RangedAudioParameter *parameter): _parameter(parameter) {}

float ModulatedAudioParameter::value()
{
    float normalizedInput = _parameter->getValue();
    for (auto & modulation : _modulationStages) {
        normalizedInput = modulation.get()->modulatedValue(normalizedInput);
    }
    return normalizedInput;
}

void ModulatedAudioParameter::appendModulationStage(ModulationSource<float> *source, RangedAudioParameter *avParameter)
{
    _modulationStages.push_back(
        std::make_unique<AttenuvertedModulationStage<float>>(source, avParameter)
    );
}
