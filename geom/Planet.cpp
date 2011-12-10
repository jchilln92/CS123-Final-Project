#include "Planet.h"
#include "Sphere.h"
#include "support/resourceloader.h"
#include <qgl.h>

#define PARAM_VERY_LOW  15
#define PARAM_LOW       30
#define PARAM_MEDIUM    60
#define PARAM_HIGH      120
#define PARAM_VERY_HIGH 240

bool Planet::staticInitialized = false;
Sphere* Planet::m_vl_sphere = NULL;
Sphere* Planet::m_l_sphere = NULL;
Sphere* Planet::m_m_sphere = NULL;
Sphere* Planet::m_h_sphere = NULL;
Sphere* Planet::m_vh_sphere = NULL;

void Planet::initStaticResources() {
    if (staticInitialized) return;

    // initialize random generator
    srand(time(NULL));

    // precalculate spheres at different mesh levels
    m_vl_sphere = new Sphere(PARAM_VERY_LOW, PARAM_VERY_LOW);
    m_vl_sphere->calculateGeometry();
    m_l_sphere = new Sphere(PARAM_LOW, PARAM_LOW);
    m_l_sphere->calculateGeometry();
    m_m_sphere = new Sphere(PARAM_MEDIUM, PARAM_MEDIUM);
    m_m_sphere->calculateGeometry();
    m_h_sphere = new Sphere(PARAM_HIGH, PARAM_HIGH);
    m_h_sphere->calculateGeometry();
    m_vh_sphere = new Sphere(PARAM_VERY_HIGH, PARAM_VERY_HIGH);
    m_vh_sphere->calculateGeometry();

    staticInitialized = true;
}

Planet::Planet(Vector3 center, Vector3 axis, float radius) {
    initStaticResources();

    m_renderDetail = MEDIUM; // set default mesh level
    m_radius = radius;
    m_center = center;
    m_axis = axis;
    m_axialRotation = 0;

    m_seed = rand();
    m_octaveCount = 4;
}

Planet::~Planet() {
}

void Planet::setTexture(const char *filename, int i) {
    if (i < MAX_PLANET_TEXTURES) {
        //glDeleteTextures(1, &textures[i]);
        textures[i] = ResourceLoader::loadTextureImage(filename);
    }
}

GLuint Planet::getTexture(int i) {
    return textures[i];
}

void Planet::setDetail(MeshDetail detail) {
    m_renderDetail = detail;
}

void Planet::render() {
    glPushMatrix();

    float rad2deg = 180.0/M_PI;

    // move the planet into place
    glTranslatef(m_center.x, m_center.y, m_center.z);

    // rotate top into position, rotate around axis
    float zangle = acos(m_axis.dot(Vector3(0, 1, 0)));
    glRotatef(zangle * rad2deg, 0, 0, 1);
    float xangle = asin(m_axis.dot(Vector3(0, 0, 1)));
    glRotatef(xangle * rad2deg, 1, 0, 0);

    glRotatef(m_axialRotation * rad2deg, 0, 1, 0);

    // scale the planet to the right size
    float scale = m_radius / .5;
    glScalef(scale, scale, scale);

    switch (m_renderDetail) {
    case VERY_LOW:
        m_vl_sphere->render();
        break;
    case LOW:
        m_l_sphere->render();
        break;
    case MEDIUM:
        m_m_sphere->render();
        break;
    case HIGH:
        m_h_sphere->render();
        break;
    case VERY_HIGH:
        std::cout << "very high" << std::endl;
        m_vh_sphere->render();
        break;
    }

    glPopMatrix();
}
