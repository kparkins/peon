#include "Loaders.h"

const unordered_map<string, ShaderType> Loaders::ShaderTypes = {
    {"vert", ShaderType::VERTEX},
    {"vs", ShaderType::VERTEX},
    {"frag", ShaderType::FRAGMENT},
    {"fs", ShaderType::FRAGMENT}};