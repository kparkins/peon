/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "GLProgram.h"

Peon::GLProgram::GLProgram() : mLinked(false), mEnabled(false), mId(0) {
}

Peon::GLProgram::~GLProgram() {
    glDeleteProgram(mId);
}

void Peon::GLProgram::AddShader(GLuint type, const string & file) {
    this->AddShaderSource(type, Peon::ReadFile(file)); 
}

void Peon::GLProgram::AddShaderSource(GLuint type, const string & source) {
    GLint success = 0;
    GLchar errorLog[512];
    GLuint shader = glCreateShader(type);
    const char* code = source.c_str();
    glShaderSource(shader, 1, &code, 0);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, 0, errorLog);
        glDeleteShader(shader);
        LOG_ERROR("Unable to compile shader source. " << errorLog);
    }
    mShaders.push_back(shader);
}

void Peon::GLProgram::LinkProgram() {
    assert(!mLinked);
    mId = glCreateProgram();
    for (GLuint shader : mShaders) {
        glAttachShader(mId, shader);
    }
    glLinkProgram(mId);
    GLint linkSuccess = GL_TRUE;
    glGetProgramiv(mId, GL_LINK_STATUS, &linkSuccess);
    for (GLuint shader : mShaders) {
        glDetachShader(mId, shader);
        glDeleteShader(shader);
    }
    mShaders.clear();
    if (!linkSuccess) {
        GLchar errorLog[512];
        glGetShaderInfoLog(mId, 512, 0, errorLog);
        LOG_ERROR("Unable to link program. " << errorLog);
        return;
    }
    mLinked = true;
}

void Peon::GLProgram::Enable() {
    assert(mLinked);
    glUseProgram(mId);
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
    if (mUniforms.find(uniform) != mUniforms.end()) {
        glUniformMatrix4fv(mUniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
        return;
    }
    GLint location = glGetUniformLocation(mId, uniform.c_str());
    if (location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mId);
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, const vec3 & vector) {
    if (mUniforms.find(uniform) != mUniforms.end()) {
        glUniform3fv(mUniforms[uniform], 1, glm::value_ptr(vector));
        return;
    }
    GLint location = glGetUniformLocation(mId, uniform.c_str());
    if (location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mId);
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(vector));
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, const vec4 & vector) {
    if (mUniforms.find(uniform) != mUniforms.end()) {
        glUniform4fv(mUniforms[uniform], 1, glm::value_ptr(vector));
        return;
    }
    GLint location = glGetUniformLocation(mId, uniform.c_str());
    if (location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mId);
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vector));
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, float value) {
    if (mUniforms.find(uniform) != mUniforms.end()) {
        glUniform1f(mUniforms[uniform], value);
        return;
    }
    GLint location = glGetUniformLocation(mId, uniform.c_str());
    if (location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mId);
        return;
    }
    glUniform1f(location, value);
    mUniforms[uniform] = location;
}

void Peon::GLProgram::SetUniform(const string & uniform, int value) {
    if (mUniforms.find(uniform) != mUniforms.end()) {
        glUniform1i(mUniforms[uniform], value);
        return;
    }
    GLint location = glGetUniformLocation(mId, uniform.c_str());
    if (location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on GLShader program with id -- " << mId);
        return;
    }
    glUniform1i(location, value);
    mUniforms[uniform] = location;
}

GLint Peon::GLProgram::GetUniformLocation(const string & uniformName) {
    if (mUniforms.find(uniformName) != mUniforms.end()) {
        return mUniforms[uniformName];
    }
    GLint location = glGetUniformLocation(mId, uniformName.c_str());
    if (location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniformName << " on GLShader program with id -- " << mId);
    } else {
        mUniforms[uniformName] = location;
    }
    return location;
}