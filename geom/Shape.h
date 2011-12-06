#ifndef SHAPE_H
#define SHAPE_H

class Shape {
public:
    Shape(int param1, int param2, float param3);
    virtual ~Shape();

    void setParam1(int param1);
    void setParam2(int param2);
    void setParam3(float param3);

    virtual void calculateGeometry() = 0;
    virtual void render();
protected:
    // tesselation parameters
    int m_parameter1;
    int m_parameter2;
    float m_parameter3;

    // return adjusted values for the parameters.  can be overridden to impose arbitrary
    // constraints over the parameter values.  by default these simply pass back the value given
    virtual int validateParam1(int param1);
    virtual int validateParam2(int param2);
    virtual float validateParam3(float param3);

    // can be overridden to indicate whether the geometry needs to be recomputed
    // when each of these parameters has changed. all default to false
    virtual bool dependsOnParam1();
    virtual bool dependsOnParam2();
    virtual bool dependsOnParam3();

    // geometry information
    float *m_vertices; // 1d array, 4 entries per vertex
    float *m_vertexNormals; // 1d array, 4 entries per normal, indices match m_vertices
    float *m_textureCoordinates; // 1d array, 2 entries per tex coord, indices match m_vertices
    int *m_triangles; // 3 entries per triangle, indices into m_vertices

    virtual int getTriangleCount() = 0;
    virtual int getVertexCount() = 0;
    virtual void computeTextureCoordinates(float *pos, float *u, float *v) = 0;
    void initializeContainers();

    void setVertex(int v_idx, float x, float y, float z);
    void setVertexNormal(int v_idx, float x, float y, float z);
    void setTriangle(int t_idx, int v1, int v2, int v3);
};

#endif // SHAPE_H
