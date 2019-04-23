#version 460 core

layout( location = 0 ) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 mvp;
uniform vec3 color;
out vec4 glColor;
void main() {
    gl_Position = vPosition;
	glColor = vec4(color, 1);
}
