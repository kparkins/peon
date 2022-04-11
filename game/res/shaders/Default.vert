#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 tex_coord;

uniform vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 icolor;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.f);
    icolor = color;
}