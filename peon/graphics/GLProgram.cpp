/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "GLProgram.h"

Peon::GLProgram::GLProgram() : mLinked(false), mEnabled(false) {
}

Peon::GLProgram::~GLProgram() {
    glDeleteProgram(mHandle);
}

Peon::GLProgram & Peon::GLProgram::AttachStage(GLuint id) {
    assert(!mLinked);
    mShaders.push_back(id);
    return *this;
}

Peon::GLProgram & Peon::GLProgram::AttachStage(const GLShader & shader) {
    assert(!mLinked);
    mShaders.push_back(shader.GetHandle());
    return *this;
}

void Peon::GLProgram::LinkProgram() {
    assert(!mLinked);
    mHandle = glCreateProgram();
    for(GLuint shader : mShaders) {
        glAttachShader(mHandle, shader);
    }
    GLint linkSuccess = GL_TRUE;
    glLinkProgram(mHandle);
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &linkSuccess);
    if(!linkSuccess) {
        GLchar errorLog[512];
        glGetShaderInfoLog(mHandle, 512, 0, errorLog);
        LOG_ERROR(errorLog);
        return;
    }
    for(GLuint shader : mShaders) {
        glDetachShader(mHandle, shader);
        glDeleteShader(shader);
    }
    mShaders.clear();
    mLinked = true;
}

void Peon::GLProgram::Enable() {
    assert(mLinked);
    glUseProgram(mHandle);
    mEnabled = true;
}

void Peon::GLProgram::Disable() {
    assert(mLinked);
    assert(mEnabled);
    glUseProgram(0);
    mEnabled = false;
}

bool Peon::GLProgram::IsLinked() {
    return mLinked;
}

void Peon::GLProgram::SetUniform(const string & uniform, const mat4 & matrix) {
    if(mUniforms.find(uniform) != mUniforms.end()) {
        glUniformMatrix4fv(mUniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
        return;
    }
    GLint location = glGetUniformLocation(mHandle, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mHandle);
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, const vec3 & vector) {
    if(mUniforms.find(uniform) != mUniforms.end()) {
        glUniform3fv(mUniforms[uniform], 1, glm::value_ptr(vector));
        return;
    }
    GLint location = glGetUniformLocation(mHandle, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mHandle);
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(vector));
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, const vec4 & vector) {
    if(mUniforms.find(uniform) != mUniforms.end()) {
        glUniform4fv(mUniforms[uniform], 1, glm::value_ptr(vector));
        return;
    }
    GLint location = glGetUniformLocation(mHandle, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mHandle);
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vector));
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, float value) {
    if(mUniforms.find(uniform) != mUniforms.end()) {
        glUniform1f(mUniforms[uniform], value);
        return;
    }
    GLint location = glGetUniformLocation(mHandle, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mHandle);
        return;
    }
    glUniform1f(location, value);
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, int value) {
    if(mUniforms.find(uniform) != mUniforms.end()) {
        glUniform1i(mUniforms[uniform], value);
        return;
    }
    GLint location = glGetUniformLocation(mHandle, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mHandle);
        return;
    }
    glUniform1i(location, value);
    mUniforms[uniform] = location;
}
