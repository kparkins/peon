//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_WINDOW_H
#define PEON_GL_WINDOW_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "log/Logger.h"
#include "GLDisplayDevice.h"
#include "common/Uncopyable.h"
#include "common/TypeAliases.h"

using namespace glm;

namespace Peon {

#define PEON_DONT_CARE GLFW_DONT_CARE
#define PEON_OPENGL_ANY_PROFILE GLFW_OPENGL_ANY_PROFILE;
#define PEON_OPENGL_COMPAT_PROFILE GLFW_OPENGL_COMPAT_PROFILE;
#define PEON_OPENGL_CORE_PROFILE GLFW_OPENGL_CORE_PROFILE;

    typedef struct GLWindowCreateInfo {
        bool resizeable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = true;
        bool autoIconify = true;
        bool floating = false;
        bool maximized = false;
        unsigned int redBitDepth = 8;
        unsigned int greenBitDepth = 8;
        unsigned int blueBitDepth = 8;
        unsigned int alphaBitDepth = 8;
        unsigned int depthBitDepth = 24;
        unsigned int stencilBitDepth = 8;
        unsigned int samples = 0;
        int refreshRate = PEON_DONT_CARE;
        bool stereoScopic = false;
        bool srgbCapable = false;
        bool doubleBuffer = true;
        unsigned int contextVersionMajor = 1;
        unsigned int contextVersionMinor = 0;
        bool forwardCompatible = false;
        unsigned int profile = PEON_OPENGL_ANY_PROFILE;
    } GLWindowCreateInfo;

    typedef struct PixelData {
        unsigned int width;
        unsigned int height;
        unsigned char* pixels;
    }PixelData;

    class GLWindow : private Uncopyable {

    public:

        GLWindow(const GLWindowCreateInfo & info);
        ~GLWindow();

        bool ShouldClose();
        void SetShouldClose(bool close);
        void SetTitle(const string & title);
        void SetIcon(const PixelData & icon);

        ivec2 GetPosition() const;
        void SetPosition(const ivec2 & position);

        ivec2 GetSize() const;
        void SetSize(const ivec2 & dimensions);

        void SetSizeLimits(const ivec2 & min, const ivec2 & max);
        void SetAspectRatio(unsigned int numerator, unsigned int denominator);

        ivec2 GetFramebufferSize() const;

        void Iconify();
        void Restore();
        void Maximize();
        void Show();
        void Hide();
        void Focus();
        void SetFullScreen(bool on);
        void SetVerticalSync(bool on);

        Shared<GLDisplayDevice> GetDisplayDevice();
   
    protected:

        GLFWwindow* mWindow;

    };
}


#endif //PEON_GLRENDERWINDOW_H
