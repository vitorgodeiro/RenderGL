#version 460 core

layout( location = 0 ) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 mvp;
uniform vec3 color;
uniform int closeGL;

out vec4 glColor;

void main() {
	if (closeGL == 1) {	gl_Position = vPosition;} else {gl_Position = mvp*vPosition;}
	glColor = vec4(color, 1);
}
