#version 460 core

in vec2 texCoord;
uniform sampler2D texture1;


layout (location = 0) out vec4 fColor;

void main() {
    fColor = texture(texture1, texCoord);//vec4(1,1,1,1);
}
