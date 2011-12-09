#include "scene.h"
#include "qgl.h"

Scene::Scene() {}
Scene::~Scene() {}

void Scene::addBody(Planet& body) {
    m_bodies.push_back(body);
}

std::vector<Planet>& Scene::getBodies() {
    return m_bodies;
}

void Scene::render() {
}
