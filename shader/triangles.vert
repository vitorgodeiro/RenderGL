#version 460 core

layout( location = 0 ) in vec3 vPosition;
layout (location = 1) in vec4 vColor;

out vec4 glColor;

void main() {
    gl_Position = vec4(vPosition, 1.0);
	glColor = vec4(1.0, 1.0, 1.0, 1.0);//vColor;
}
