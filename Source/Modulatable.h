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

template <typename RealType>
class Modulation {
    static_assert(std::is_floating_point<RealType>::value,
                  "class Modulatable can only be instantiated with floating point types");
public:
    Modulation(ModulationSource<RealType> *m);
    ModulationSource<RealType> source();
    RealType scale;
    bool active;
private:
    ModulationSource<RealType> *_source;
};

template <typename RealType>
class Modulatable {
    static_assert(std::is_floating_point<RealType>::value,
                  "class Modulatable can only be instantiated with floating point types");
public:
    Modulatable(RealType initial, RealType minimum, RealType maximum);
    RealType value();
    RealType rootValue();
    void setRootValue(RealType value);
    void appendModulationSource(ModulationSource<RealType> *m);
    ModulationSource<RealType> *modulationAtIndex(int i);
    int numModulations();
private:
    std::vector<ModulationSource<RealType> *> _modulationSources;
    RealType _rootValue;
    RealType _minimum;
    RealType _maximum;
};

#endif /* Modulatable_h */
