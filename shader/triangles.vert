#version 460 core

layout( location = 0 ) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
uniform vec3 color;

uniform vec3 eye;
uniform vec3 aColor;
uniform vec3 dColor;
uniform vec3 sColor;
uniform float mShine;
uniform int closeGL;

out vec2 texCoord;
out vec3 glColor;
out mat4 model_;
out vec3 eye_;
out vec3 aColor_;
out vec3 dColor_;
out vec3 sColor_;
out float mShine_;
out vec3 vNormal_;
out vec3 vPosition_;

const vec3 lightPos = vec3(0.0, 10000.0, 0.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

subroutine vec3 shadingType();

subroutine (shadingType) vec3 normal() { return color; } 

subroutine (shadingType) vec3 phong() { 
	model_ = model;
	eye_ = eye;
	aColor_ = aColor;
	dColor_ = dColor;
	sColor_ = sColor;
	mShine_ = mShine;
	vNormal_ = vNormal;
	vPosition_ = vPosition;
	return color; 
} 
 
// option 2
subroutine (shadingType) vec3 goroud() { 

vec3 norm = normalize(vNormal);
    vec3 surfacePos = (vPosition);
		
	//ambient
	vec3 ambient = aColor*lightColor;

	//difuse
	vec3 lightDir = normalize(lightPos - surfacePos);  
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = dColor*lightColor*diff;
	
	vec3 gamma = vec3(1.0/2.2);
    vec3 result = (ambient + diffuse)*color;
	return pow(result, gamma);

}

subroutine (shadingType) vec3 goroudFull() { 

vec3 norm = normalize(vNormal);
    vec3 surfacePos = (vPosition);
		
	//ambient
	vec3 ambient = aColor*lightColor;

	//difuse
	vec3 lightDir = normalize(lightPos - surfacePos);  
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = dColor*lightColor*diff;

	vec3 specular = vec3(0.0);
	if (diff > 0.0) {
		vec3 viewDir = normalize(eye - surfacePos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		vec3 h = normalize(lightDir+viewDir);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), mShine);
		specular = sColor*spec*lightColor;  
	}
	
	vec3 gamma = vec3(1.0/2.2);
    vec3 result = (ambient + diffuse + specular)*color;
	return pow(result, gamma);

}

subroutine uniform shadingType shading;


void main() {

	if (closeGL == 1) { gl_Position = vec4(vPosition, 1);} else {gl_Position = mvp*vec4(vPosition, 1);}
    texCoord = vTexCoord;
    glColor = shading();
}
