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
        void SetVideoMode(const GLVideoMode & videoMode);
        void SetFullscreen(bool fullscreen, GLMonitor monitor = GLMonitor::GetPrimaryMonitor());
        void SetVsync(bool on);

        ivec2 GetPosition() const;
        ivec2 GetFramebufferSize() const;
        GLVideoMode GetVideoMode() const;
        Shared<GLContext> GetContext() const;
        GLMonitor GetCurrentMonitor() const;  // TODO make sure to handle disconnected monitors once event system implemented.
 
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

        ivec2 mWindowedPos;
        GLVideoMode mVideoMode;
        GLMonitor mFullscreenMonitor;

        Shared<GLContext> mContext;

    };
}


#endif //PEON_GLRENDERWINDOW_H
