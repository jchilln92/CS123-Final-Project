#ifndef PLANET_H
#define PLANET_H

#include "math/vector.h"
#include "MeshDetail.h"
class Sphere;

class Planet {
public:
    static void initStaticResources();

    Planet();
    virtual ~Planet();

    void setDetail(MeshDetail detail);
    void render();
private:
    Vector3 m_center;
    float m_radius;
    MeshDetail m_renderDetail;

    // sphere meshes at different detail levels
    // these are used as the basis for every planet
    static bool staticInitialized;
    static Sphere *m_vl_sphere;
    static Sphere *m_l_sphere;
    static Sphere *m_m_sphere;
    static Sphere *m_h_sphere;
    static Sphere *m_vh_sphere;
};

#endif // PLANET_H
