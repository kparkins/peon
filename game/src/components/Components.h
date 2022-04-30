

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
