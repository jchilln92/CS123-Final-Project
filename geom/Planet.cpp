#include "Planet.h"
#include "Sphere.h"

#define PARAM_VERY_LOW  10
#define PARAM_LOW       25
#define PARAM_MEDIUM    40
#define PARAM_HIGH      75
#define PARAM_VERY_HIGH 150

bool Planet::staticInitialized = false;
Sphere* Planet::m_vl_sphere = NULL;
Sphere* Planet::m_l_sphere = NULL;
Sphere* Planet::m_m_sphere = NULL;
Sphere* Planet::m_h_sphere = NULL;
Sphere* Planet::m_vh_sphere = NULL;

void Planet::initStaticResources() {
    if (staticInitialized) return;

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

Planet::Planet() {
    initStaticResources();

    // set default mesh level
    m_renderDetail = MEDIUM;
}

Planet::~Planet() {}

void Planet::setDetail(MeshDetail detail) {
    m_renderDetail = detail;
}

void Planet::render() {
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
        m_vh_sphere->render();
        break;
    }
}
