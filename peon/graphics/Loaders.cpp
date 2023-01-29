#include "Loaders.h"

const unordered_map<string, ShaderType> Peon::Loaders::ShaderTypes = {
    {"vert", ShaderType::VERTEX},
    {"vs", ShaderType::VERTEX},
    {"frag", ShaderType::FRAGMENT},
    {"fs", ShaderType::FRAGMENT}};