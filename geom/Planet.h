#ifndef PLANET_H
#define PLANET_H

#include "math/vector.h"
#include "MeshDetail.h"
#include <qgl.h>
class Sphere;

#define MAX_PLANET_TEXTURES 4

class Planet {
public:
    static void initStaticResources();

    Planet();
    Planet(Vector3 center, float radius);
    virtual ~Planet();

    void setDetail(MeshDetail detail);
    void render();

    void setTexture(const char *filename, int i);
    GLuint getTexture(int i);

    Vector3 getCenter() { return m_center; }
    float getRadius() { return m_radius; }
private:
    Vector3 m_center;
    float m_radius;
    MeshDetail m_renderDetail;

    GLuint textures[MAX_PLANET_TEXTURES];

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
