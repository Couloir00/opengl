#include <vector>
#include "glimac/TrackballCamera.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct EarthProgram {
    p6::Shader m_Program;
    GLint      uMVPMatrixID;
    GLint      uMVMatrixID;
    GLint      uNormalMatrixID;
    GLint      uEarthTexture;
    GLint      uCloudTexture;

    EarthProgram()
        : m_Program(p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl"))
    {
        uMVPMatrixID    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrixID     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrixID = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uEarthTexture   = glGetUniformLocation(m_Program.id(), "uEarthTexture ");
        uCloudTexture   = glGetUniformLocation(m_Program.id(), "uCloudTexture");
    }
};

struct MoonProgram {
    p6::Shader m_Program;

    GLint uMVPMatrixID;
    GLint uMVMatrixID;
    GLint uNormalMatrixID;
    GLint uMoonTexture;

    MoonProgram()
        : m_Program(p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl"))
    {
        uMVPMatrixID    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrixID     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrixID = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uMoonTexture    = glGetUniformLocation(m_Program.id(), "uMoonTexture");
    }
};

int main()
{
    int  window_width  = 1280;
    int  window_height = 720;
    auto ctx           = p6::Context{{1280, 720, "TP_MULTI"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    /*INITIALIZATION CODE*/
    EarthProgram earthProgram;
    MoonProgram  moonProgram;

    // test de profondeur du GPU
    glEnable(GL_DEPTH_TEST);

    // earth MVP
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ((float)window_width / (float)window_height), 0.1f, 100.f);
    glm::mat4 MVMatrix   = glm::mat4(1.f);
    MVMatrix             = glm::translate(MVMatrix, {0, 0, -5});
    glm::mat4 NormalMatrix;

    // Creation sphere
    const std::vector<glimac::ShapeVertex> sphere = glimac::sphere_vertices(1.f, 32, 16);

    // VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.size() * sizeof(glimac::ShapeVertex), sphere.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION  = 0;
    const GLuint VERTEX_ATTR_NORMAL    = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Vertex Attribute
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // moon
    std::vector<glm::vec3> axes;
    std::vector<glm::vec3> position;
    axes.reserve(32);
    position.reserve(32);
    for (int i = 0; i < 32; i++)
    {
        axes.emplace_back(glm::ballRand(2.f));
        position.emplace_back(glm::linearRand(0, 1), glm::linearRand(0, 1), glm::linearRand(0, 1));
    }

    // camera
    TrackballCamera ViewMatrix;
    bool            Z = false;
    bool            S = false;
    bool            Q = false;
    bool            D = false;

    // texture
    img::Image earth = p6::load_image_buffer("assets/EarthMap.jpg");
    GLuint     earth_texture;
    glGenTextures(1, &earth_texture);
    glBindTexture(GL_TEXTURE_2D, earth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earth.width(), earth.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, earth.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    img::Image cloud = p6::load_image_buffer("assets/CloudMap.jpg");
    GLuint     cloud_texture;
    glGenTextures(1, &cloud_texture);
    glBindTexture(GL_TEXTURE_2D, cloud_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloud.width(), cloud.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, cloud.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    img::Image moon = p6::load_image_buffer("assets/MoonMap.jpg");
    GLuint     moon_texture;
    glGenTextures(1, &moon_texture);
    glBindTexture(GL_TEXTURE_2D, moon_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moon.width(), moon.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, moon.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        if (Z)
        {
            ViewMatrix.moveFront(0.1);
        }
        if (S)
        {
            ViewMatrix.moveFront(-0.1);
        }
        if (Q)
        {
            ViewMatrix.rotateUp(-0.1);
        }
        if (D)
        {
            ViewMatrix.rotateLeft(-0.1);
        }

        glClearColor(0.000f, 0.f, 0.f, 1.000f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw the Earth
        glBindVertexArray(vao);

        earthProgram.m_Program.use();
        glUniform1i(earthProgram.uEarthTexture, 0);
        glUniform1i(earthProgram.uCloudTexture, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earth_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cloud_texture);

        MVMatrix     = ViewMatrix.getViewMatrix();
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(earthProgram.uMVMatrixID, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrixID, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(earthProgram.uMVPMatrixID, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));

        glDrawArrays(GL_TRIANGLES, 0, sphere.size());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        // draw the Moons
        moonProgram.m_Program.use();
        glUniform1i(moonProgram.uMoonTexture, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moon_texture);

        for (int i = 0; i < 32; i++)
        {
            glm::mat4 moonMVMatrix     = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
            moonMVMatrix               = ViewMatrix.getViewMatrix();
            moonMVMatrix               = glm::rotate(moonMVMatrix, ctx.time(), axes[i]);
            moonMVMatrix               = glm::translate(moonMVMatrix, {position[i].x, position[i].y, position[i].z});
            moonMVMatrix               = glm::scale(moonMVMatrix, glm::vec3{0.2});
            glm::mat4 moonNormalMatrix = glm::transpose(glm::inverse(moonMVMatrix));

            glUniformMatrix4fv(moonProgram.uMVMatrixID, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uNormalMatrixID, 1, GL_FALSE, glm::value_ptr(moonNormalMatrix));
            glUniformMatrix4fv(moonProgram.uMVPMatrixID, 1, GL_FALSE, glm::value_ptr(ProjMatrix * moonMVMatrix));

            glDrawArrays(GL_TRIANGLES, 0, sphere.size());
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);
    };

    ctx.key_pressed = [&Z, &S, &Q, &D](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = true;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = true;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = true;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = true;
        }
    };

    ctx.key_released = [&Z, &S, &Q, &D](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = false;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = false;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = false;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = false;
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteTextures(1, &earth_texture);
    glDeleteTextures(1, &cloud_texture);
    glDeleteTextures(1, &moon_texture);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    return 0;
}