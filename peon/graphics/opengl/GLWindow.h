//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_WINDOW_H
#define PEON_GL_WINDOW_H

#include <glm/glm.hpp>

#include "GLContext.h"
#include "GLContextOpts.h"
#include "GLMonitor.h"
#include "GLViewport.h"
#include "GLWindowOpts.h"
#include "GraphicsExtensions.h"
#include "common/Macros.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"
#include "event/Bus.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"
#include "log/Logger.h"

using namespace glm;

namespace Peon {

enum class CursorMode {
  NORMAL = GLFW_CURSOR_NORMAL,
  HIDDEN = GLFW_CURSOR_HIDDEN,
  DISABLED = GLFW_CURSOR_DISABLED,
};

class GLWindow : private Uncopyable {
 public:
  explicit GLWindow(const GLContextOpts& ctxSettings = GLContextOpts(),
                    const GLVideoMode& videoMode = GLVideoMode(),
                    const GLWindowOpts& windowSettings = GLWindowOpts());

  explicit GLWindow(Shared<GLContext> context,
                    const GLVideoMode& videoMode = GLVideoMode(),
                    const GLWindowOpts& windowSettings = GLWindowOpts());

  explicit GLWindow(const GLContext* const context,
                    const GLVideoMode& videoMode = GLVideoMode(),
                    const GLWindowOpts& windowSettings = GLWindowOpts());

  ~GLWindow();

  void CloseWindow();

  void SetTitle(const string& title);
  void SetIcon(unsigned int width, unsigned int height, uint8* pixels);
  void SetPosition(const ivec2& position);
  void SetVideoMode(const GLVideoMode& videoMode);
  void SetFullscreen(bool fullscreen,
                     const GLMonitor& monitor = GLMonitor::GetPrimaryMonitor());
  void SetVsync(bool on);
  void SetCursorMode(CursorMode mode);
  void SetDefaultBus(Shared<Bus> bus);

  ivec2 GetPosition() const;
  Shared<Bus> GetDefaultBus() const;
  ivec2 GetFramebufferSize() const;
  GLViewport GetViewport() const;
  GLVideoMode GetVideoMode() const;
  Shared<GLContext> GetContext() const;
  GLMonitor GetCurrentMonitor() const;
  CursorMode GetCursorMode() const;

  void Maximize();
  void Minimize();
  void Restore();
  void Show();
  void Hide();
  void Focus();

  bool IsOpen() const;
  bool IsVisible() const;
  bool IsFocused() const;
  bool IsMinimized() const;
  bool IsMaximized() const;
  bool IsFullscreen() const;
  bool IsVsyncEnabled() const;

  void SwapBuffers();
  void MakeContextCurrent();

 protected:
  void SetCallbacks();

  // Window related callbacks
  static void OnWindowResize(GLFWwindow* window, int width, int height);
  static void OnWindowMove(GLFWwindow* window, int xpos, int ypos);
  static void OnWindowDamage(GLFWwindow* window);
  static void OnWindowClose(GLFWwindow* window);
  static void OnWindowFocus(GLFWwindow* window, int focused);
  static void OnWindowMinimize(GLFWwindow* window, int minimized);
  static void OnFramebufferSizeChange(GLFWwindow* window,
                                      int width,
                                      int height);

  // Monitor callback for monitor connect/disconnect events;
  static void OnMonitorStateChange(GLFWmonitor* monitor, int stateChange);

  // Input callbacks
  static void OnMouseButtonEvent(GLFWwindow* window,
                                 int button,
                                 int action,
                                 int mods);
  static void OnCursorPositionChange(GLFWwindow* window,
                                     double xpos,
                                     double ypos);
  static void OnCursorEnteredWindow(GLFWwindow* window, int entered);
  static void OnMouseScrollEvent(GLFWwindow* window,
                                 double xoffset,
                                 double yoffset);
  static void OnKeyboardKeyEvent(GLFWwindow* window,
                                 int key,
                                 int scancode,
                                 int action,
                                 int mods);
  static void OnCharacterTypedEvent(GLFWwindow* window,
                                    unsigned int codepoint,
                                    int mods);
  static void OnFileDropEvent(GLFWwindow* window,
                              int numFiles,
                              const char** filePaths);

  bool mIsFullscreen;
  bool mIsVsyncEnabled;

  ivec2 mWindowedPos;
  GLVideoMode mVideoMode;
  GLMonitor mFullscreenMonitor;

  Shared<Bus> mBus;
  Shared<GLContext> mContext;
};
}  // namespace Peon

#endif  // PEON_GLRENDERWINDOW_H
