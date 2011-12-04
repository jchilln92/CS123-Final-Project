#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator(float roughness, float persistence) {
        int arr_size = NOISE_SIZE*NOISE_SIZE*NOISE_SIZE;
    m_noise = new float[arr_size];
    m_roughness = roughness;
    m_persistence = persistence;
}

NoiseGenerator::~NoiseGenerator() {
    delete[] m_noise;
}
