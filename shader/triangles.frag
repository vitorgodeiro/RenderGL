#version 460 core

in vec4 glColor;

layout (location = 0) out vec4 fColor;

void main() {
    fColor = glColor;
}
