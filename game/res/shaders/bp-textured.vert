#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

out vec3 vFragmentNormal;
out vec3 vFragmentPosition;
out vec2 vTextureCoordinate;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.f);
    vFragmentPosition = vec3(model * vec4(position, 1.f));
    vFragmentNormal = normalize(normalMatrix * normal);
    vTextureCoordinate = texture;
}