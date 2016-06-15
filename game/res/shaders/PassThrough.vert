#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 color;

void main() {
	gl_Position = vertexPosition;
	color = vertexColor;
}