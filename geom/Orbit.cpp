#include "Orbit.h"
#include "math.h"

Orbit::Orbit() {
    m_xamp = 0;
    m_tilt = 0;
    m_zamp = 0;
}

Orbit::Orbit(float xamp, float tilt, float zamp) {
    m_xamp = xamp;
    m_tilt = tilt;
    m_zamp = zamp;
}

Orbit::~Orbit() {}

Vector3 Orbit::getPositionAtAngle(float radians) {
    return Vector3(m_xamp * cos(m_tilt) * sin(radians),
                   m_xamp * sin(m_tilt) * sin(radians),
                   m_zamp * cos(radians));
}
