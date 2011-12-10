#ifndef ORBIT_H
#define ORBIT_H

#include "math/vector.h"

class Orbit {
public:
    Orbit();
    Orbit(float xamp, float yamp, float zamp, float freq);
    virtual ~Orbit();

    Vector3 getPositionAtTime(float t);
private:
    float m_xamp, m_zamp, m_yamp;
    float m_freq;
};

#endif // ORBIT_H
