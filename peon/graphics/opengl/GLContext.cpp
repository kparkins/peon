//
// Created by Kyle on 5/11/2016.
//

#include "GLContext.h"

Peon::GLContext::GLContext(const GLContext* const partner) : mWindow(nullptr) {
  assert(partner != nullptr);
  glfwDefaultWindowHints();
  mOpts = partner->mOpts;
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  mWindow = glfwCreateWindow(1, 1, "", nullptr, partner->mWindow);
  MakeContextCurrent();
  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

Peon::GLContext::GLContext(const GLContextOpts& settings)
    : mWindow(nullptr), mOpts(settings) {
  glfwDefaultWindowHints();
  Apply();
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  mWindow = glfwCreateWindow(1, 1, "", nullptr, nullptr);
  MakeContextCurrent();
  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

Peon::GLContext::GLContext(const GLContextOpts& settings,
                           const GLVideoMode& videoMode,
                           const GLWindowOpts& windowSettings,
                           GLFWwindow* shared)
    : mWindow(nullptr), mOpts(settings) {
  Apply();
  windowSettings.Apply();
  glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);
  glfwSetErrorCallback(GLContext::error);
  mWindow = glfwCreateWindow(videoMode.width, videoMode.height,
                             windowSettings.title.c_str(), nullptr, shared);
  MakeContextCurrent();
  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

Peon::GLContext::~GLContext() { glfwDestroyWindow(mWindow); }

void Peon::GLContext::error(int num, const char* msg) {
  LOG_ERROR("GLFW error " << num << " " << msg);
}

Peon::GLProcAddress Peon::GLContext::IsExtensionSupported(
    const string& extensionName) {
  assert(mWindow == glfwGetCurrentContext());
  return glfwGetProcAddress(extensionName.c_str());
}

void Peon::GLContext::MakeContextCurrent() { glfwMakeContextCurrent(mWindow); }

bool Peon::GLContext::IsContextCurrent() {
  return mWindow == glfwGetCurrentContext();
}

void Peon::GLContext::Apply() {
  glfwWindowHint(GLFW_STEREO, mOpts.stereoScopic);
  glfwWindowHint(GLFW_SRGB_CAPABLE, mOpts.srgbCapable);
  glfwWindowHint(GLFW_DOUBLEBUFFER, mOpts.doubleBuffer);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, mOpts.forwardCompatible);
  glfwWindowHint(GLFW_RED_BITS, mOpts.redBitDepth);
  glfwWindowHint(GLFW_GREEN_BITS, mOpts.greenBitDepth);
  glfwWindowHint(GLFW_BLUE_BITS, mOpts.blueBitDepth);
  glfwWindowHint(GLFW_ALPHA_BITS, mOpts.alphaBitDepth);
  glfwWindowHint(GLFW_DEPTH_BITS, mOpts.depthBitDepth);
  glfwWindowHint(GLFW_STENCIL_BITS, mOpts.stencilBitDepth);
  glfwWindowHint(GLFW_SAMPLES, mOpts.samples);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mOpts.versionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mOpts.versionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, mOpts.profile);
}