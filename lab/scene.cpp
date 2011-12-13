#include <QGLShaderProgram>
#include "scene.h"
#include "support/camera.h"
#include "qgl.h"

Scene::Scene() {}
Scene::~Scene() {}

void Scene::addBody(Planet& body) {
    m_bodies.push_back(body);
}

std::vector<Planet>& Scene::getBodies() {
    return m_bodies;
}

void Scene::doTick() {
    m_time++;

    for (unsigned int i = 0; i < m_bodies.size(); i++) {
        Planet *body = &m_bodies.at(i);
        if (body->getIsStar()) continue;

        int axialPeriod = (int)body->getAxialPeriod();
        float arot = (m_time % axialPeriod) / (float)axialPeriod;
        arot *= M_2PI;
        body->setAxialRotation(arot);
        body->calculateNewOrbitalPosition();
    }
}

void Scene::updateBodyDetails(Camera *cam) {
    for (unsigned int i = 0; i < m_bodies.size(); i++) {
        Planet *body = &m_bodies.at(i);

        // determine what level the planet should be rendered at
        Vector3 eye_to_planet = body->getCenter() - cam->getEye();
        float dist = eye_to_planet.length();
        dist -= body->getRadius();

        if (dist >= 0 && dist < .9) {
            body->setDetail(VERY_HIGH);
        } else if (dist >= .9 && dist < 1.5) {
            body->setDetail(HIGH);
        } else if (dist >= 1.5 && dist < 2.5) {
            body->setDetail(MEDIUM);
        } else if (dist >= 2.5 && dist < 4) {
            body->setDetail(LOW);
        } else {
            body->setDetail(VERY_LOW);
        }
    }
}

void Scene::render() {
    // set up a test light
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    for (unsigned int i = 0; i < m_bodies.size(); i++) {
        Planet *body = &m_bodies.at(i);
        body->render();
    }

    glActiveTexture(GL_TEXTURE0); // make renderText work again
}
