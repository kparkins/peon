/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "GLProgram.h"

Peon::GLProgram::GLProgram(const GLShader& first, const GLShader& second)
    : mEnabled(false), mProgram(static_cast<GLuint>(-1)) {
  assert(!this->IsLinked());
  mProgram = glCreateProgram();
  glAttachShader(mProgram, first.mId);
  glAttachShader(mProgram, second.mId);
  bool linked = this->Link();
  glDetachShader(mProgram, first.mId);
  glDetachShader(mProgram, second.mId);
  if (!linked) {
    glDeleteProgram(mProgram);
    mProgram = static_cast<GLuint>(-1);
    return;
  }
  this->SetupUniforms();
}

Peon::GLProgram::GLProgram(vector<GLShader*>& stages)
    : mEnabled(false), mProgram(static_cast<GLuint>(-1)) {
  assert(!this->IsLinked());
  mProgram = glCreateProgram();
  for (auto s : stages) {
    glAttachShader(mProgram, s->mId);
  }
  bool linked = this->Link();
  for (auto s : stages) {
    glDetachShader(mProgram, s->mId);
  }
  if (!linked) {
    glDeleteProgram(mProgram);
    mProgram = static_cast<GLuint>(-1);
    return;
  }
  this->SetupUniforms();
}

Peon::GLProgram::~GLProgram() {
  if (mProgram != static_cast<GLuint>(-1)) {
    glDeleteProgram(mProgram);
  }
}

void Peon::GLProgram::SetupUniforms() {
  if (!this->IsLinked()) {
    return;
  }
  int count = -1;
  glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &count);
  for (int i = 0; i < count; i++) {
    int len = -1;
    int num = -1;
    GLenum type = GL_ZERO;
    char name[64];
    glGetActiveUniform(mProgram, GLuint(i), sizeof(name) - 1, &len, &num, &type,
                       name);
    name[len] = 0;
    mUniforms[std::string(name)] = glGetUniformLocation(mProgram, name);
  }
}

bool Peon::GLProgram::Link() {
  glLinkProgram(mProgram);
  GLint linkSuccess = 0;
  glGetProgramiv(mProgram, GL_LINK_STATUS, &linkSuccess);
  return linkSuccess;
}

GLint Peon::GLProgram::GetUniformLocation(const string& uniformName) {
  assert(mUniforms.find(uniformName) != mUniforms.end());
  return mUniforms[uniformName];
}
