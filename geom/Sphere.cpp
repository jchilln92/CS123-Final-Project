#include "Sphere.h"
#include "math.h"

Sphere::Sphere(int param1, int param2) : Shape(param1, param2, 0) {
    m_parameter1 = validateParam1(param1);
    m_parameter2 = validateParam2(param2);
}

Sphere::~Sphere() {}

int Sphere::getTriangleCount() {
    return (m_parameter1 - 1) * 2 * m_parameter2;
}

int Sphere::getVertexCount() {
    return 2 + (m_parameter1 - 1) * m_parameter2;
}

void Sphere::calculateGeometry() {
    Shape::initializeContainers();

    int v_idx = 0;
    int t_idx = 0;

    float r = .5;
    float phi, theta;
    int t_step, p_step;
    float d_phi = M_PI / m_parameter1;
    float d_theta = 2 * M_PI / m_parameter2;

    // top and bottom vertices
    Shape::setVertex(v_idx, 0, r, 0);
    Shape::setVertexNormal(v_idx, 0, 1, 0);
    v_idx++;
    Shape::setVertex(v_idx, 0, -r, 0);
    Shape::setVertexNormal(v_idx, 0, -1, 0);
    v_idx++;

    // build all other vertices
    for (t_step = 0; t_step < m_parameter2; t_step++) {
        for (p_step = 1; p_step < m_parameter1; p_step++) { // exclude top and bottom vertices
            theta = t_step * d_theta;
            phi = p_step * d_phi;

            float x = r * sin(phi) * cos(theta);
            float y = r * cos(phi);
            float z = r * sin(phi) * sin(theta);

            Shape::setVertex(v_idx, x, y, z);
            Shape::setVertexNormal(v_idx, x/r, y/r, z/r);
            v_idx++;;
        }
    }

    for (t_step = 0; t_step < m_parameter2; t_step++) {
        for (p_step = 1; p_step < m_parameter1 - 1; p_step++) {
            v_idx = 2 + (m_parameter1 - 1) * t_step + (p_step - 1);

            // at each vertex, we are interested in creating triangles with 3 other vertices
            int below_idx = v_idx + 1;
            int left_idx = v_idx + m_parameter1 - 1;
            int diag_idx = v_idx + m_parameter1;

            // in order to "wrap around" fully, we need this check
            if (t_step == m_parameter2 - 1) {
                left_idx = 2 + (p_step - 1);
                diag_idx = 2 + (p_step - 1) + 1;
            }

            Shape::setTriangle(t_idx, v_idx, left_idx, below_idx);
            t_idx++;
            Shape::setTriangle(t_idx, left_idx, diag_idx, below_idx);
            t_idx++;
        }
    }

    for (t_step = 0; t_step < m_parameter2; t_step++) {
        // top circle
        v_idx = 2 + (m_parameter1 - 1) * t_step;
        int left_idx = t_step == m_parameter2 - 1 ? 2 : v_idx + m_parameter1 - 1;

        Shape::setTriangle(t_idx, 0, left_idx, v_idx);
        t_idx++;

        // bottom circle
        v_idx += m_parameter1 - 2; // move to lowest ring of vertices
        left_idx = t_step == m_parameter2 - 1 ? m_parameter1 : v_idx + m_parameter1 - 1;

        Shape::setTriangle(t_idx, left_idx, 1, v_idx);
        t_idx++;
    }
}
