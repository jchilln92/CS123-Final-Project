#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "geom/Planet.h"
class QGLShaderProgram;
class Camera;

class Scene {
public:
    Scene();
    virtual ~Scene();

    void addBody(Planet& body);
    std::vector<Planet>& getBodies();

    void updateBodyDetails(Camera *cam);
    void render();
    void doTicks(int numTicks);

    void clear();
private:
    int m_time; // the scene time, in ticks
    std::vector<Planet> m_bodies;
};

#endif // SCENE_H
