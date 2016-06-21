//
// Created by Kyle on 5/11/2016.
//

#include "GLWindow.h"

Peon::GLWindow::GLWindow(const GLWindowCreateInfo & info)
    : mIsOpen(false),
    mIsFullscreen(false),
    mFullscreenDisplay(nullptr),
    mAspectRatio(AspectRatio{0,0}) {

    // TODO figure out context sharing issue
    glfwWindowHint(GLFW_RESIZABLE, info.resizeable);
    glfwWindowHint(GLFW_VISIBLE, info.visible);
    glfwWindowHint(GLFW_DECORATED, info.decorated);
    glfwWindowHint(GLFW_FOCUSED, info.focused);
    glfwWindowHint(GLFW_AUTO_ICONIFY, info.autoIconify);
    glfwWindowHint(GLFW_FLOATING, info.floating);
    glfwWindowHint(GLFW_MAXIMIZED, info.maximized);
    glfwWindowHint(GLFW_STEREO, info.stereoScopic);
    glfwWindowHint(GLFW_SRGB_CAPABLE, info.srgbCapable);
    glfwWindowHint(GLFW_DOUBLEBUFFER, info.doubleBuffer);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, info.forwardCompatible);
    glfwWindowHint(GLFW_REFRESH_RATE, info.refreshRate);
    glfwWindowHint(GLFW_RED_BITS, info.redBitDepth);
    glfwWindowHint(GLFW_GREEN_BITS, info.greenBitDepth);
    glfwWindowHint(GLFW_BLUE_BITS, info.blueBitDepth);
    glfwWindowHint(GLFW_ALPHA_BITS, info.alphaBitDepth);
    glfwWindowHint(GLFW_DEPTH_BITS, info.depthBitDepth);
    glfwWindowHint(GLFW_STENCIL_BITS, info.stencilBitDepth);
    glfwWindowHint(GLFW_SAMPLES, info.samples);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.contextVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.contextVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, info.profile);

    GLFWmonitor* monitor = nullptr;
    if (info.monitor) {
        monitor = info.monitor->mMonitor;
        mFullscreenDisplay = info.monitor;
        mIsFullscreen = true;
    }
    mWindow = glfwCreateWindow(info.width, info.height, info.name.c_str(), monitor, nullptr);
    assert(mWindow);
    mIsOpen = true;
    glfwSetWindowUserPointer(mWindow, static_cast<void*>(this));
    GLFWwindow* old = glfwGetCurrentContext();
    glfwMakeContextCurrent(mWindow);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        LOG_ERROR("GLEW failed to load extensions for window " << mWindow);
    }
    if (old) {
        glfwMakeContextCurrent(old);
    }
}

Peon::GLWindow::~GLWindow() {
    if (mIsOpen) {
        this->CloseWindow();
    }
}

bool Peon::GLWindow::IsOpen() {
    return mIsOpen;
}

void Peon::GLWindow::CloseWindow() {
    mIsOpen = false;
    glfwSetWindowShouldClose(mWindow, true);
}

void Peon::GLWindow::SetTitle(const string & title) {
    glfwSetWindowTitle(mWindow, title.c_str());
}

/* May want to make  this a list of icons instead of 1
 * so glfw can select the best fit.
 */
void Peon::GLWindow::SetIcon(const PixelData & icon) {
    GLFWimage image;
    image.height = icon.height;
    image.width = icon.width;
    image.pixels = icon.pixels;
    assert(image.pixels);
    glfwSetWindowIcon(mWindow, 1, &image);
}

ivec2 Peon::GLWindow::GetPosition() const {
    ivec2 pos;
    glfwGetWindowPos(mWindow, &pos.x, &pos.y);
    return pos;
}

void Peon::GLWindow::SetPosition(const ivec2 & position) {
    glfwSetWindowPos(mWindow, position.x, position.y);
}

ivec2 Peon::GLWindow::GetSize() const {
    ivec2 size;
    glfwGetWindowSize(mWindow, &size.x, &size.y);
    return size;
}

void Peon::GLWindow::SetSize(const ivec2 & dimensions) {
    glfwSetWindowSize(mWindow, dimensions.x, dimensions.y);
}

void Peon::GLWindow::SetSizeLimits(const ivec2 & min, const ivec2 & max) {
    glfwSetWindowSizeLimits(mWindow, min.x, min.y, max.x, max.y);
}

void Peon::GLWindow::SetAspectRatio(unsigned int width, unsigned int height) {
    mAspectRatio.width = width;
    mAspectRatio.height = height;
    glfwSetWindowAspectRatio(mWindow, width, height);
}

ivec2 Peon::GLWindow::GetFramebufferSize() const {
    ivec2 size;
    glfwGetFramebufferSize(mWindow, &size.x, &size.y);
    return size;
}

void Peon::GLWindow::Minimize() {
    glfwIconifyWindow(mWindow);
}

void Peon::GLWindow::Restore() {
    glfwRestoreWindow(mWindow);
}

void Peon::GLWindow::Maximize() {
    glfwMaximizeWindow(mWindow);
}

void Peon::GLWindow::Show() {
    glfwShowWindow(mWindow);
}

void Peon::GLWindow::Hide() {
    glfwHideWindow(mWindow);
}

void Peon::GLWindow::Focus() {
    glfwFocusWindow(mWindow);
}

Peon::Shared<Peon::GLDisplayDevice> Peon::GLWindow::GetFullscreenDisplay() const {
    assert(mIsFullscreen);
    return mFullscreenDisplay;
}

void Peon::GLWindow::DisableFullscreen(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    glfwSetWindowMonitor(mWindow, nullptr, x, y, width, height, 0);
}

void Peon::GLWindow::EnableFullscreen(Shared<GLDisplayDevice> display, const GLVideoMode & videoMode) {
    glfwSetWindowMonitor(mWindow, display->mMonitor, 0, 0, videoMode.GetWidth(), videoMode.GetHeight(), videoMode.GetRefreshRate());
}

void Peon::GLWindow::EnableVsync() {
    GLFWwindow* context = glfwGetCurrentContext();
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);
    glfwMakeContextCurrent(context);
}

void Peon::GLWindow::DisableVsync() {
    GLFWwindow* context = glfwGetCurrentContext();
    assert(context);
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(0);
    glfwMakeContextCurrent(context);
}

void Peon::GLWindow::SwapBuffers() {
    assert(mWindow);
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}
