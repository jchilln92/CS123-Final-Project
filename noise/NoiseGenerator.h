#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#define MAX_OCTAVES 5
#define NOISE_SIZE ((1<<MAX_OCTAVES)+1);

class NoiseGenerator
{
public:
    NoiseGenerator(float roughness, float persistence);
    virtual ~NoiseGenerator();

    virtual void generate(long seed) = 0;
    inline float *getNoise() { return m_noise; }

protected:
    float *m_noise;
    float m_roughness;
    float m_persistence;
};

#endif // NOISEGENERATOR_H
