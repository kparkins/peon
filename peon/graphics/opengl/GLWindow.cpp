//
// Created by Kyle on 5/11/2016.
//

#include "GLWindow.h"

Peon::GLWindow::GLWindow(const GLContextOpts& ctxSettings,
                         const GLVideoMode& videoMode,
                         const GLWindowOpts& windowOpts)
    : mIsFullscreen(false),
      mIsVsyncEnabled(false),
      mFullscreenMonitor(nullptr),
      mBus(MakeShared<Bus>()),
      mContext(Shared<GLContext>(
          new GLContext(ctxSettings, videoMode, windowOpts, nullptr))) {
  this->SetCallbacks();
  glfwSetWindowUserPointer(mContext->mWindow, reinterpret_cast<void*>(this));
}

Peon::GLWindow::GLWindow(Shared<GLContext> context,
                         const GLVideoMode& videoMode,
                         const GLWindowOpts& windowOpts)
    : GLWindow(context.get(), videoMode, windowOpts) {}

Peon::GLWindow::GLWindow(const GLContext* const context,
                         const GLVideoMode& videoMode,
                         const GLWindowOpts& windowOpts)
    : mIsFullscreen(false),
      mIsVsyncEnabled(false),
      mFullscreenMonitor(nullptr),
      mBus(MakeShared<Bus>()),
      mContext(nullptr) {
  assert(context != nullptr);
  mContext = Shared<GLContext>(
      new GLContext(context->mOpts, videoMode, windowOpts, context->mWindow));
  this->SetCallbacks();
  glfwSetWindowUserPointer(mContext->mWindow, reinterpret_cast<void*>(this));
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

void Peon::GLWindow::SetTitle(const string& title) {
  assert(mContext->mWindow);
  glfwSetWindowTitle(mContext->mWindow, title.c_str());
}

void Peon::GLWindow::SetIcon(unsigned int width, unsigned int height,
                             uint8* pixels) {
  assert(pixels);
  assert(width > 0 && height > 0);
  GLFWimage image;
  image.height = height;
  image.width = width;
  image.pixels = static_cast<unsigned char*>(pixels);
  glfwSetWindowIcon(mContext->mWindow, 1, &image);
}

void Peon::GLWindow::SetPosition(const ivec2& position) {
  assert(mContext->mWindow);
  glfwSetWindowPos(mContext->mWindow, position.x, position.y);
}

void Peon::GLWindow::SetVideoMode(const GLVideoMode& videoMode) {
  if (!mIsFullscreen) {
    uvec2 pos = GetPosition();
    glfwSetWindowMonitor(mContext->mWindow, nullptr, pos.x, pos.y,
                         videoMode.width, videoMode.height,
                         videoMode.refreshRate);
  } else {
    glfwSetWindowMonitor(mContext->mWindow, mFullscreenMonitor.mMonitor, 0, 0,
                         videoMode.width, videoMode.height,
                         videoMode.refreshRate);
  }
  mVideoMode = videoMode;
}

void Peon::GLWindow::SetFullscreen(bool fullscreen, const GLMonitor& monitor) {
  if (!mIsFullscreen && fullscreen) {
    mWindowedPos = GetPosition();
    mFullscreenMonitor = monitor;
    glfwSetWindowMonitor(mContext->mWindow, monitor.mMonitor, 0, 0,
                         mVideoMode.width, mVideoMode.height,
                         mVideoMode.refreshRate);
  } else if (mIsFullscreen && !fullscreen) {
    glfwSetWindowMonitor(mContext->mWindow, nullptr, mWindowedPos.x,
                         mWindowedPos.y, mVideoMode.width, mVideoMode.height,
                         mVideoMode.refreshRate);
  }
  mIsFullscreen = fullscreen;
}

void Peon::GLWindow::SetVsync(bool on) {
  glfwSwapInterval(static_cast<int>(on));
  mIsVsyncEnabled = on;
}

void Peon::GLWindow::SetDefaultBus(Shared<Bus> bus) {
  assert(bus);
  this->mBus = bus;
}

void Peon::GLWindow::SetCursorMode(Peon::CursorMode mode) {
  glfwSetInputMode(mContext->mWindow, GLFW_CURSOR, static_cast<int>(mode));
}

Peon::GLVideoMode Peon::GLWindow::GetVideoMode() const { return mVideoMode; }

ivec2 Peon::GLWindow::GetPosition() const {
  ivec2 position;
  glfwGetWindowPos(mContext->mWindow, &position.x, &position.y);
  return position;
}

Peon::Shared<Peon::Bus> Peon::GLWindow::GetDefaultBus() const {
  return this->mBus;
}

ivec2 Peon::GLWindow::GetFramebufferSize() const {
  ivec2 size;
  glfwGetFramebufferSize(mContext->mWindow, &size.x, &size.y);
  return size;
}

Peon::GLViewport Peon::GLWindow::GetViewport() const {
  ivec2 size = this->GetFramebufferSize();
  return GLViewport(0, 0, size.x, size.y);
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
  for (GLMonitor& monitor : monitors) {
    const uvec2& thatPos = monitor.GetPosition();
    const GLVideoMode& thatMode = monitor.GetVideoMode();
    if (thisPos.x >= thatPos.x && thisPos.x < thatPos.x + thatMode.width &&
        thisPos.y >= thatPos.y && thisPos.y < thatPos.y + thatMode.height) {
      return monitor;
    }
  }
  return GLMonitor(nullptr);
}

Peon::CursorMode Peon::GLWindow::GetCursorMode() const {
  int mode = glfwGetInputMode(mContext->mWindow, GLFW_CURSOR);
  return static_cast<CursorMode>(mode);
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

bool Peon::GLWindow::IsFullscreen() const { return mIsFullscreen; }

bool Peon::GLWindow::IsVsyncEnabled() const { return mIsVsyncEnabled; }

void Peon::GLWindow::SwapBuffers() {
  assert(mContext->mWindow);
  glfwSwapBuffers(mContext->mWindow);
  glfwPollEvents();
}

void Peon::GLWindow::MakeContextCurrent() { mContext->MakeContextCurrent(); }

void Peon::GLWindow::SetCallbacks() {
  // Window callbacks
  glfwSetWindowSizeCallback(mContext->mWindow, &GLWindow::OnWindowResize);
  glfwSetWindowPosCallback(mContext->mWindow, &GLWindow::OnWindowMove);
  glfwSetWindowRefreshCallback(mContext->mWindow, &GLWindow::OnWindowDamage);
  glfwSetWindowCloseCallback(mContext->mWindow, &GLWindow::OnWindowClose);
  glfwSetWindowIconifyCallback(mContext->mWindow, &GLWindow::OnWindowMinimize);
  glfwSetFramebufferSizeCallback(mContext->mWindow,
                                 &GLWindow::OnFramebufferSizeChange);

  // Monitor connect/disconnect events
  glfwSetMonitorCallback(&GLWindow::OnMonitorStateChange);

  // input callbacks
  glfwSetMouseButtonCallback(mContext->mWindow, &GLWindow::OnMouseButtonEvent);
  glfwSetCursorPosCallback(mContext->mWindow,
                           &GLWindow::OnCursorPositionChange);
  glfwSetCursorEnterCallback(mContext->mWindow,
                             &GLWindow::OnCursorEnteredWindow);
  glfwSetScrollCallback(mContext->mWindow, &GLWindow::OnMouseScrollEvent);
  glfwSetKeyCallback(mContext->mWindow, &GLWindow::OnKeyboardKeyEvent);
  glfwSetCharModsCallback(mContext->mWindow, &GLWindow::OnCharacterTypedEvent);
  glfwSetDropCallback(mContext->mWindow, &GLWindow::OnFileDropEvent);
}

void Peon::GLWindow::OnWindowResize(GLFWwindow* window, int width, int height) {

}

void Peon::GLWindow::OnWindowMove(GLFWwindow* window, int xpos, int ypos) {}

void Peon::GLWindow::OnWindowDamage(GLFWwindow* window) {}

void Peon::GLWindow::OnWindowClose(GLFWwindow* window) {}

void Peon::GLWindow::OnWindowFocus(GLFWwindow* window, int focused) {}

void Peon::GLWindow::OnWindowMinimize(GLFWwindow* window, int minimized) {}

void Peon::GLWindow::OnFramebufferSizeChange(GLFWwindow* window, int width,
                                             int height) {
  GLFWwindow* current = glfwGetCurrentContext();
  GLWindow* currentWindow =
      reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(current));
  GLWindow* resizedWindow =
      reinterpret_cast<GLWindow*>(glfwGetWindowUserPointer(window));
  if (window == current) {
    glViewport(0, 0, width, height);
    return;
  }
  resizedWindow->MakeContextCurrent();
  glViewport(0, 0, width, height);
  currentWindow->MakeContextCurrent();
}

void Peon::GLWindow::OnMonitorStateChange(GLFWmonitor* monitor,
                                          int stateChange) {}

void Peon::GLWindow::OnMouseButtonEvent(GLFWwindow* window, int button,
                                        int action, int mods) {}

void Peon::GLWindow::OnCursorPositionChange(GLFWwindow* window, double xpos,
                                            double ypos) {
  GLWindow* w = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
  Shared<Bus> bus = w->GetDefaultBus();
  bus->Emit<MouseMove>(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Peon::GLWindow::OnCursorEnteredWindow(GLFWwindow* window, int entered) {}

void Peon::GLWindow::OnMouseScrollEvent(GLFWwindow* window, double xoffset,
                                        double yoffset) {}

void Peon::GLWindow::OnKeyboardKeyEvent(GLFWwindow* window, int key,
                                        int scancode, int action, int mods) {
  GLWindow* w = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
  Shared<Bus> bus = w->GetDefaultBus();
  bus->Emit<KeyEvent>(key, action);
}

void Peon::GLWindow::OnCharacterTypedEvent(GLFWwindow* window,
                                           unsigned int codepoint, int mods) {}

void Peon::GLWindow::OnFileDropEvent(GLFWwindow* window, int numFiles,
                                     const char** filePaths) {}
