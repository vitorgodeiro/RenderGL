#version 460 core

layout( location = 0 ) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;

out vec2 texCoord;
out vec4 glColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
uniform vec3 color;


uniform vec3 eye;
uniform vec3 aColor;
uniform vec3 dColor;
uniform vec3 sColor;
uniform float mShine;

const vec3 lightPos = vec3(1.0, 1.0, 0.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main() {

	gl_Position = mvp*vec4(vPosition, 1);
    texCoord = vTexCoord;


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
	glColor = vec4(pow(result, gamma), 1);
    
}
