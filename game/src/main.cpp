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
#include "log/StdoutStream.h"

using std::cout;
using std::endl;
using std::cerr;
using std::unordered_map;

using namespace Peon;
using namespace glm;

static glm::mat4 perspectiveProjection;
static GLFWwindow* mainWindow;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        static bool fullscreen = false;
        fullscreen = !fullscreen;
        if (fullscreen) {
            glfwSetWindowMonitor(mainWindow, glfwGetPrimaryMonitor(), 0, 0, 640, 480, GLFW_DONT_CARE);
        } else {
            glfwSetWindowMonitor(mainWindow, nullptr, 0, 0, 640, 480, GLFW_DONT_CARE);
        }
    }
}

static void scroll_callback(GLFWwindow* window, double x, double y) {

}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {

}

static void cursor_callback(GLFWwindow* window, double x, double y) {
}


static void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void errorCallback(int error, const char* errorMessage) {
    cout << error << " -- " << errorMessage << std::endl;
}

static void setupGlContext() {
    glfwSetErrorCallback(errorCallback);
    // create a newer opengl context than 2.1 (default on osx)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int windowWidth = 640;
    int windowHeight = 480;
    mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Peon", NULL, NULL);


    if (!mainWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // set the opengl context
    glfwMakeContextCurrent(mainWindow);

    // enable GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // key callback executed for each key event when we call poll events
    glfwSetKeyCallback(mainWindow, key_callback);
    glfwSetScrollCallback(mainWindow, scroll_callback);
    glfwSetCursorPosCallback(mainWindow, cursor_callback);
    glfwSetMouseButtonCallback(mainWindow, mouse_callback);
    glfwSetWindowSizeCallback(mainWindow, window_size_callback);


    glViewport(0, 0, windowWidth, windowHeight);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
 
    glClearColor(0.f, 0.f, 0.f, 0.f);
}

static float triangleVertices[18] = {
        0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.5f, 0.f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f
};

namespace Peon {

    static bool initialize() {
        if (!glfwInit()) {
            LOG_ERROR("Failed to initialize glfw.");
            return false;
        }
        return true;
    }

    static bool initialized = initialize();
    static bool willTerminate = (atexit(glfwTerminate)) ? true : false;
}


int main(int argc, char* argv[]) {


    gLogger.AddStream(new StdoutStream());
    gLogger.SetLogLevel(LogLevel::TRACE);
  
    GLDisplayDeviceManager & deviceManager = GLDisplayDeviceManager::GetInstance();
    Shared<GLDisplayDevice> displayDevice = deviceManager.GetPrimaryDisplayDevice();

    setupGlContext();
    
    Shared<GLProgram> program = Shared<GLProgram>(new GLProgram()); 
    program->AddShader(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert");
    program->AddShader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.frag");
    program->LinkProgram();

    assert(program->IsLinked());

    Assimp::Importer importer;
    std::cin.get();
    const aiScene* scene = importer.ReadFile("res/models/mosesly4.3ds",
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("Unable to load model -- " << importer.GetErrorString());
        return 0;
    }

   // std::cout << scene->HasMaterials() << std::endl;
    std::cin.get();

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
    
    // main loop
   while (!glfwWindowShouldClose(mainWindow)) {
       
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        program->Enable();
        glBindVertexArray(triangleVao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        program->Disable();

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &triangleVbo);
    glDeleteVertexArrays(1, &triangleVao);
    
    return 0;
}



