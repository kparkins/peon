/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include <iostream>
#include <unordered_map>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common/Macros.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/GLDisplayDeviceManager.h"
#include "graphics/GLProgram.h"
#include "log/StdoutStream.h"

using std::cout;
using std::endl;
using std::cerr;
using std::unordered_map;


using namespace Peon;
using namespace glm;

static glm::mat4 perspectiveProjection;
static GLFWwindow*mainWindow;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
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

    if(!glfwInit()){
        exit(-1);
    }
    // create a newer opengl context than 2.1 (default on osx)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

/*
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    cout << glfwGetMonitorName(monitor) << endl;
    int size = 0;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &size);
    for(int i = 0; i < size; ++i, ++modes) {
        cout << modes->width << " -- " << modes->height << " -- " << modes->refreshRate << " -- " << modes->redBits
        << " -- " << modes->blueBits << " -- " << modes->greenBits << endl;
    }
    cout << "=========================================" << endl;
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    cout << mode->width << " -- " << mode->height << " -- " << mode->refreshRate << " -- " << mode->redBits
    << " -- " << mode->blueBits << " -- " << mode->greenBits << endl;

    std::cout << "done.." << std::endl; */

    int windowWidth = 640;
    int windowHeight = 480;
    mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Peon", NULL, NULL);


    if(!mainWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // set the opengl context
    glfwMakeContextCurrent(mainWindow);

    // enable GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        glfwDestroyWindow(mainWindow);
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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.f, 0.f, 0.f, 0.f);
}

static float triangleVertices[18] = {
        0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.5f, 0.f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f
};


int main(int argc, char* argv[]) {
	if (!glfwInit()) {
		LOG_FATAL("Unable to initialize GLFW.");
		exit(-1);
	}
    GLDisplayDeviceManager* deviceManager = new GLDisplayDeviceManager();
	GLDisplayDevice* displayDevice = deviceManager->GetPrimaryDisplayDevice();
	GLVideoMode videoMode = displayDevice->GetVideoMode();
	GLGammaRamp gammaRamp = displayDevice->GetGammaRamp();

	gLogger.AddStream(new StdoutStream());

    setupGlContext();

    GLShader vs(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert");

	GLProgram* program = new GLProgram();
	program->AttachStage(GLShader(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert"));
	program->AttachStage(GLShader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.frag"));
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

    // main loop
    while(!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        program->Enable();
        glBindVertexArray(triangleVao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        program->Disable();

        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
	glfwTerminate();
    delete displayDevice;
    delete deviceManager;
	delete program;

    return 0;
}



