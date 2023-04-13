#include <vector>
#include "glimac/FreeflyCamera.hpp"
#include "glimac/ShadowFbo.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct ShadowProgram {
    p6::Shader m_Program;
    GLint      uMVPLight;

    ShadowProgram()
        : m_Program(p6::load_shader("shaders/shadowMap.vs.glsl", "shaders/shadowMap.fs.glsl"))
    {
        uMVPLight = glGetUniformLocation(m_Program.id(), "uMVPLight");
    }
};

struct EarthProgram {
    p6::Shader m_Program;

    std::vector<glimac::ShapeVertex> m_sphere;

    GLint uMVPMatrixID;
    GLint uMVMatrixID;
    GLint uNormalMatrixID;

    GLint uKd;
    GLint uKs;
    GLint uShininess;

    GLint uLightDir_vs;
    GLint uLightIntensity;

    // Shadow
    GLint uMVPLight;
    GLint uShadowMap;

    EarthProgram()
        : m_Program(p6::load_shader("shaders/3D.vs.glsl", "shaders/directionalLight.fs.glsl"))
    {
        uMVPMatrixID    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrixID     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrixID = glGetUniformLocation(m_Program.id(), "uNormalMatrix");

        uKd        = glGetUniformLocation(m_Program.id(), "uKd");
        uKs        = glGetUniformLocation(m_Program.id(), "uKs");
        uShininess = glGetUniformLocation(m_Program.id(), "uShininess");

        uLightDir_vs    = glGetUniformLocation(m_Program.id(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(m_Program.id(), "uLightIntensity");

        // Shadow
        uMVPLight  = glGetUniformLocation(m_Program.id(), "uMVPLight");
        uShadowMap = glGetUniformLocation(m_Program.id(), "uShadowMap");
    }
    void render()
    {
        glDrawArrays(GL_TRIANGLES, 0, m_sphere.size());
    }

    void uniformSender(glm::mat4 camera, glm::mat4 ProjMatrix, glm::vec3 LightDir, float Rotation)
    {
        glm::mat4 MVMatrix     = camera;
        MVMatrix               = glm::rotate(MVMatrix, Rotation, glm::vec3(0, 1, 0));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(uMVMatrixID, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixID, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(uMVPMatrixID, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));

        glUniform3fv(uKd, 1, glm::value_ptr(glm::vec3(0.428f, 0.1638f, 0.8326f)));
        glUniform3fv(uKs, 1, glm::value_ptr(glm::vec3(0.294f, 0.567f, 0.4162f)));
        glUniform1f(uShininess, 0.6);

        glUniform3fv(uLightDir_vs, 1, glm::value_ptr(LightDir));
        glUniform3fv(uLightIntensity, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));

        glUniform1i(uShadowMap, 0);
    }
};

struct MoonProgram {
    p6::Shader m_Program;
    GLint      uMVPMatrixID;
    GLint      uMVMatrixID;
    GLint      uNormalMatrixID;

    GLint uKd;
    GLint uKs;
    GLint uShininess;

    GLint uLightDir_vs;
    GLint uLightIntensity;

    // Shadow
    GLint uMVPLight;
    GLint uShadowMap;

    MoonProgram()
        : m_Program(p6::load_shader("shaders/3D.vs.glsl", "shaders/directionalLight.fs.glsl"))
    {
        uMVPMatrixID    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrixID     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrixID = glGetUniformLocation(m_Program.id(), "uNormalMatrix");

        uKd        = glGetUniformLocation(m_Program.id(), "uKd");
        uKs        = glGetUniformLocation(m_Program.id(), "uKs");
        uShininess = glGetUniformLocation(m_Program.id(), "uShininess");

        uLightDir_vs    = glGetUniformLocation(m_Program.id(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(m_Program.id(), "uLightIntensity");

        // Shadow
        uMVPLight  = glGetUniformLocation(m_Program.id(), "uMVPLight");
        uShadowMap = glGetUniformLocation(m_Program.id(), "uShadowMap");
    }
};

int main()
{
    int  window_width  = 1280;
    int  window_height = 720;
    auto ctx           = p6::Context{{1280, 720, "TP_SHADOW"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    /*INITIALIZATION CODE*/
    EarthProgram earthProgram;
    MoonProgram  moonProgram;

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Creation sphere
    const std::vector<glimac::ShapeVertex> sphere = glimac::sphere_vertices(1.f, 32, 16);
    glBufferData(GL_ARRAY_BUFFER, sphere.size() * sizeof(glimac::ShapeVertex), sphere.data(), GL_STATIC_DRAW);

    // test de profondeur du GPU
    glEnable(GL_DEPTH_TEST);

    // VAO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION  = 0;
    const GLuint VERTEX_ATTR_NORMAL    = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;

    // Vertex Attribute
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //----------------------MATRICES-------------------

    FreeflyCamera ViewMatrixCamera = FreeflyCamera();

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (float)window_width / (float)window_height, 0.1f, 100.f);
    glm::mat4 MVMatrix   = glm::mat4(1.f);
    MVMatrix             = glm::translate(MVMatrix, {0, 0, -5});
    glm::mat4 NormalMatrix;

    glm::mat4 moonMVMatrix;
    glm::mat4 moonNormalMatrix;

    std::vector<glm::vec3> axes;
    std::vector<glm::vec3> direction;
    std::vector<glm::vec3> Kd;
    std::vector<glm::vec3> Ks;
    std::vector<float>     Shininess;
    axes.reserve(32);
    direction.reserve(32);
    for (int i = 0; i < 32; i++)
    {
        axes.emplace_back(glm::ballRand(2.f));
        direction.emplace_back(glm::linearRand(0, 1), glm::linearRand(0, 1), glm::linearRand(0, 1));
        Kd.emplace_back(glm::linearRand(0.f, 0.5f), glm::linearRand(0.f, 0.5f), glm::linearRand(0.f, 0.5f));
        Ks.emplace_back(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f));
        Shininess.emplace_back(glm::linearRand(0.f, 1.f));
    }
    bool Z = false;
    bool S = false;
    bool Q = false;
    bool D = false;

    //-------------------init shadow--------------

    ShadowMapFBO  shadowMap;
    ShadowProgram shadowProgram;
    shadowMap.Init(window_width, window_height);

    // what is this
    glEnable(GL_POLYGON_OFFSET_FILL);
    float factor = 1.f;
    float units  = 1.f;
    glPolygonOffset(factor, units);

    // ShadowMVP
    glm::vec3 Light(0, 2, 2);

    // position light
    FreeflyCamera ViewMatrixLight = FreeflyCamera();

    glm::mat4 ProjMatrixLight    = glm::perspective(glm::radians(70.f), (float)window_width / (float)window_height, 0.1f, 25.f);
    glm::mat4 shadowOrthoProjMat = glm::ortho(-35.f, 35.f, -35.f, 35.f, -35.f, 35.f);

    glm::vec3 uLightDir = glm::vec3(glm::rotate(ViewMatrixCamera.getViewMatrix(), ctx.time(), glm::vec3(0, 1, 0)) * glm::vec4(Light, 0));
    glm::vec3 uLightPos = glm::vec3(glm::rotate(ViewMatrixCamera.getViewMatrix(), ctx.time(), glm::vec3(0, 1, 0)) * glm::vec4(Light, 1));

    ViewMatrixLight.rotateLeft(-glm::degrees(ctx.delta_time()));

    //------------------end init shadow-------------

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        if (Z)
        {
            ViewMatrixCamera.moveFront(0.1);
        }
        if (S)
        {
            ViewMatrixCamera.moveFront(-0.1);
        }
        if (Q)
        {
            ViewMatrixCamera.rotateUp(-0.1);
        }
        if (D)
        {
            ViewMatrixCamera.rotateLeft(-0.1);
        }

        //********************************************
        //                  SHADOW MAP PASS
        //********************************************

        shadowMap.BindForWriting();
        glClear(GL_DEPTH_BUFFER_BIT);
        // m_shadowMapTech.Enable(); = glUseProgram()
        shadowProgram.m_Program.use();

        // do this for every object
        // WorldView Matrix comes from the object itself
        glBindVertexArray(vao);
        {
            glm::mat4 MVMatrix = ViewMatrixLight.getViewMatrix();
            MVMatrix           = glm::rotate(MVMatrix, -ctx.time(), glm::vec3(0, 1, 0));
            glUniformMatrix4fv(shadowProgram.uMVPLight, 1, GL_FALSE, glm::value_ptr(shadowOrthoProjMat * MVMatrix));
        }

        earthProgram.render();

        glBindVertexArray(0);

        // do this for every object
        // WorldView Matrix comes from the object itself

        // glm::mat4 World = m_pMesh1->GetWorldMatrix();

        // // ViewMatrix from the light point of view
        // glm::mat4 LightView;
        // glm::vec3 Up(0.0f, 1.0f, 0.0f);
        // LightView.InitCameraTransform(m_spotLight.WorldPosition, m_spotLight.WorldDirection, Up); // Init Camera transform combines translation and rotation matrix

        // // MVP Matrix for the light source
        // glm::mat4 MVP = m_lightPersProjMatrix * LightView * World;
        // m_shadowMapTech.SetMVP(MVP);
        // m_pMesh1->Render();

        // if the terrain is rough we should render it into the shadow map as well

        //********************************************

        //********************************************
        //                   LIGHTING PASS
        //********************************************

        // see 11min video
        ctx.render_to_main_canvas();

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //  change
        glViewport(0, 0, ctx.current_canvas_width(), ctx.current_canvas_height());
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        earthProgram.m_Program.use();

        shadowMap.BindForReading(GL_TEXTURE0);
        {
            glm::mat4 MVMatrix = ViewMatrixLight.getViewMatrix();
            MVMatrix           = glm::rotate(MVMatrix, -ctx.time(), glm::vec3(0, 1, 0));
            glUniformMatrix4fv(earthProgram.uMVPLight, 1, GL_FALSE, glm::value_ptr(shadowOrthoProjMat * MVMatrix));
        }

        earthProgram.uniformSender(ViewMatrixCamera.getViewMatrix(), ProjMatrix, uLightDir, -ctx.time());
        earthProgram.render();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        // here

        //********************************************

        //     moonProgram.m_Program.use();
        //     for (int i = 0; i < 32; i++)
        //     {
        //         moonMVMatrix     = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
        //         moonMVMatrix     = ViewMatrixCamera.getViewMatrix();
        //         moonMVMatrix     = glm::rotate(moonMVMatrix, ctx.time(), axes[i]);
        //         moonMVMatrix     = glm::translate(moonMVMatrix, {direction[i].x, direction[i].y, direction[i].z});
        //         moonMVMatrix     = glm::scale(moonMVMatrix, glm::vec3{0.2});
        //         moonNormalMatrix = glm::transpose(glm::inverse(moonMVMatrix));

        //         glUniformMatrix4fv(moonProgram.uMVMatrixID, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
        //         glUniformMatrix4fv(moonProgram.uNormalMatrixID, 1, GL_FALSE, glm::value_ptr(moonNormalMatrix));
        //         glUniformMatrix4fv(moonProgram.uMVPMatrixID, 1, GL_FALSE, glm::value_ptr(ProjMatrix * moonMVMatrix));

        //         glUniform3fv(moonProgram.uKd, 1, glm::value_ptr(Kd[i]));
        //         glUniform3fv(moonProgram.uKs, 1, glm::value_ptr(Ks[i]));
        //         glUniform1f(moonProgram.uShininess, Shininess[i]);

        //         glUniform3fv(moonProgram.uLightDir_vs, 1, glm::value_ptr(glm::vec3(glm::vec4(1, 1, 1, 0) * glm::rotate(glm::mat4(1.0f), ctx.time(), glm::vec3(0, 1, 0)) * ViewMatrixCamera.getViewMatrix())));
        //         glUniform3fv(moonProgram.uLightIntensity, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));

        //         glDrawArrays(GL_TRIANGLES, 0, sphere.size());
        //     }
        //     glBindVertexArray(0);
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
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    return 0;
};
