//
// Created by Kyle on 5/11/2016.
//

#include "GLContext.h"

Peon::GLContext::GLContext(Shared<GLContext> partner) : GLContext(partner.get()) {

}

Peon::GLContext::GLContext(const GLContext* const partner)
: mWindow(nullptr) 
{
    assert(partner != nullptr);
    glfwDefaultWindowHints();
    mSettings = partner->mSettings;
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    mWindow = glfwCreateWindow(1,1, "", nullptr, partner->mWindow);
    MakeContextCurrent();
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

Peon::GLContext::GLContext(const GLContextSettings & settings)
    : mWindow(nullptr),
    mSettings(settings) 
{
    glfwDefaultWindowHints();
    this->ApplySettings();
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    mWindow = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    MakeContextCurrent();
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

Peon::GLContext::GLContext(const GLVideoMode & videoMode, 
    const GLContextSettings & settings, 
    const GLWindowSettings & windowSettings, 
    GLFWwindow* shared) 
    : mWindow(nullptr),
      mSettings(settings)
{
    this->ApplySettings();
    windowSettings.ApplySettings();
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);
    mWindow = glfwCreateWindow(videoMode.width, videoMode.height, windowSettings.title.c_str(), nullptr, shared);
    MakeContextCurrent();
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

Peon::GLContext::~GLContext() {

}

Peon::GLProcAddress Peon::GLContext::IsExtensionSupported(const string & extensionName) {
    assert(mWindow == glfwGetCurrentContext());
    return glfwGetProcAddress(extensionName.c_str());
}

void Peon::GLContext::MakeContextCurrent() {
    glfwMakeContextCurrent(mWindow);
}

bool Peon::GLContext::IsContextCurrent() {
    return mWindow == glfwGetCurrentContext();
}

void Peon::GLContext::ApplySettings() { 
    glfwWindowHint(GLFW_STEREO, mSettings.stereoScopic);
    glfwWindowHint(GLFW_SRGB_CAPABLE, mSettings.srgbCapable);
    glfwWindowHint(GLFW_DOUBLEBUFFER, mSettings.doubleBuffer);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, mSettings.forwardCompatible);
    glfwWindowHint(GLFW_RED_BITS, mSettings.redBitDepth);
    glfwWindowHint(GLFW_GREEN_BITS, mSettings.greenBitDepth);
    glfwWindowHint(GLFW_BLUE_BITS, mSettings.blueBitDepth);
    glfwWindowHint(GLFW_ALPHA_BITS, mSettings.alphaBitDepth);
    glfwWindowHint(GLFW_DEPTH_BITS, mSettings.depthBitDepth);
    glfwWindowHint(GLFW_STENCIL_BITS, mSettings.stencilBitDepth);
    glfwWindowHint(GLFW_SAMPLES, mSettings.samples);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mSettings.contextVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mSettings.contextVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, mSettings.profile);
}