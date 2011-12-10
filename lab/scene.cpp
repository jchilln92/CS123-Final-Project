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

        int period = (int)body->getAxialPeriod();
        float rot = (m_time % period) / (float)period;
        rot *= M_2PI;
        body->setAxialRotation(rot);
        body->updatePosition(m_time);
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

void Scene::render(QGLShaderProgram *shader) {
    // set up a test light
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = {0.0, 0.0, 10.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    for (unsigned int i = 0; i < m_bodies.size(); i++) {
        Planet body = m_bodies.at(i);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, body.getTexture(0));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, body.getTexture(1));
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, body.getTexture(2));
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, body.getTexture(3));
        shader->setUniformValue("tex1", (GLuint)0);
        shader->setUniformValue("tex2", (GLuint)1);
        shader->setUniformValue("tex3", (GLuint)2);
        shader->setUniformValue("tex4", (GLuint)3);

        // load in other uniforms
        shader->setUniformValue("global_amp_scale",(GLfloat)0.05);
        shader->setUniformValue("global_pos_scale",(GLfloat)2.0);
        shader->setUniformValue("planet_seed",(GLuint)body.getSeed());
        shader->setUniformValue("noise_octaves",(GLuint)body.getOctaveCount());

        body.render();
    }

    glActiveTexture(GL_TEXTURE0); // make renderText work again
}
