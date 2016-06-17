//
// Created by Kyle on 2/15/2016.
//

#include "GLShader.h"

Peon::GLShader::GLShader() : mHandle(0) {
    mRefCount = new unsigned int;
    *mRefCount = 1;
}

Peon::GLShader::GLShader(GLuint type, const string & file) : mHandle(0) {
    this->Load(type, file);
    mRefCount = new unsigned int;
    *mRefCount = 1;
}

Peon::GLShader::GLShader(const GLShader & other) {
    if (mHandle && mHandle != other.mHandle) {
        --mRefCount;
        if (!mRefCount) {
            glDeleteShader(mHandle);
            delete mRefCount;
        }
    }
    mHandle = other.mHandle;
    mShaderType = other.mShaderType;
    mRefCount = other.mRefCount;
    ++(*mRefCount);
}

Peon::GLShader& Peon::GLShader::operator=(GLShader other) {
    if (mHandle && other.mHandle != mHandle) {
        --mRefCount;
        if (!mRefCount) {
            glDeleteShader(mHandle);
            delete mRefCount;
        }
    }
    mHandle = other.mHandle;
    mShaderType = other.mShaderType;
    mRefCount = other.mRefCount;
    ++(*mRefCount);
    return *this;
}

Peon::GLShader::~GLShader() {
    --(*mRefCount);
    if (mRefCount == 0) {
        glDeleteShader(mHandle);
        delete mRefCount;
    }
}

void Peon::GLShader::Load(GLuint type, const string & file) {
    string source = Peon::ReadFile(file);
    if (source == "") {
        LOG_ERROR("Could not read GLShader file " << file);
        return;
    }
    this->Compile(type, source);
}

void Peon::GLShader::Compile(GLuint type, const string & shader) {
    mShaderType = type;
    int success = 0;
    GLchar errorLog[512];
    const char* source = shader.c_str();
    mHandle = glCreateShader(type);
    glShaderSource(mHandle, 1, &source, 0);
    glCompileShader(mHandle);
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(mHandle, 512, 0, errorLog);
        LOG_ERROR(errorLog);
    }
}

GLuint Peon::GLShader::GetHandle() const {
    return mHandle;
}

GLuint Peon::GLShader::GetShaderType() const {
    return mShaderType;
}

