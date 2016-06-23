//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_WINDOW_H
#define PEON_GL_WINDOW_H

#include <glm/glm.hpp>

#include "Peon.h"
#include "log/Logger.h"
#include "GLContextSettings.h"
#include "GLWindowSettings.h"
#include "GLMonitor.h"
#include "GLContext.h"

#include "common/Uncopyable.h"
#include "common/TypeAliases.h"
#include "common/Macros.h"

using namespace glm;

namespace Peon {

    class GLWindow : private Uncopyable {

    public:

        explicit GLWindow(const GLVideoMode& videoMode = GLVideoMode(),
            const GLContextSettings & ctxSettings = GLContextSettings(),
            const GLWindowSettings & windowSettings = GLWindowSettings());

        explicit GLWindow(Shared<GLContext> context,
            const GLVideoMode & videoMode = GLVideoMode(),
            const GLWindowSettings & windowSettings = GLWindowSettings());

        explicit GLWindow(const GLContext* const context,
            const GLVideoMode & videoMode = GLVideoMode(),
            const GLWindowSettings & windowSettings = GLWindowSettings());

        ~GLWindow();

        void CloseWindow();

        void SetTitle(const string & title);
        void SetIcon(unsigned int width, unsigned int height, uint8* pixels);
        void SetPosition(const ivec2 & position);
        void SetSize(const ivec2 & dimensions);
        void SetFullscreen(bool fullscreen); // TODO
        void SetVsync(bool on);

        ivec2 GetSize();
        ivec2 GetPosition();
        ivec2 GetFramebufferSize();
        Shared<GLContext> GetContext();
        GLMonitor GetCurrentMonitor(); // TODO
        
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

        bool mIsFullscreen;
        bool mIsVsyncEnabled;

        GLVideoMode mVideoMode;
        Shared<GLContext> mContext;
        
    };
}


#endif //PEON_GLRENDERWINDOW_H
