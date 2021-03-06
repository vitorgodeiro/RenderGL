#version 460 core

const vec3 lightPos = vec3(0.0, 10000.0, 0.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

in mat4 model_;
in vec3 eye_;
in vec3 aColor_;
in vec3 dColor_;
in vec3 sColor_;
in float mShine_;
in vec3 vNormal_;
in vec3 vPosition_;

in vec2 texCoord;
in vec3 glColor;
uniform sampler2D texture1;
uniform int textGL;
uniform int closeGL;



layout (location = 0) out vec4 fColor;

subroutine vec4 shadingType();
subroutine (shadingType) vec4 phong() {
	vec3 norm = normalize(mat3(transpose(inverse(model_))) * normalize(vNormal_));
    vec3 vPos = vec3(model_*vec4(vPosition_, 1));
	
	//ambient
	vec3 ambient = aColor_*lightColor;
	
	//difuse
	vec3 lightDir = normalize(lightPos - vPos);  
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = dColor_*lightColor*diff;
	
	vec3 specular = vec3(0.0);
	if (diff > 0.0) {
		vec3 viewDir = normalize(eye_ - vPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		//vec3 h = normalize(lightDir+viewDir);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), mShine_);
		specular = sColor_*spec*lightColor;  
	}	

	vec3 gamma = vec3(1.0/2.2);
    vec3 result = (ambient + diffuse + specular)*vec3(glColor);

	return vec4(pow(result, gamma), 1);
}

// option 1
subroutine (shadingType) vec4 normal() { return vec4(glColor, 1); } 

subroutine uniform shadingType shading;


void main() {
	if (textGL == 0 && closeGL == 0){
    	fColor = shading();
    } else {
    	fColor = texture(texture1, texCoord)*shading();
    }
}
