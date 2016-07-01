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
#include "profile/BlockTimer.h"
#include "event/EventDispatcher.h"
#include "event/Event.h"
#include "profile/RecordKeeper.h"

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

class Game {

public:

    Game() {

    }

    ~Game() {
        delete mWindow;
        delete mProgram;
        glDeleteBuffers(1, &mVbo);
        glDeleteVertexArrays(1, &mVao);

    }

    void Initialize() {
        PEON_INITIALIZE;

        gLogger.AddStream(MakeUnique<StdoutStream>());
        gLogger.SetLogLevel(LogLevel::TRACE);

        GLContextSettings ctxSettings;
        ctxSettings.forwardCompatible = false;
        ctxSettings.profile = PEON_OPENGL_CORE_PROFILE;
        ctxSettings.versionMajor = 4;
        ctxSettings.versionMinor = 0;

        mWindow = new GLWindow(GLVideoMode(640, 480), ctxSettings);

        mProgram = new GLProgram();
        mProgram->AddShader(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert");
        mProgram->AddShader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.frag");
        mProgram->LinkProgram();

        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

        glBindVertexArray(0);

        ivec2 size = mWindow->GetFramebufferSize();
        glViewport(0, 0, size.x, size.y);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glClearColor(0.f, 0.f, 0.f, 0.f);
    }

    void Run() {

        while (mWindow->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
            mProgram->Enable();
            glBindVertexArray(mVao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            mProgram->Disable();
            mWindow->SwapBuffers();
        }

    }

private:

    GLuint mVao;
    GLuint mVbo;
    
    GLProgram* mProgram;
    GLWindow* mWindow;

};

int main(int argc, char* argv[]) {
   
    Game g;
    g.Initialize();
    g.Run();

    return 0;
}


