#include "FreeflyCamera.hpp"
#include <cmath>
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

FreeflyCamera::FreeflyCamera()
    : m_Position{0.}, m_Phi(glm::pi<float>()), m_Theta(0.f)
{
    computeDirectionVectors();
};

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(glm::cos((m_Theta)) * glm::sin((m_Phi)), glm::sin((m_Theta)), glm::cos((m_Theta)) * glm::cos((m_Phi)));
    m_LeftVector  = glm::vec3(glm::sin(m_Phi + (glm::pi<float>() / 2.f)), 0, glm::cos(m_Phi + (glm::pi<float>() / 2.)));
    m_UpVector    = glm::cross(m_FrontVector, m_LeftVector);
};

void FreeflyCamera::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
};

void FreeflyCamera::moveFront(float t)
{
    m_Position += t * m_FrontVector;
};

void FreeflyCamera::rotateLeft(float degrees)
{
    m_Phi += glm::radians(degrees);
    computeDirectionVectors();
};

void FreeflyCamera::rotateUp(float degrees)
{
    m_Theta += glm::radians(degrees);
    computeDirectionVectors();
};

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}