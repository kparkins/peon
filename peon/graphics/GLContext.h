//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_CONTEXT_H
#define PEON_GL_CONTEXT_H

#include <GLFW/glfw3.h>

#include "log/Logger.h"
#include "common/Uncopyable.h"
#include "common/TypeAliases.h"

namespace Peon {

    typedef void(*GLProcAddress)(void);

    class GLContextSettings {
    public:

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
        bool forwardCompatible = false;
        bool stereoScopic = false;
        bool srgbCapable = false;
        bool doubleBuffer = true;

    };

    class GLContext : private Uncopyable {

    public:

        explicit GLContext(Shared<GLContext> partner);

        ~GLContext();

        GLProcAddress IsExtensionSupported(const string & extensionName);

        void MakeCurrentContext();
        bool IsCurrentContext();

    private:

        explicit GLContext(const GLContextSettings& settings = GLContextSettings());
        void ApplySettings();

        GLFWwindow* mWindow;
        GLContextSettings mSettings;
        Shared<GLContext> mPartner;
    
        friend class GLWindow;

    };
}


#endif //PEON_GL_RENDER_CONTEXT_H
