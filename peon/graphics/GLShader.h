//
// Created by Kyle on 2/15/2016.
//

#ifndef PEON_GL_SHADER_H
#define PEON_GL_SHADER_H

#include <string>
#include <memory>
#include <unordered_map>

#define GLEW_STATIC
#include <GL/glew.h>

#include "log/Logger.h"

using std::string;

namespace Peon {
    class GLShader {
    public:

        GLShader();
        GLShader(GLuint type, const string & source);
        ~GLShader();

        void Load(GLuint type, const string & file);
        void Compile(GLuint type, const string & shader);

        GLuint GetHandle() const;
        GLuint GetShaderType() const;

    protected:

        GLuint mHandle;
        GLuint mShaderType;

    };

}

#endif //PEON_SHADER_GL_H
