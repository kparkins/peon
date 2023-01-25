#ifndef GAME_LOADERS_H
#define GAME_LOADERS_H

#include <filesystem>
#include <string>
#include <vector>

#include "Atlas.h"
#include "common/TypeAliases.h"
#include "graphics/opengl/GLProgram.h"
#include "graphics/opengl/GLShader.h"
#include "graphics/opengl/GLTexture2D.h"
#include "log/Logger.h"

using Peon::GLProgram;
using Peon::GLShader;
using Peon::GLTexture2D;
using Peon::LogLevel;
using Peon::MakeShared;
using Peon::ShaderType;
using Peon::Shared;
using std::function;
using std::string;
using std::vector;
using std::filesystem::directory_entry;
using std::filesystem::directory_iterator;

using std::filesystem::exists;

// minimal implmentation for convenience
class Loaders {
 public:
  static Shared<Atlas<GLTexture2D>> Textures(const string& directory) {
    LOG_INFO("Loading textures from - " << directory);
    auto atlas = MakeShared<Atlas<GLTexture2D>>();
    if (!exists(directory)) {
      LOG_ERROR(directory << " is not a directory.");
      return atlas;
    }
    for (const auto& entry : directory_iterator(directory)) {
      if (!entry.is_regular_file()) {
        continue;
      }
      auto& path = entry.path();
      GLTexture2D* texture = new GLTexture2D(path.string());
      atlas->Put(path.stem().string(), texture);
      LOG_INFO("Loaded texture - " << path.filename().string());
    }
    return atlas;
  }

  const static unordered_map<string, ShaderType> ShaderTypes;

  static Shared<Atlas<GLProgram>> Shaders(const string& directory) {
    unordered_map<string, vector<GLShader*>> shaders;
    auto atlas = MakeShared<Atlas<GLProgram>>();
    LOG_INFO("Compiling shaders in - " << directory);
    if (!exists(directory)) {
      LOG_ERROR(directory << " is not a directory.");
      return atlas;
    }
    // compile individual shaders
    for (const auto& entry : directory_iterator(directory)) {
      auto path = entry.path();
      auto filename = path.filename().string();
      auto extension = path.extension().string().substr(1);
      std::for_each(extension.begin(), extension.end(),
                    [](char& c) { c = static_cast<char>(std::tolower(c)); });
      auto it = ShaderTypes.find(extension);
      if (it == ShaderTypes.end()) {
        LOG_ERROR("Unknown shader file type - " << extension << " for file "
                                                << filename);
        continue;
      }
      auto basename = path.stem();
      GLShader* shader = new GLShader(it->second);
      if (!shader->Load(path.string())) {
        LOG_ERROR("Failed to compile shader - " << filename << " : "
                                                << shader->Error());
        continue;
      }
      LOG_INFO("Successfully compiled - " << filename);
      shaders[basename.string()].push_back(shader);
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