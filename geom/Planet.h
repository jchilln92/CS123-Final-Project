#ifndef PLANET_H
#define PLANET_H

#include "math/vector.h"
#include "MeshDetail.h"
class Sphere;

class Planet {
public:
    Planet();
    virtual ~Planet();

    void setDetail(MeshDetail detail);
    void render();
private:
    Vector3 m_center;
    float m_radius;
    MeshDetail m_renderDetail;

    // sphere meshes at different detail levels
    Sphere *m_vl_sphere;
    Sphere *m_l_sphere;
    Sphere *m_m_sphere;
    Sphere *m_h_sphere;
    Sphere *m_vh_sphere;
};

#endif // PLANET_H
