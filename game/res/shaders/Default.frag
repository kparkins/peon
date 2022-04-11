#version 330 core

in vec3 icolor;
out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(icolor, 1);
}