
#include "TrackballCamera.hpp"
#include "glm/gtc/type_ptr.hpp"

TrackballCamera::TrackballCamera()
    : m_Distance(5.f), m_AngleX(0.f), m_AngleY(0.f){};

void TrackballCamera::moveFront(float delta)
{
    m_Distance += delta;
};

void TrackballCamera::rotateLeft(float degrees)
{
    m_AngleY += degrees;
};
void TrackballCamera::rotateUp(float degrees)
{
    m_AngleX += degrees;
};
glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::mat4 matrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -m_Distance));
    matrix           = glm::rotate(matrix, glm::radians(m_AngleX), glm::vec3(1, 0, 0));
    matrix           = glm::rotate(matrix, glm::radians(m_AngleY), glm::vec3(0, 1, 0));

    return matrix;
};