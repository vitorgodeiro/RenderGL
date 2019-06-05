#version 460 core

layout( location = 0 ) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;


out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
uniform vec3 color;

void main() {

	gl_Position = mvp*vec4(vPosition, 1);
    texCoord = vTexCoord;

    
}
