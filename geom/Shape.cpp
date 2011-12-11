#include "Shape.h"
#include <iostream>
#include <qgl.h>

Shape::Shape(int param1, int param2, float param3) {
    m_parameter1 = param1;
    m_parameter2 = param2;
    m_parameter3 = param3;

    m_vertices = NULL;
    m_vertexNormals = NULL;
    m_triangles = NULL;
    m_textureCoordinates = NULL;
}

Shape::~Shape() {
    delete[] m_vertices;
    delete[] m_vertexNormals;
    delete[] m_triangles;
    delete[] m_textureCoordinates;
}

/* Parameters */

void Shape::setParam1(int param1) {
    int validated = validateParam1(param1);

    if (validated != m_parameter1) {
        m_parameter1 = validated;

        if (dependsOnParam1())
            calculateGeometry();
    }
}

void Shape::setParam2(int param2) {
    int validated = validateParam2(param2);

    if (validated != m_parameter2) {
        m_parameter2 = validated;

        if (dependsOnParam2())
            calculateGeometry();
    }
}

void Shape::setParam3(float param3) {
    float validated = validateParam3(param3);

    if (validated != m_parameter3) {
        m_parameter3 = validated;

        if (dependsOnParam3())
            calculateGeometry();
    }
}

int Shape::validateParam1(int param1) { return param1; }
int Shape::validateParam2(int param2) { return param2; }
float Shape::validateParam3(float param3) { return param3; }

bool Shape::dependsOnParam1() { return false; }
bool Shape::dependsOnParam2() { return false; }
bool Shape::dependsOnParam3() { return false; }

/* Geometry */

void Shape::initializeContainers() {
    delete[] m_triangles;
    delete[] m_vertices;
    delete[] m_vertexNormals;
    delete[] m_textureCoordinates;

    m_triangles = new int[3 * getTriangleCount()];
    m_vertices = new float[4 * getVertexCount()];
    m_vertexNormals = new float[4 * getVertexCount()];
    m_textureCoordinates = new float[2 * getVertexCount()];
}

void Shape::setVertex(int v_idx, float x, float y, float z) {
    m_vertices[v_idx * 4 + 0] = x;
    m_vertices[v_idx * 4 + 1] = y;
    m_vertices[v_idx * 4 + 2] = z;
    m_vertices[v_idx * 4 + 3] = 1;

    float u, v;
    computeTextureCoordinates(&m_vertices[v_idx * 4], &u, &v);
    //std::cout << m_vertices[v_idx * 4] << ", " << m_vertices[v_idx * 4 + 1] << ", " << m_vertices[v_idx * 4 + 2] << std::endl;
    //std::cout << u << ", " << v << std::endl;
    m_textureCoordinates[v_idx * 2 + 0] = u;
    m_textureCoordinates[v_idx * 2 + 1] = v;
}

void Shape::setVertexNormal(int v_idx, float x, float y, float z) {
    m_vertexNormals[v_idx * 4 + 0] = x;
    m_vertexNormals[v_idx * 4 + 1] = y;
    m_vertexNormals[v_idx * 4 + 2] = z;
    m_vertexNormals[v_idx * 4 + 3] = 1;
}

void Shape::setTriangle(int t_idx, int v1, int v2, int v3) {
    m_triangles[t_idx * 3 + 0] = v1;
    m_triangles[t_idx * 3 + 1] = v2;
    m_triangles[t_idx * 3 + 2] = v3;
}

/* Drawing */

void Shape::render() {
    glBegin(GL_TRIANGLES);

    int i;
    for (i = 0; i < getTriangleCount(); i++) {
        int *triangle = &m_triangles[i * 3];
        float *v1 = &m_vertices[triangle[0] * 4];
        float *v2 = &m_vertices[triangle[1] * 4];
        float *v3 = &m_vertices[triangle[2] * 4];
        float *n1 = &m_vertexNormals[triangle[0] * 4];
        float *n2 = &m_vertexNormals[triangle[1] * 4];
        float *n3 = &m_vertexNormals[triangle[2] * 4];

        glNormal3f(n1[0], n1[1], n1[2]);
        glVertex3f(v1[0], v1[1], v1[2]);
        glNormal3f(n2[0], n2[1], n2[2]);
        glVertex3f(v2[0], v2[1], v2[2]);
        glNormal3f(n3[0], n3[1], n3[2]);
        glVertex3f(v3[0], v3[1], v3[2]);
    }

    glEnd();
}
