#version 460 core

layout( location = 0 ) in vec3 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 view;

out vec4 glColor;

void main() {
    gl_Position = view * vec4(vPosition, 1);
	
	glColor = vColor;
}
