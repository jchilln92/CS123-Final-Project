#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "NoiseGenerator.h"

class PerlinNoise : public NoiseGenerator
{
public:
    PerlinNoise(float roughness, float persistence);
    virtual ~PerlinNoise();

    void generate(long seed);
};

#endif // PERLINNOISE_H
