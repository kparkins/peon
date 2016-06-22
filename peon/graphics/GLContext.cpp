//
// Created by Kyle on 5/11/2016.
//

#include "GLContext.h"



Peon::GLContext::GLContext(const GLContextSettings & settings) 
    : mWindow(nullptr),
    mSettings(settings) 
{
}

/*
 * Used for access to a shared context w/o window use. (For resource loading etc.)
 */
Peon::GLContext::GLContext(Shared<GLContext> partner) {
    assert(partner.get() != nullptr);
    glfwDefaultWindowHints();
    mPartner->ApplySettings();
    mSettings = mPartner->mSettings;
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    mWindow = glfwCreateWindow(1,1, "", nullptr, mPartner->mWindow);
}

Peon::GLContext::~GLContext() {

}

Peon::GLProcAddress Peon::GLContext::IsExtensionSupported(const string & extensionName) {
    assert(mWindow == glfwGetCurrentContext());
    return glfwGetProcAddress(extensionName.c_str());
}

void Peon::GLContext::MakeCurrentContext() {
    glfwMakeContextCurrent(mWindow);
}

bool Peon::GLContext::IsCurrentContext() {
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