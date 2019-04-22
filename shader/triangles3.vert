#version 460 core

layout( location = 0 ) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
uniform vec3 color;

uniform vec3 eye;
uniform vec3 aColor;
uniform vec3 dColor;
uniform vec3 sColor;
uniform float mShine;

out vec3 glColor;

out mat4 model_;
out vec3 eye_;
out vec3 aColor_;
out vec3 dColor_;
out vec3 sColor_;
out float mShine_;
out vec3 vNormal_;
out vec3 vPosition_;

void main() {

    gl_Position = mvp*vec4(vPosition, 1.0);
    
	glColor = color;

	model_ = model;
	eye_ = eye;
	aColor_ = aColor;
	dColor_ = dColor;
	sColor_ = sColor;
	mShine_ = mShine;
	vNormal_ = vNormal;
	vPosition_ = vPosition;
}
