//
// Created by Kyle on 5/11/2016.
//

#include "GLWindow.h"

Peon::GLWindow::GLWindow(const GLVideoMode & videoMode, 
    const GLContextSettings & ctxSettings,
    const GLWindowSettings & windowSettings)
    : mIsFullscreen(false),
      mIsVsyncEnabled(false),
      mFullscreenMonitor(nullptr),
      mContext(Shared<GLContext>(new GLContext(videoMode, ctxSettings, windowSettings))) 
{ 
}

Peon::GLWindow::GLWindow(Shared<GLContext> context,
    const GLVideoMode & videoMode,
    const GLWindowSettings & windowSettings)
    : GLWindow(context.get(), videoMode, windowSettings) 
{ }

Peon::GLWindow::GLWindow(const GLContext* const context,
    const GLVideoMode & videoMode,
    const GLWindowSettings & windowSettings) 
    : mIsFullscreen(false),
    mIsVsyncEnabled(false),
    mFullscreenMonitor(nullptr),
    mContext(nullptr)
{
    assert(context != nullptr);
    mContext = Shared<GLContext>(new GLContext(videoMode, context->mSettings, windowSettings, context->mWindow));
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

void Peon::GLWindow::SetVideoMode(const GLVideoMode & videoMode) {
    if (!mIsFullscreen) {
        uvec2 pos = GetPosition();
        glfwSetWindowMonitor(mContext->mWindow, nullptr, pos.x, pos.y, 
            videoMode.width, videoMode.height, videoMode.refreshRate);
    } else {
        glfwSetWindowMonitor(mContext->mWindow, mFullscreenMonitor.mMonitor, 
            0, 0, videoMode.width, videoMode.height, videoMode.refreshRate);
    }
    mVideoMode = videoMode;
}

void Peon::GLWindow::SetFullscreen(bool fullscreen, GLMonitor monitor) {
    if (!mIsFullscreen && fullscreen) {
        mWindowedPos = GetPosition();
        mFullscreenMonitor = monitor;
        glfwSetWindowMonitor(mContext->mWindow, monitor.mMonitor, 0, 0,
            mVideoMode.width, mVideoMode.height, mVideoMode.refreshRate);
    } else if (mIsFullscreen && !fullscreen) {
        glfwSetWindowMonitor(mContext->mWindow, nullptr, mWindowedPos.x, mWindowedPos.y, 
            mVideoMode.width, mVideoMode.height, mVideoMode.refreshRate);
    }
    mIsFullscreen = fullscreen;
}

void Peon::GLWindow::SetVsync(bool on) {
    glfwSwapInterval(static_cast<int>(on));
    mIsVsyncEnabled = on;
}

Peon::GLVideoMode Peon::GLWindow::GetVideoMode() const {
    return mVideoMode;
}

ivec2 Peon::GLWindow::GetPosition() const {
    ivec2 position;
    glfwGetWindowPos(mContext->mWindow, &position.x, &position.y);
    return position;
}

ivec2 Peon::GLWindow::GetFramebufferSize() const {
    ivec2 size;
    glfwGetFramebufferSize(mContext->mWindow, &size.x, &size.y);
    return size;
}

Peon::Shared<Peon::GLContext> Peon::GLWindow::GetContext() const {
    assert(mContext.get() != nullptr);
    return mContext;
}

Peon::GLMonitor Peon::GLWindow::GetCurrentMonitor() const {
    if (mIsFullscreen) {
        return mFullscreenMonitor;
    }
    uvec2 thisPos = this->GetPosition();
    vector<GLMonitor> monitors = GLMonitor::GetMonitors();
     for (GLMonitor & monitor : monitors) {
         const uvec2 & thatPos = monitor.GetPosition();
         const GLVideoMode & thatMode = monitor.GetVideoMode();
         if (thisPos.x >= thatPos.x && thisPos.x < thatPos.x + thatMode.width &&
             thisPos.y >= thatPos.y && thisPos.y < thatPos.y + thatMode.height) {
             return monitor;
         }
     }
     return GLMonitor(nullptr);
}

void Peon::GLWindow::Maximize() {
    assert(mContext->mWindow);
    glfwMaximizeWindow(mContext->mWindow);
}

void Peon::GLWindow::Minimize() {
    assert(mContext->mWindow);
    glfwIconifyWindow(mContext->mWindow);
}

void Peon::GLWindow::Restore() {
    assert(mContext->mWindow);
    glfwRestoreWindow(mContext->mWindow);
}

void Peon::GLWindow::Show() {
    assert(mContext->mWindow);
    glfwShowWindow(mContext->mWindow);
}

void Peon::GLWindow::Hide() {
    assert(mContext->mWindow);
    glfwHideWindow(mContext->mWindow);
}

void Peon::GLWindow::Focus() {
    assert(mContext->mWindow);
    if (!this->IsMinimized() && this->IsVisible()) {
        glfwFocusWindow(mContext->mWindow);
    }
}

bool Peon::GLWindow::IsOpen() const {
    return glfwWindowShouldClose(mContext->mWindow) == 0;
}

bool Peon::GLWindow::IsVisible() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_VISIBLE) != 0;
}

bool Peon::GLWindow::IsFocused() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_FOCUSED) != 0;
}

bool Peon::GLWindow::IsMinimized() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_ICONIFIED) != 0;
}

bool Peon::GLWindow::IsMaximized() const {
    assert(mContext->mWindow);
    return glfwGetWindowAttrib(mContext->mWindow, GLFW_MAXIMIZED) != 0;
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

void Peon::GLWindow::MakeContextCurrent() {
    mContext->MakeContextCurrent();
}
