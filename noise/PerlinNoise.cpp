#include "PerlinNoise.h"
#include "vector.h"

#include <math.h>
#include <cstdlib>
#include <map>

struct vec_comp {
    bool operator()(const Vector3 lhs, const Vector3 rhs) const {
        return lhs.x < rhs.x || lhs.y < rhs.y || lhs.z < rhs.z;
    }
};

float interp(float pos, float lpos, float hpos, float lval, float hval) {
    if (pos == lpos) { return lval; }
    else if (pos == hpos) { return hval; }
    else {
        float npos = (pos-lpos)/(hpos-lpos);
        float mpos = npos*npos*(3-2*npos);
        return lval*(1-mpos)+hval*(mpos);
    }
}

PerlinNoise::PerlinNoise(float roughness, float persistence) : NoiseGenerator(roughness, persistence) {}

PerlinNoise::~PerlinNoise() {}

void PerlinNoise::generate(long seed) {
    srand(seed);
    // create internal height maps
    std::map<Vector3,Vector3,vec_comp> *disp_cubes[MAX_OCTAVES];
    for (int i = 0; i < MAX_OCTAVES; i++) {
        disp_cubes[i] = new std::map<Vector3,Vector3,vec_comp>();
    }

    // generation loop
    float frequency;
    for (int i = 0; i < MAX_OCTAVES; i++) {
        frequency = 1/pow(2,i+2);
        for (int x = 0; x <= 1; x += frequency) {
            for (int y = 0; y <= 1; y += frequency) {
                for (int z = 0; z <= 1; z += frequency) {
                    disp_cubes[i]->insert(std::pair<Vector3,Vector3>(Vector3(x,y,z),Vector3::uniform()));
                }
            }
        }
    }

    // create final height map
    for (int i = 0; i < MAX_OCTAVES; i++) {
        float grain = 1/pow(2,i+2);
        float amp = pow(m_roughness,i/m_persistence)*2;
        float resx,lowx,highx,normx,resy,lowy,highy,normy,resz,lowz,highz,normz,inter1,inter2,inter3,inter4;
        for (int x = 0; x < NOISE_SIZE; x++) {
            for (int y = 0; y < NOISE_SIZE; y++) {
                for (int z = 0; z < NOISE_SIZE; z++) {
                    // find surrounding points
                    resx = x/(NOISE_SIZE*grain);
                    resy = y/(NOISE_SIZE*grain);
                    resz = z/(NOISE_SIZE*grain);

                    lowx = floor(resx)*grain; highx = ceil(resx)*grain;
                    lowy = floor(resy)*grain; highy = ceil(resy)*grain;
                    lowz = floor(resz)*grain; highz = ceil(resz)*grain;

                    if (lowx == highx) {
                        normx = 0;
                    } else {
                        normx = (resx*grain-lowx)/(highx-lowx);
                    }
                    if (lowy == highy) {
                        normy = 0;
                    } else {
                        normy = (resy*grain-lowy)/(highy-lowy);
                    }
                    if (lowy == highz) {
                        normz = 0;
                    } else {
                        normz = (resz*grain-lowz)/(highz-lowz);
                    }

                    // interpolate
                    inter1 = interp((float)x/NOISE_SIZE,lowx,highx,
                                    Vector3(normx,normy,normz).dot((*disp_cubes[i])[Vector3(lowx,lowy,lowz)]),
                                    Vector3(normx-1,normy,normz).dot((*disp_cubes[i])[Vector3(highx,lowy,lowz)]));
                    inter2 = interp((float)x/NOISE_SIZE,lowx,highx,
                                    Vector3(normx,normy-1,normz).dot((*disp_cubes[i])[Vector3(lowx,highy,lowz)]),
                                    Vector3(normx-1,normy-1,normz).dot((*disp_cubes[i])[Vector3(highx,highy,lowz)]));
                    inter3 = interp((float)y/NOISE_SIZE,lowy,highy,inter1,inter2);

                    inter1 = interp((float)x/NOISE_SIZE,lowx,highx,
                                    Vector3(normx,normy,normz-1).dot((*disp_cubes[i])[Vector3(lowx,lowy,highz)]),
                                    Vector3(normx-1,normy,normz-1).dot((*disp_cubes[i])[Vector3(highx,lowy,highz)]));
                    inter2 = interp((float)x/NOISE_SIZE,lowx,highx,
                                    Vector3(normx,normy-1,normz-1).dot((*disp_cubes[i])[Vector3(lowx,highy,highz)]),
                                    Vector3(normx-1,normy-1,normz-1).dot((*disp_cubes[i])[Vector3(highx,highy,highz)]));
                    inter4 = interp((float)y/NOISE_SIZE,lowy,highy,inter1,inter2);

                    if (i == 0) {
                        m_noise[x*NOISE_SIZE*NOISE_SIZE+y*NOISE_SIZE+z] = interp((float)z/NOISE_SIZE,lowz,highz,inter3,inter4)*amp;
                    } else {
                        m_noise[x*NOISE_SIZE*NOISE_SIZE+y*NOISE_SIZE+z] += interp((float)z/NOISE_SIZE,lowz,highz,inter3,inter4)*amp;
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_OCTAVES; i++) {
        delete disp_cubes[i];
    }
}
