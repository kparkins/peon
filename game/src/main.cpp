/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include <vld.h>

#include <iostream>
#include <unordered_map>

#ifdef _WIN32
#define APIENTRY __stdcall
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "graphics/GLProgram.h"
#include "graphics/GLContext.h"
#include "graphics/GLWindow.h"
#include "log/StdoutStream.h"
#include "profile/Profile.h"
#include "Peon.h"

using std::cout;
using std::endl;
using std::cerr;
using std::unordered_map;

using namespace Peon;
using namespace glm;

GLWindow* mainWindow;

static float triangleVertices[18] = {
        0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.5f, 0.f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f
};


class EventBusContainer {

};

template<typename EventGroup, typename EventBusContainer>
class EventBus {

public:


protected:

    EventBusContainer mContainer;

};

#include "internal/Startup.h"
Peon::internal::Startup startup;

int main(int argc, char* argv[]) {
    gLogger.AddStream(new StdoutStream());
    gLogger.SetLogLevel(LogLevel::TRACE);

    GLContextSettings ctxSettings;
    ctxSettings.forwardCompatible = false;
    ctxSettings.profile = PEON_OPENGL_CORE_PROFILE;
    ctxSettings.contextVersionMajor = 4;
    ctxSettings.contextVersionMinor = 0;
   
    GLContext* context = new GLContext(ctxSettings);

    
    Shared<GLProgram> program = Shared<GLProgram>(new GLProgram()); 
    program->AddShader(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert");
    program->AddShader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.frag");
    program->LinkProgram();

    GLuint triangleVbo;

    glGenBuffers(1, &triangleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    GLWindow* second = new GLWindow(context);
    second->MakeContextCurrent();

    GLuint triangleVao2;
    glGenVertexArrays(1, &triangleVao2);
    glBindVertexArray(triangleVao2);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

    glBindVertexArray(0);

    ivec2 size2 = second->GetFramebufferSize();
    glViewport(0, 0, size2.x, size2.y);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 0.f);


    while (second->IsOpen()) {

        glClear(GL_COLOR_BUFFER_BIT);
        program->Enable();
        glBindVertexArray(triangleVao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        program->Disable();
        second->SwapBuffers();

    }

    glDeleteBuffers(1, &triangleVbo);
    glDeleteVertexArrays(1, &triangleVao2);
  
    delete context;
    delete second;
    return 0;
}


