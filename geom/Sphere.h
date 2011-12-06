#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
    Sphere(int param1, int param2);
    virtual ~Sphere();

    void calculateGeometry();
    void render();
protected:
    int getTriangleCount();
    int getVertexCount();
    virtual void computeTextureCoordinates(float *pos, float *u, float *v);

    int validateParam1(int param1) { return param1 < 2 ? 2 : param1; }
    int validateParam2(int param2) { return param2 < 3 ? 3 : param2; }
    bool dependsOnParam1() { return true; }
    bool dependsOnParam2() { return true; }
};

#endif // SPHERE_H
