#ifndef GAME_COMPONENT_TYPES_H
#define GAME_COMPONENT_TYPES_H

#include <glm/glm.hpp>

#include "graphics/opengl/GLTexture2D.h"

using glm::vec3;
using Peon::GLTexture2D;

typedef struct BPMaterial {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
} BPMaterial;

typedef struct BPTextured {
  GLTexture2D* texture;
} BPTextured;

typedef struct BPLightMap {
  GLTexture2D* diffuse;
  GLTexture2D* specular;
  float shininess;
} BPLightMap;

typedef struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
} Light;

typedef struct StaticColor {
  vec3 color;
} StaticColor;

#endif