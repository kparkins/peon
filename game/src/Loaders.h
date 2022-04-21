#ifndef GAME_LOADERS_H
#define GAME_LOADERS_H

#include <string>
#include <vector>

#include "Atlas.h"
#include "common/Filesystem.h"
#include "common/TypeAliases.h"
#include "graphics/opengl/GLProgram.h"
#include "graphics/opengl/GLShader.h"
#include "graphics/opengl/GLTexture2D.h"
#include "log/Logger.h"

using Peon::GLProgram;
using Peon::GLShader;
using Peon::GLTexture2D;
using Peon::ListDirectory;
using Peon::LogLevel;
using Peon::MakeShared;
using Peon::ShaderType;
using Peon::Shared;
using std::function;
using std::string;
using std::vector;

// minimal implmentation for convenience
class Loaders {
 public:
  static Shared<Atlas<GLTexture2D>> Textures(const string& directory) {
    LOG_INFO("Loading textures from - " << directory);
    vector<string> files;
    ListDirectory(directory, files);
    auto atlas = MakeShared<Atlas<GLTexture2D>>();
    for (auto f : files) {
      string fullPath = directory + "/" + f;
      GLTexture2D* texture = new GLTexture2D(fullPath);
      string basename = f.substr(0, f.find_last_of('.'));
      atlas->Put(basename, texture);
      LOG_INFO("Loaded texture - " << f);
    }
    return atlas;
  }

  const static unordered_map<string, ShaderType> ShaderTypes;

  static Shared<Atlas<GLProgram>> Shaders(const string& directory) {
    vector<string> files;
    unordered_map<string, vector<GLShader*>> shaders;
    auto atlas = MakeShared<Atlas<GLProgram>>();
    ListDirectory(directory, files);
    LOG_INFO("Compiling shaders in - " << directory);
    // compile individual shaders
    for (auto f : files) {
      size_t extensionStart = f.find_last_of('.');
      string basename = f.substr(0, extensionStart);
      string extension = f.substr(extensionStart + 1);
      std::for_each(extension.begin(), extension.end(),
                    [](char& c) { c = static_cast<char>(std::tolower(c)); });
      auto it = ShaderTypes.find(extension);
      if (it == ShaderTypes.end()) {
        LOG_ERROR("Unknown shader file type - " << extension << " for file "
                                                << f);
        continue;
      }
      GLShader* shader = new GLShader(it->second);
      if (!shader->Load(directory + "/" + f)) {
        LOG_ERROR("Failed to compile shader - " << f << " : "
                                                << shader->Error());
        continue;
      }
      LOG_INFO("Successfully compiled - " << f);
      shaders[basename].push_back(shader);
    }
    // link individual shaders together into a program based on the filename
    for (auto& pair : shaders) {
      vector<GLShader*>& stages = pair.second;
      GLProgram* program = new GLProgram(stages);
      for (auto s : stages) {
        delete s;
      }
      if (!program->IsLinked()) {
        LOG_ERROR("Failed to create program - " << pair.first << " : "
                                                << program->Error());
        continue;
      }
      atlas->Put(pair.first, program);
      LOG_INFO("Created program - " << pair.first);
    }
    return atlas;
  }
};

#endif