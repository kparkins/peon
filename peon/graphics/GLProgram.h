/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef PEON_GL_PROGRAM_H
#define PEON_GL_PROGRAM_H

#include <string>
#include <memory>
#include <cassert>
#include <vector>

#include "common/Macros.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "log/Logger.h"
#include "GLShader.h"

using std::vector;
using std::string;
using std::shared_ptr;
using std::unordered_map;

using namespace glm;

namespace Peon {
    class GLProgram {
    public:

        GLProgram();
        ~GLProgram();

        GLProgram & AttachShader(GLuint id);
        GLProgram & AttachShader(const GLShader & shader);

        bool IsLinked();
        void LinkProgram();
        void Enable();
        void Disable();

        void SetUniform(const string & uniform, const mat4 & matrix);
        void SetUniform(const string & uniform, const vec4 & vector);
        void SetUniform(const string & uniform, const vec3 & vector);
        void SetUniform(const string & uniform, float value);
        void SetUniform(const string & uniform, int value);

    private:

        bool mLinked;
        bool mEnabled;
        GLuint mHandle;
        vector<GLuint> mShaders;
        unordered_map<string, GLint> mUniforms;

    };
}
#endif // PEON_SHADER_PROGRAM_GL_H
