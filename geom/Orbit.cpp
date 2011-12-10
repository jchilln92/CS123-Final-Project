#include "Orbit.h"
#include "math.h"

Orbit::Orbit() {
    m_xamp = 0;
    m_yamp = 0;
    m_zamp = 0;
    m_freq = 0;
}

Orbit::Orbit(float xamp, float yamp, float zamp, float freq) {
    m_xamp = xamp;
    m_yamp = yamp;
    m_zamp = zamp;
    m_freq = freq;
}

Orbit::~Orbit() {}

Vector3 Orbit::getPositionAtTime(float t) {
    return Vector3(m_xamp * cos(m_freq * t),
                   m_yamp * cos(m_freq * t),
                   m_zamp * sin(m_freq * t));
}
