/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include <vld.h>

#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Peon.h"

#include "graphics/GLProgram.h"
#include "graphics/GLContext.h"
#include "graphics/GLWindow.h"
#include "log/StdoutStream.h"
#include "profile/Profile.h"
#include "event/EventDispatcher.h"
#include "event/Event.h"


using std::cout;
using std::endl;
using std::cerr;
using std::unordered_map;

using namespace Peon;
using namespace glm;

static float triangleVertices[18] = {
        0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.5f, 0.f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f
};

class GameStateEvent : public Event {

public:

    virtual void OnStart() = 0;
    virtual void OnPause() = 0;
    virtual void OnResume() = 0;
    virtual void OnExit() = 0;

};

class Game : public EventListener<GameStateEvent> {
public:

    void OnStart() override {
        std::cout << "on start" << std::endl;
    }

    void OnPause() override {
        std::cout << "on pause" << std::endl;
    }

    void OnResume() override {
        std::cout << "on resume" << std::endl;
    }

    void OnExit() override {
        std::cout << "on exit" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    PEON_INITIALIZE;

    Unique<Game> game = MakeUnique<Game>();
    
    PEON_EVENT(GameStateEvent, OnStart);
    PEON_EVENT(GameStateEvent, OnPause);
    PEON_EVENT(GameStateEvent, OnResume);
    PEON_EVENT(GameStateEvent, OnExit);
    delete game.release();

    gLogger.AddStream(MakeUnique<StdoutStream>());
    gLogger.SetLogLevel(LogLevel::TRACE);

    GLContextSettings ctxSettings;
    ctxSettings.forwardCompatible = false;
    ctxSettings.profile = PEON_OPENGL_CORE_PROFILE;
    ctxSettings.versionMajor = 4;
    ctxSettings.versionMinor = 0;
   
    Shared<GLWindow> mainWindow = Shared<GLWindow>(new GLWindow(GLVideoMode(640, 480), ctxSettings));
    
    Shared<GLProgram> program = Shared<GLProgram>(new GLProgram()); 
    program->AddShader(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert");
    program->AddShader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.frag");
    program->LinkProgram();

    GLuint triangleVbo;

    glGenBuffers(1, &triangleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint triangleVao2;
    glGenVertexArrays(1, &triangleVao2);
    glBindVertexArray(triangleVao2);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

    glBindVertexArray(0);

    ivec2 size = mainWindow->GetFramebufferSize();
    glViewport(0, 0, size.x, size.y);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 0.f);


    while (mainWindow->IsOpen()) {

        glClear(GL_COLOR_BUFFER_BIT);
        program->Enable();
        glBindVertexArray(triangleVao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        program->Disable();
        mainWindow->SwapBuffers();
    }

    glDeleteBuffers(1, &triangleVbo);
    glDeleteVertexArrays(1, &triangleVao2);
    
    return 0;
}


