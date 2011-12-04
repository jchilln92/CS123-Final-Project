#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include <QMouseEvent>
#include <QWheelEvent>

class Camera
{
public:
    Camera();
    ~Camera();

    Vector3 getEye() const { return m_eye; }
    Vector3 getLook() const { return m_look; }
    Vector3 getUp() const { return m_up; }

    Vector3 getU() const { return m_look.cross(m_up).unit(); }
    Vector3 getV() const { return getU().cross(m_look).unit(); }
    Vector3 getW() const { return -m_look.unit(); }

    void multMatrix();
    void lookAt(const Vector3 &eye, const Vector3 &look, const Vector3 &up);

    void mouseMove(const Vector2 &delta, const Qt::MouseButtons &buttons);
    void mouseWheel(float delta);

private:
    void filmPlaneTranslate(const Vector2 &delta);
    void lookVectorRotate(const Vector2 &delta);
    void lookVectorTranslate(float delta);

    Vector3 m_eye, m_look, m_up;
};

#endif // CAMERA_H
