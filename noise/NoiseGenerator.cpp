#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator(float roughness, float persistence) {
    m_noise = new float[NOISE_SIZE*NOISE_SIZE*NOISE_SIZE];
    m_roughness = roughness;
    m_persistence = persistence;
}

NoiseGenerator::~NoiseGenerator() {
    delete[] m_noise;
}
