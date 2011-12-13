#ifndef ORBIT_H
#define ORBIT_H

#include "math/vector.h"

class Orbit {
public:
    Orbit();
    Orbit(float xamp, float tilt, float zamp);
    virtual ~Orbit();

    Vector3 getPositionAtAngle(float radians);
private:
    float m_xamp, m_zamp, m_tilt;
};

#endif // ORBIT_H
