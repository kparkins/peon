//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_CONTEXT_H
#define PEON_GL_CONTEXT_H

#include "GLContextSettings.h"
#include "GLWindowSettings.h"
#include "GLVideoMode.h"
#include "log/Logger.h"
#include "common/Uncopyable.h"
#include "common/TypeAliases.h"

namespace Peon {

    typedef void(*GLProcAddress)(void);

    class GLContext : private Uncopyable {

    public:

        explicit GLContext(Shared<GLContext> partner);
        explicit GLContext(const GLContext* const partner);
        explicit GLContext(const GLContextSettings & settings = GLContextSettings());
        
        ~GLContext();

        GLProcAddress IsExtensionSupported(const string & extensionName);

        void MakeContextCurrent();
        bool IsContextCurrent();

    private:

        GLContext(const GLVideoMode & videoMode,
            const GLContextSettings& settings,
            const GLWindowSettings & windowSettings,
            GLFWwindow* shared = nullptr);

        void ApplySettings();

        GLFWwindow* mWindow;
        GLContextSettings mSettings;
    
        friend class GLWindow;

    };
}


#endif //PEON_GL_RENDER_CONTEXT_H
