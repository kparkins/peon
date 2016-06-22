/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */
/*
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
        } else if (key == GLFW_KEY_A) {

        } else if (key == GLFW_KEY_S) {
            
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

    Shared<GLContext> context = Shared<GLContext>(new GLContext());
    
    mainWindow = new GLWindow(GLVideoMode(640, 480));



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



*/
//========================================================================
// Context sharing test program
// Copyright (c) Camilla Berglund <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//
// This program is used to test sharing of objects between contexts
//
//========================================================================

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH  400
#define HEIGHT 400
#define OFFSET 50

static GLFWwindow* windows[2];

static void error_callback(int error, const char* description) {
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static GLFWwindow* open_window(const char* title, GLFWwindow* share, int posX, int posY) {
    GLFWwindow* window;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, share);
    if (!window)
        return NULL;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowPos(window, posX, posY);
    glfwShowWindow(window);

    glfwSetKeyCallback(window, key_callback);

    return window;
}

static GLuint create_texture(void) {
    int x, y;
    char pixels[256 * 256];
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    for (y = 0; y < 256; y++) {
        for (x = 0; x < 256; x++)
            pixels[y * 256 + x] = rand() % 256;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

static void draw_quad(GLuint texture) {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBegin(GL_QUADS);

    glTexCoord2f(0.f, 0.f);
    glVertex2f(0.f, 0.f);

    glTexCoord2f(1.f, 0.f);
    glVertex2f(1.f, 0.f);

    glTexCoord2f(1.f, 1.f);
    glVertex2f(1.f, 1.f);

    glTexCoord2f(0.f, 1.f);
    glVertex2f(0.f, 1.f);

    glEnd();
}

int main(int argc, char** argv) {
    int x, y, width;
    GLuint texture;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
    }

    windows[0] = open_window("First", NULL, OFFSET, OFFSET);
    if (!windows[0]) {
        glfwTerminate();
    }

    // This is the one and only time we create a texture
    // It is created inside the first context, created above
    // It will then be shared with the second context, created below
    texture = create_texture();

    glfwGetWindowPos(windows[0], &x, &y);
    glfwGetWindowSize(windows[0], &width, NULL);

    // Put the second window to the right of the first one
    windows[1] = open_window("Second", windows[0], x + width + OFFSET, y);
    if (!windows[1]) {
        glfwTerminate();
    }

    // Set drawing color for both contexts
    glfwMakeContextCurrent(windows[0]);
    glColor3f(0.6f, 0.f, 0.6f);
    glfwMakeContextCurrent(windows[1]);
    glColor3f(0.6f, 0.6f, 0.f);
    glfwHideWindow(windows[1]);

    glfwMakeContextCurrent(windows[0]);

    while (!glfwWindowShouldClose(windows[0]) &&
        !glfwWindowShouldClose(windows[1])) {
        glfwMakeContextCurrent(windows[0]);
        draw_quad(texture);

        glfwSwapBuffers(windows[0]);
        
        glfwWaitEvents();
    }

    glfwTerminate();
}

