//
//  Modulatable.h
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 3/10/22.
//

#ifndef Modulatable_h
#define Modulatable_h

#include <type_traits>
#include <vector>
#include "ModulationSource.h"
#include "JuceHeader.h"

template <typename RealType>
class ModulationStage {
    static_assert(std::is_floating_point<RealType>::value,
                  "class Modulatable can only be instantiated with floating point types");
public:
    ModulationStage(ModulationSource<RealType> *source);
    virtual RealType modulatedValue(RealType normalizedInputValue) = 0;
protected:
    ModulationSource<RealType> *_source;
};

template <typename RealType>
class AttenuvertedModulationStage: public ModulationStage<RealType>
{
public:
    AttenuvertedModulationStage(ModulationSource<RealType> *source, RangedAudioParameter *avParameter);
    RealType modulatedValue(RealType normalizedInputValue) override;
private:
    RangedAudioParameter *_avParameter;
};

class ModulatedAudioParameter {
public:
    ModulatedAudioParameter(RangedAudioParameter *parameter);

    float value();
    void appendModulationStage(ModulationSource<float> *source, RangedAudioParameter *avParameter);
private:
    RangedAudioParameter *_parameter;
    std::vector<std::unique_ptr<AttenuvertedModulationStage<float>>> _modulationStages;
};

#endif /* Modulatable_h */
