#ifndef PLANET_H
#define PLANET_H

#include "math/vector.h"
#include "MeshDetail.h"
#include <qgl.h>
#include <QGLShaderProgram>
#include "Orbit.h"
class Sphere;

#define MAX_PLANET_TEXTURES 4

class Planet {
public:
    static void initStaticResources();

    Planet(Vector3 center = Vector3(0, 0, 0), Vector3 axis = Vector3(0, 1, 0), float radius = .5);
    virtual ~Planet();

    void setDetail(MeshDetail detail);
    MeshDetail getDetail() { return m_renderDetail; }
    void render();

    void setTexture(const char *filename, int i);
    GLuint getTexture(int i);

    void updatePosition(float t);

    Vector3 getAxis() { return m_axis; }
    Vector3 getCenter() { return m_center; }
    float getRadius() { return m_radius; }
    float getAxialRotation() { return m_axialRotation; }
    float getAxialPeriod() { return 18000.0 * m_radius; }
    float getOrbitalPeriod() { return 10000.0 * sqrt(m_center.length()); }
    bool getIsStar() { return m_isStar; }
    void setIsStar(bool s) { m_isStar = s; }
    void setRadius(float radius) { m_radius = radius; }
    void setOrbit(Orbit orbit) { m_orbit = orbit; m_center = m_orbit.getPositionAtAngle(0); }
    void setAxis(Vector3 axis) { m_axis = axis; m_axis.normalize(); }
    void setAxialRotation(float radians) { m_axialRotation = fmod(radians, M_2PI); }
    void setOrbitalRotation(float radians);
    void calculateNewOrbitalPosition(int numTicks);
    void setTerrainAmplitude(float amp) { m_terrainAmplitude = amp; }
    void setHasWater(bool hasWater) { m_hasWater = hasWater; }

    int getSeed() { return m_seed; }
    int getOctaveCount() { return m_octaveCount; }
private:
    bool m_isStar;
    Orbit m_orbit;
    Vector3 m_center;

    Vector3 m_axis;
    float m_axialRotation;
    float m_orbitalRotation;

    float m_radius;
    MeshDetail m_renderDetail;
    int m_seed;
    int m_octaveCount;
    float m_terrainAmplitude;
    bool m_hasWater;

    GLuint textures[MAX_PLANET_TEXTURES];

    // sphere meshes at different detail levels
    // these are used as the basis for every planet
    static bool staticInitialized;
    static Sphere *m_vl_sphere;
    static Sphere *m_l_sphere;
    static Sphere *m_m_sphere;
    static Sphere *m_h_sphere;
    static Sphere *m_vh_sphere;

    // shader programs for planets and stars
    static QGLShaderProgram *m_planetShader;
    static QGLShaderProgram *m_starShader;
};

#endif // PLANET_H
