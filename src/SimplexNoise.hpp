//
//  SimplexNoise.hpp
//  PinkTrombone - VST3
//
//  Created by Samuel Tarakajian on 8/29/19.
//

#ifndef noise_hpp
#define noise_hpp

#include <stdio.h>

class SimplexNoise {
public:
    typedef struct Grad {
        double x;
        double y;
        double z;
    } Grad;

    double simplex1(double xin);
    double simplex2(double xin, double yin);

private:
    static inline double dot2(Grad *gin, double x, double y) {
        return gin->x * x + gin->y * y;
    }

    static Grad grad3[];
    static short p[]; // permutation

    void seed(short seed);

    int perm[512];
    Grad gradP[512];
    bool didseed = false;
};

#endif /* noise_hpp */
