#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "geom/Planet.h"

class Scene {
public:
    Scene();
    virtual ~Scene();

    void addBody(Planet& body);
    std::vector<Planet>& getBodies();

    void render();
private:
    std::vector<Planet> m_bodies;
};

#endif // SCENE_H
