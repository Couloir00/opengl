#pragma once

// #include <GL/glew.h>

#include <glm/glm.hpp>
#include "p6/p6.h"

class ShadowMapFBO {
private:
    unsigned int m_width  = 0;
    unsigned int m_height = 0;
    GLuint       m_fbo;
    GLuint       m_shadowMap;

public:
    ShadowMapFBO() = default;
    ~ShadowMapFBO();

    bool Init(unsigned int width, unsigned int height);

    void BindForWriting();

    void BindForReading(GLenum TextureUnit);
};