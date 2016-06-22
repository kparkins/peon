//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_WINDOW_H
#define PEON_GL_WINDOW_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "log/Logger.h"
#include "GLMonitor.h"
#include "GLContext.h"
#include "common/Uncopyable.h"
#include "common/TypeAliases.h"
#include "common/Macros.h"


using namespace glm;

namespace Peon {

    class GLWindowSettings {
    public:

        void ApplySettings() const;

        bool resizeable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = true;
        bool autoIconify = true;
        bool floating = false;
        bool maximized = false;
        string title = "GLWindow";

    };


    class GLWindow : private Uncopyable {

    public:

        explicit GLWindow(const GLVideoMode& videoMode = GLVideoMode(),
            const GLContextSettings & ctxSettings = GLContextSettings(),
            const GLWindowSettings & windowSettings = GLWindowSettings());

        explicit GLWindow(Shared<GLContext> context,
            const GLVideoMode & videoMode = GLVideoMode(),
            const GLWindowSettings & windowSettings = GLWindowSettings());

        ~GLWindow();

        void CloseWindow();

        void SetTitle(const string & title);
        void SetIcon(unsigned int width, unsigned int height, uint8* pixels);
        void SetPosition(const ivec2 & position);
        void SetSize(const ivec2 & dimensions);
        void SetMinimized(bool minimized);
        void SetMaximized(bool maximized);
        void SetVisible(bool visible);
        void SetFocused(bool focused);
        void SetFullscreen(bool fullscreen);
        void SetVsync(bool on);

        ivec2 GetSize();
        ivec2 GetPosition();
        Shared<GLContext> GetContext();
        Shared<GLMonitor> GetCurrentMonitor();

        bool IsMinimized() const;
        bool IsMaximized() const;
        bool IsVisible() const;
        bool IsFocused() const;
        bool IsFullscreen() const;
        bool IsVsyncEnabled() const;

        void SwapBuffers();

    protected:

        bool mIsFullscreen;
        bool mIsVsyncEnabled;

        GLVideoMode mVideoMode;
        Shared<GLContext> mContext;
        
    };
}


#endif //PEON_GLRENDERWINDOW_H
