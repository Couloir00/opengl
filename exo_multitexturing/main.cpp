
#include <vector>
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

int main()
{
    int  window_width  = 1280;
    int  window_height = 720;
    auto ctx           = p6::Context{{1280, 720, "TP_SPHERE"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    /*INITIALIZATION CODE*/
    p6::Shader Shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normal.fs.glsl");

    GLint uMVPMatrixID    = glGetUniformLocation(Shader.id(), "uMVPMatrix");
    GLint uMVMatrixID     = glGetUniformLocation(Shader.id(), "uMVMatrix");
    GLint uNormalMatrixID = glGetUniformLocation(Shader.id(), "uNormalMatrix");
    GLint uTex3DID        = glGetUniformLocation(Shader.id(), "uTex3D");
    // test de profondeur du GPU
    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ((float)window_width / (float)window_height), 0.1f, 100.f);
    glm::mat4 MVMatrix     = glm::mat4(1.f);
    MVMatrix               = glm::translate(MVMatrix, {0, 0, -5});
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

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
    std::vector<glm::vec3> axes;
    std::vector<glm::vec3> position;
    axes.reserve(32);
    position.reserve(32);
    for (int i = 0; i < 32; i++)
    {
        axes.emplace_back(glm::ballRand(2.f));
        position.emplace_back(glm::ballRand(3.f));
    }

    // texture
    img::Image earth = p6::load_image_buffer("assets/EarthMap.jpg");
    GLuint     earth_texture;
    glGenTextures(1, &earth_texture);
    glBindTexture(GL_TEXTURE_2D, earth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earth.width(), earth.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, earth.data());
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
        glClearColor(0.000f, 0.f, 0.f, 1.000f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Shader.use();

        glBindVertexArray(vao);

        glUniformMatrix4fv(uMVPMatrixID, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uNormalMatrixID, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(uMVMatrixID, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earth_texture);
        glUniform1i(uTex3DID, 0);
        glDrawArrays(GL_TRIANGLES, 0, sphere.size());
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, moon_texture);
        glUniform1i(uTex3DID, 1);

        for (int i = 0; i < 32; i++)
        {
            glm::mat4 MoonMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
            MoonMatrix           = glm::rotate(MoonMatrix, ctx.time(), axes[i]);
            MoonMatrix           = glm::translate(MoonMatrix, {position[i].x, position[i].y, position[i].z});
            MoonMatrix           = glm::scale(MoonMatrix, glm::vec3{0.2f});

            glUniformMatrix4fv(uMVPMatrixID, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MoonMatrix));
            glUniformMatrix4fv(uNormalMatrixID, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniformMatrix4fv(uMVMatrixID, 1, GL_FALSE, glm::value_ptr(MoonMatrix));

            glDrawArrays(GL_TRIANGLES, 0, sphere.size());
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteTextures(1, &earth_texture);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    return 0;
}