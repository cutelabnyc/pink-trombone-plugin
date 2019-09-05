//
//  WhiteNoise.hpp
//  PinkTrombone - All
//
//  Created by Samuel Tarakajian on 8/30/19.
//

#ifndef WhiteNoise_hpp
#define WhiteNoise_hpp

#include <stdio.h>

class WhiteNoise {
public:
	WhiteNoise(long sampleLength);
	~WhiteNoise();
	double runStep();
private:
	long index;
	double *buffer;
	long size;
};

#endif /* WhiteNoise_hpp */
