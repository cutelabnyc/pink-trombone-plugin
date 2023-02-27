//
//  ModulationSource.h
//  PinkTrombone
//
//  Created by Samuel Tarakajian on 3/10/22.
//

#ifndef ModulationSource_h
#define ModulationSource_h

#include <type_traits>

template <typename RealType>
class ModulationSource {
    static_assert(std::is_floating_point<RealType>::value,
                  "class can only be instantiated with floating point types");
public:
    virtual RealType value() = 0;
    virtual ~ModulationSource() = default;
};

#endif /* ModulationSource_h */
