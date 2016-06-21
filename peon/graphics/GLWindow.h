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
#include "GLDisplayDeviceManager.h"
#include "GLDisplayDevice.h"
#include "common/Uncopyable.h"
#include "common/TypeAliases.h"

#define PEON_DONT_CARE GLFW_DONT_CARE
#define PEON_OPENGL_ANY_PROFILE GLFW_OPENGL_ANY_PROFILE;
#define PEON_OPENGL_COMPAT_PROFILE GLFW_OPENGL_COMPAT_PROFILE;
#define PEON_OPENGL_CORE_PROFILE GLFW_OPENGL_CORE_PROFILE;

using namespace glm;

namespace Peon {

   typedef struct GLWindowCreateInfo {
        bool resizeable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = true;
        bool autoIconify = true;
        bool floating = false;
        bool maximized = false;
        bool stereoScopic = false;
        bool srgbCapable = false;
        bool doubleBuffer = true;
        bool forwardCompatible = false;
        int refreshRate = PEON_DONT_CARE;
        unsigned int redBitDepth = 8;
        unsigned int greenBitDepth = 8;
        unsigned int blueBitDepth = 8;
        unsigned int alphaBitDepth = 8;
        unsigned int depthBitDepth = 24;
        unsigned int stencilBitDepth = 8;
        unsigned int samples = 0;   
        unsigned int contextVersionMajor = 1;
        unsigned int contextVersionMinor = 0;
        unsigned int profile = PEON_OPENGL_ANY_PROFILE;
        unsigned int width = 640;
        unsigned int height = 480;
        Shared<GLDisplayDevice> monitor = nullptr;
        string name;
    } GLWindowCreateInfo;

    typedef struct PixelData {
        unsigned int width;
        unsigned int height;
        unsigned char* pixels;
    }PixelData;

    typedef struct AspectRatio {
        unsigned int width;
        unsigned int height;
    }AspectRatio;

    class GLWindow : private Uncopyable {

    public:

        explicit GLWindow(const GLWindowCreateInfo & info);
        ~GLWindow();

        bool IsOpen();
        void CloseWindow();
        void SetTitle(const string & title);
        void SetIcon(const PixelData & icon);

        ivec2 GetPosition() const;
        void SetPosition(const ivec2 & position);

        ivec2 GetSize() const;
        void SetSize(const ivec2 & dimensions);

        void SetSizeLimits(const ivec2 & min, const ivec2 & max);
        void SetAspectRatio(unsigned int width, unsigned int height);

        ivec2 GetFramebufferSize() const;

        /* These functions are idempotent so we dont need state getters */
        void Minimize();
        void Restore();
        void Maximize();
        void Show();
        void Hide();
        void Focus();

        /* Not idempotent */
        bool IsFullscreen() const;
        void DisableFullscreen(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
        void EnableFullscreen(Shared<GLDisplayDevice> display, const GLVideoMode & videoMode);

        void EnableVsync();
        void DisableVsync();

        void SwapBuffers();

        Shared<GLDisplayDevice> GetFullscreenDisplay() const;
        GLFWwindow* mWindow;
        
    protected:

        bool mIsOpen;
        bool mIsFullscreen;

        GLVideoMode mVideoMode;
        AspectRatio mAspectRatio;
        Shared<GLDisplayDevice> mFullscreenDisplay;
    };
}


#endif //PEON_GLRENDERWINDOW_H
