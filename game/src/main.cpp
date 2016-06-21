/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include <vld.h>

#include <iostream>
#include <unordered_map>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "graphics/GLDisplayDeviceManager.h"
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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        LOG_INFO("setting should close");
        mainWindow->CloseWindow();
    } else if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W) {
            Shared<GLDisplayDevice> monitor = GLDisplayDeviceManager::GetPrimaryDisplayDevice();
            GLVideoMode videoMode = monitor->GetVideoMode();
            mainWindow->EnableFullscreen(monitor, videoMode);
            glViewport(0, 0, videoMode.GetWidth(), videoMode.GetHeight());
        } else if (key == GLFW_KEY_A) {

        } else if (key == GLFW_KEY_S) {
            Shared<GLDisplayDevice> monitor = GLDisplayDeviceManager::GetPrimaryDisplayDevice();
            mainWindow->DisableFullscreen(25, 25, 640, 480);

            glViewport(0, 0, 640, 480);
        } else if (key == GLFW_KEY_D) {

        }
    }
}

static void scroll_callback(GLFWwindow* window, double x, double y) {

}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {

}

static void cursor_callback(GLFWwindow* window, double x, double y) {
    std::cout << x << " " << y << std::endl;
}


static void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void errorCallback(int error, const char* errorMessage) {
    cout << error << " -- " << errorMessage << std::endl;
}

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


int main(int argc, char* argv[]) {
    gLogger.AddStream(new StdoutStream());
    gLogger.SetLogLevel(LogLevel::TRACE);

    GLWindowCreateInfo info;
    info.contextVersionMajor = 3;
    info.contextVersionMinor = 3;
    info.profile = PEON_OPENGL_CORE_PROFILE;
    info.width = 640;
    info.height = 480;
    info.samples = 4;
    mainWindow = new GLWindow(info);
    
    Shared<GLProgram> program = Shared<GLProgram>(new GLProgram()); 
    program->AddShader(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert");
    program->AddShader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.frag");
    program->LinkProgram();

    assert(program->IsLinked());

    GLuint triangleVao;
    GLuint triangleVbo;
    
    glGenVertexArrays(1, &triangleVao);
    glGenBuffers(1, &triangleVbo);

    glBindVertexArray(triangleVao);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(triangleVao, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(0));

    glEnableVertexArrayAttrib(triangleVao, 1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

    glBindVertexArray(0);

    glViewport(0, 0, info.width, info.height);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 0.f);
    /*
        GLContext* context = renderer->GetContext();
        VertexBuffer* vertexBuffer = context->CreateVertexBuffer(size, data, vertexFormat, bufferUsage);
        renderer->Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 
        renderer->PushState(); 
        renderer->Enable(GL_DEPTH_TEST);
        renderer->Disable(GL_CULL_FACE);
        renderer->Render(vertexBuffer);
        renderer->PopState():
        
    */
    glfwSetKeyCallback(mainWindow->mWindow, key_callback);
    // main loop
   while (mainWindow->IsOpen()) {

    
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        program->Enable();
        glBindVertexArray(triangleVao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        program->Disable();
        mainWindow->SwapBuffers();
    }

    glDeleteBuffers(1, &triangleVbo);
    glDeleteVertexArrays(1, &triangleVao);
    delete mainWindow;
    return 0;
}



