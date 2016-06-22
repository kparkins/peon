//
// Created by Kyle on 5/11/2016.
//

#include "GLWindow.h"

void Peon::GLWindowSettings::ApplySettings() const {
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    glfwWindowHint(GLFW_VISIBLE, visible);
    glfwWindowHint(GLFW_DECORATED, decorated);
    glfwWindowHint(GLFW_FOCUSED, focused);
    glfwWindowHint(GLFW_AUTO_ICONIFY, autoIconify);
    glfwWindowHint(GLFW_FLOATING, floating);
    glfwWindowHint(GLFW_MAXIMIZED, maximized);
}

Peon::GLWindow::GLWindow(const GLVideoMode & videoMode, 
    const GLContextSettings & ctxSettings,
    const GLWindowSettings & windowSettings)
    : mVideoMode(videoMode),
      mContext(Shared<GLContext>(new GLContext(ctxSettings)))
{
    glfwDefaultWindowHints();
    mContext->ApplySettings();
    windowSettings.ApplySettings();
    glfwWindowHint(GLFW_REFRESH_RATE, mVideoMode.refreshRate);
    mContext->mWindow = glfwCreateWindow(videoMode.width, videoMode.height, windowSettings.title.c_str(), nullptr, nullptr);
}

/* Used to construct a window that shares opengl resources with context. (Allows multiple windows to share a context). 
 */
Peon::GLWindow::GLWindow(Shared<GLContext> context,
    const GLVideoMode & videoMode,
    const GLWindowSettings & windowSettings) 
    : mVideoMode(videoMode),
      mContext(nullptr)
{
    assert(context.get() != nullptr);
    glfwDefaultWindowHints();
    mContext = Shared<GLContext>(new GLContext(context->mSettings));
    mContext->ApplySettings();
    windowSettings.ApplySettings();
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);
    mContext->mWindow = glfwCreateWindow(videoMode.width, videoMode.height, windowSettings.title.c_str(), nullptr, context->mWindow);
}

Peon::GLWindow::~GLWindow() {
    if (!glfwWindowShouldClose(mContext->mWindow)) {
        glfwSetWindowShouldClose(mContext->mWindow, GL_TRUE);
    }
}

void Peon::GLWindow::CloseWindow() {
    assert(mContext->mWindow);
    glfwSetWindowShouldClose(mContext->mWindow, true);
}

void Peon::GLWindow::SetTitle(const string & title) {
    assert(mContext->mWindow);
    glfwSetWindowTitle(mContext->mWindow, title.c_str());
}

void Peon::GLWindow::SetIcon(unsigned int width, unsigned int height, uint8* pixels) {
    assert(pixels);
    assert(width > 0 && height > 0);
    GLFWimage image;
    image.height = height;
    image.width = width;
    image.pixels = static_cast<unsigned char*>(pixels);
    glfwSetWindowIcon(mContext->mWindow, 1, &image);
}

void Peon::GLWindow::SetPosition(const ivec2 & position) {
    assert(mContext->mWindow);
    glfwSetWindowPos(mContext->mWindow, position.x, position.y);
}

void Peon::GLWindow::SetSize(const ivec2 & dimensions) {
    assert(dimensions.x > 0 && dimensions.y > 0 && mContext->mWindow);
    glfwSetWindowSize(mContext->mWindow, dimensions.x, dimensions.y);
}

void Peon::GLWindow::SetMinimized(bool minimized) {
    assert(mContext->mWindow);
    glfwIconifyWindow(mContext->mWindow);
    mIsFullscreen = false;
}

void Peon::GLWindow::SetMaximized(bool maximized) {
    assert(mContext->mWindow);
    glfwMaximizeWindow(mContext->mWindow);
}

void Peon::GLWindow::SetVisible(bool visible) {
    if (glfwGetWindowAttrib(mContext->mWindow, GLFW_VISIBLE)) {
        glfwHideWindow(mContext->mWindow);
    } else {
        glfwShowWindow(mContext->mWindow);
    }
}

void Peon::GLWindow::SetFocused(bool focused) {

}

void Peon::GLWindow::SetFullscreen(bool fullscreen) {

}

void Peon::GLWindow::SetVsync(bool on) {

}

ivec2 Peon::GLWindow::GetSize() {
    ivec2 size;
    glfwGetWindowSize(mContext->mWindow, &size.x, &size.y);
    return size;
}

ivec2 Peon::GLWindow::GetPosition() {
    ivec2 position;
    glfwGetWindowPos(mContext->mWindow, &position.x, &position.y);
    return position;
}

Peon::Shared<Peon::GLContext> Peon::GLWindow::GetContext() {
    assert(mContext.get() != nullptr);
    return mContext;
}

Peon::Shared<Peon::GLMonitor> Peon::GLWindow::GetCurrentMonitor() {
    ivec2 pos = GetPosition();
    vector<GLMonitor> monitors = GLMonitor::GetMonitors();
   /* for (GLMonitor & monitor : monitors) {
        const ivec2 & pos = monitor.GetPosition();
        const GLVideoMode & videoMode = monitor.GetVideoMode();
        if (mPosition.x >= pos.x && mPosition.x <= pos.x + videoMode.width
            && mPosition.y >= pos.y && mPosition.y <= pos.y + videoMode.height) {

            return Shared<GLMonitor>(&monitor);
        }
    }*/
    return Shared<GLMonitor>(new GLMonitor);
}

bool Peon::GLWindow::IsMinimized() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_ICONIFIED);
}

bool Peon::GLWindow::IsMaximized() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_MAXIMIZED);
}

bool Peon::GLWindow::IsVisible() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_VISIBLE);
}

bool Peon::GLWindow::IsFocused() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_FOCUSED);
}

bool Peon::GLWindow::IsFullscreen() const {
    return mIsFullscreen;
}

bool Peon::GLWindow::IsVsyncEnabled() const {
    return mIsVsyncEnabled;
}

void Peon::GLWindow::SwapBuffers() {
    assert(mContext->mWindow);
    glfwSwapBuffers(mContext->mWindow);
    glfwPollEvents();
}
