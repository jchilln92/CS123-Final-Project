#ifndef PERLINNOISE_H
#define PERLINNOISE_H

class PerlinNoise
{
public:
    PerlinNoise(float roughness, float persistence);
    virtual ~PerlinNoise();

    void generate(long seed);
};

#endif // PERLINNOISE_H
