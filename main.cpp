#include "include/vgl.h"
#include "include/transformation.h"
#include "src/loadShaders.cpp"
#include "src/camera.cpp"
#include "src/mesh.cpp"
#include "include/mat4.h"
#include "include/vec4.h"
#include "include/stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>
#include <vector>
#include <limits>

#define PI 3.14159265
#define THETA 0.0174533f
#define COSTHETA float(cos(THETA))
#define SINTHETA float(sin(THETA))

GLint uniModel;
GLint uniView;
GLint uniMVP;
GLint uniColor;
GLint uniEye;
GLint uniAmbientColor;
GLint uniDiffuseColor;
GLint uniSpecularColor;
GLint uniMaterialShine ;
GLint uniCloseGl;

int checkShacder = 0;
int textureType = 0;

GLuint vao[1];
GLuint vbo[1];
GLuint shadingNormal;
GLuint shadingNormalF;

GLuint shadingGoroud;
GLuint shadingGoroudFull;
GLuint shadingPhong;
GLuint shadingPhongF;

GLuint textureID;
GLuint program;

GLfloat color[] = {1.0f, 1.0f, 1.0f};

int typeFormRender = 0;

bool closeGL = false;
bool backFaceGL = true;
bool ccw = false;
int phongGL = 0;

int width = 800;
int height = 600;
int w_, h_, nc_;
int w1_, h1_, nc1_;
int w2_, h2_, nc2_;
int w3_, h3_, nc3_;
int w4_, h4_, nc4_;
int w5_, h5_, nc5_;



int initialTime = time(NULL), finalTime, frameCount = 0, fps;
int type = GL_TRIANGLES;
int typePolMode = GL_FILL;
int typeFrontFace = GL_CCW;

Vec3 lightPos = Vec3(0.0, 20000.0, 0.0);
Vec3 lightColor = Vec3(1.0, 1.0, 1.0);


char texture = 'Y';

unsigned char *data = stbi_load("model/mandrill_256.jpg", &w_, &h_, &nc_, 0); 
unsigned char *data1 = stbi_load("model/mandrill_128.jpg", &w1_, &h1_, &nc1_, 0); 
unsigned char *data2 = stbi_load("model/mandrill_64.jpg", &w2_, &h2_, &nc2_, 0); 
unsigned char *data3 = stbi_load("model/mandrill_32.jpg", &w3_, &h3_, &nc3_, 0); 
unsigned char *data4 = stbi_load("model/mandrill_16.jpg", &w4_, &h4_, &nc4_, 0); 
unsigned char *data5 = stbi_load("model/mandrill_8.jpg", &w5_, &h5_, &nc5_, 0); 

float step = 1.0f;
float zNear, zFar;
float r;
float fDistance;
float *center = new float[3];

std::vector<float> zBuffer;
std::vector<Vec4> colorBuffer;

float getZBuffer(int i, int j){
	return zBuffer[j*width + i];
}

Vec4 getColorBuffer(int i, int j){
	return colorBuffer[j*width + i];
}

void setColorBuffer(int i, int j, Vec4 val){
	if (val[0] > 1.0f) {val[0] = 1.0f;} else if (val[0] < 0.0f) {val[0] = 0.0f;}
	if (val[1] > 1.0f) {val[1] = 1.0f;} else if (val[1] < 0.0f) {val[1] = 0.0f;}
	if (val[2] > 1.0f) {val[2] = 1.0f;} else if (val[2] < 0.0f) {val[2] = 0.0f;}
	colorBuffer[j*width + i] = val;
}

void setZBuffer(int i, int j, float val){
	zBuffer[j*width + i] = val;
}


//camera 
glm::vec3 eye;
glm::vec3 lookDir;
glm::vec3 up ;
glm::vec3 right;

glm::mat4 model, view, proj;
Mat4GL modelGL, viewGl, projGl, viewPortGL;

std::vector<GLfloat> vertexGL;
std::vector<GLfloat> vertexGL2;
std::vector<GLfloat> vertexGL3;
std::vector<GLfloat> vertexTexturePositionsGL;
std::vector<GLfloat> vertexNormalGL;


GLfloat ambientColor[3];
GLfloat diffuseColor[3];
GLfloat specularColor[3];
GLfloat materialShine;
	
Mesh *mesh;

void initBuffers(){
	zBuffer.clear();
	colorBuffer.clear();

	for (int i = 0; i < height*width; i++){
		zBuffer.push_back(std::numeric_limits<float>::infinity());
		colorBuffer.push_back(Vec4()); 
	}
}

void setVertex(float vertex[], float * vertex_, int numVertex){
	for (int i =0; i < numVertex; i++){
		vertex[i] = vertex_[i];
	}	
}

void mat4Vec (float vertex[], Mat4GL mvp){
	float a, b, c, d;
	a = mvp[0]*vertex[0] + mvp[1]*vertex[1] + mvp[2]*vertex[2] + mvp[3]*vertex[3];
	b = mvp[4]*vertex[0] + mvp[5]*vertex[1] + mvp[6]*vertex[2] + mvp[7]*vertex[3];
	c = mvp[8]*vertex[0] + mvp[9]*vertex[1] + mvp[10]*vertex[2] + mvp[11]*vertex[3];
	d = mvp[12]*vertex[0] + mvp[13]*vertex[1] + mvp[14]*vertex[2] + mvp[15]*vertex[3];

	vertex[0] = a;
	vertex[1] = b;
	vertex[2] = c;
	vertex[3] = d;
}

std::vector<float> listV0;
std::vector<float> listT0;
std::vector<float> colorsV0;
std::vector<float> listV1;
std::vector<float> listT1;
std::vector<float> colorsV1;


Vec3 reflect(Vec3 I, Vec3 N){
	return I - 2.0f*Vec3::dot(N, I)*N;
}


Vec3 shading(float u, float v, float n, float normalX, float normalY, float normalZ){
	glm::vec3 ambientGLM = glm::vec3(ambientColor[0], ambientColor[1], ambientColor[2])*glm::vec3(lightColor[0], lightColor[1], lightColor[2]);

	glm::vec3  glmNormal = glm::normalize(glm::mat3(glm::transpose(glm::inverse(model)))*glm::vec3(normalX, normalY, normalZ));
	glm::vec3 vPos = glm::vec3(u, v, n);
	  
	glm::vec3 lightDirGLM = glm::normalize(glm::vec3(lightPos[0], lightPos[1], lightPos[2]) - vPos);
	float diffGLM = glm::max(glm::dot(lightDirGLM, glmNormal), 0.0f);
	glm::vec3 diffuseGLM = glm::vec3(diffuseColor[0], diffuseColor[1], diffuseColor[2])*glm::vec3(lightColor[0], lightColor[1], lightColor[2])*diffGLM;
	
	glm::vec3 specular = glm::vec3(0.0f);
	//Specular
	if (diffGLM > 0.0) {
		//Vec3 viewDir = Vec3::unit_vector(Vec3(eye[0], eye[1], eye[2]) - Vec3(u, v, n));
		glm::vec3 viewDir = glm::normalize(glm::vec3(eye[0], eye[1], eye[2]) - vPos);
		glm::vec3 reflectDir = glm::reflect(-lightDirGLM, glmNormal);  
		float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), materialShine);
		specular = glm::vec3(specularColor[0], specularColor[1], specularColor[2])*spec*glm::vec3(lightColor[0], lightColor[1], lightColor[2]);  
	}

	glm::vec3 resultGLM = (ambientGLM + diffuseGLM )*glm::vec3(color[0], color[1], color[2]);

	//glm::vec3 gamma = glm::vec3(1.0/2.2);
    //resultGLM = glm::pow(resultGLM, gamma);

	if (resultGLM[0] > 1.0f) {resultGLM[0] = 1.0f;} else if (resultGLM[0] < 0.0f) {resultGLM[0] = 0.0f;}
	if (resultGLM[1] > 1.0f) {resultGLM[1] = 1.0f;} else if (resultGLM[1] < 0.0f) {resultGLM[1] = 0.0f;}
	if (resultGLM[2] > 1.0f) {resultGLM[2] = 1.0f;} else if (resultGLM[2] < 0.0f) {resultGLM[2] = 0.0f;}

	return Vec3(resultGLM[0], resultGLM[1], resultGLM[2]);
}

void compute(float vertex[], Mat4GL m, Mat4GL v, Mat4GL p, int numtriangles, float vertexNormal[], float texturePos[]){
	float v1[4], v2[4], v3[4];
	float n1[4], n2[4], n3[4];
	float text1[2], text2[2], text3[2];
	vertexGL.clear();
	vertexGL2.clear();
	vertexGL3.clear();
	vertexTexturePositionsGL.clear();
	vertexNormalGL.clear();
	Vec3 c1; Vec3 c2; Vec3 c3;
	float a;
	
	for (int i =0; i < numtriangles; i++){
		
		v1[0] = vertex[i*9];
		v1[1] = vertex[i*9 + 1];
		v1[2] = vertex[i*9 + 2];
		v1[3] = 1;
		n1[0] = vertexNormal[i*9]; n1[1] = vertexNormal[i*9 + 1]; n1[2] = vertexNormal[i*9 + 2]; n1[3] = 1;
		text1[0] = texturePos[i*6]; text1[1] = texturePos[i*6+1];

		v2[0] = vertex[i*9 + 3];
		v2[1] = vertex[i*9 + 4];
		v2[2] = vertex[i*9 + 5];
		v2[3] = 1;
		n2[0] = vertexNormal[i*9 +3]; n2[1] = vertexNormal[i*9 + 4]; n2[2] = vertexNormal[i*9 + 5]; n2[3] = 1;
		text2[0] = texturePos[i*6+2]; text2[1] = texturePos[i*6+3]; 

		v3[0] = vertex[i*9 + 6];
		v3[1] = vertex[i*9 + 7];
		v3[2] = vertex[i*9 + 8];
		v3[3] = 1;
		n3[0] = vertexNormal[i*9 + 6]; n3[1] = vertexNormal[i*9 + 7]; n3[2] = vertexNormal[i*9 + 8]; n3[3] = 1;
		text3[0] = texturePos[i*6+4]; text3[1] = texturePos[i*6+5]; 
				
		mat4Vec(v1, modelGL);
		mat4Vec(v2, modelGL);
		mat4Vec(v3, modelGL);		
		
		if (phongGL == 1){
			c1 = shading(v1[0], v1[1], v1[2], n1[0], n1[1], n1[2]);
			c2 = shading(v2[0], v2[1], v2[2], n2[0], n2[1], n2[2]);
			c3 = shading(v3[0], v3[1], v3[2], n3[0], n3[1], n3[2]);
		}else if(phongGL == 2){
			c1 = Vec3(n1[0], n1[1], n1[2]);
			c2 = Vec3(n2[0], n2[1], n2[2]);
			c3 = Vec3(n3[0], n3[1], n3[2]);
		}
		
		mat4Vec(v1, (projGl*viewGl));
		mat4Vec(v2, (projGl*viewGl));
		mat4Vec(v3, (projGl*viewGl));

		//Verificando se está no NDC e se W > 0
		if (v1[3] > 0 && v2[3] > 0 && v3[3] > 0 && -1.0f < (v1[2]/v1[3]) && (v1[2]/v1[3]) < 1.0f && -1.0f < (v2[2]/v2[3]) && (v2[2]/v2[3]) < 1.0f && -1.0f < (v3[2]/v3[3]) && (v3[2]/v3[3]) <1.0f &&
			(v1[0]/v1[3]) < 1.0f && -1.0f < (v1[0]/v1[3]) && (v2[0]/v2[3]) < 1.0f && -1.0f < (v2[0]/v2[3]) && (v3[0]/v3[3]) < 1.0f && -1.0f < (v3[0]/v3[3]) &&
			(v1[1]/v1[3]) < 1.0f && -1.0f < (v1[1]/v1[3]) && (v2[1]/v2[3]) < 1.0f && -1.0f < (v2[1]/v2[3]) && (v3[1]/v3[3]) < 1.0f && -1.0f < (v3[1]/v3[3])){

			//c1 = c1/v1[3];
			//c2 = c2/v2[3];
			//c3 = c3/v3[3];
			float w1 = v1[3];
			float w2 = v2[3];
			float w3 = v3[3];

			//Divisao por w
			v1[0] = v1[0]/v1[3]; v1[1] = v1[1]/v1[3]; v1[2] = v1[2]/v1[3]; v1[3] = v1[3]/v1[3]; 
			v2[0] = v2[0]/v2[3]; v2[1] = v2[1]/v2[3]; v2[2] = v2[2]/v2[3]; v2[3] = v2[3]/v2[3];
			v3[0] = v3[0]/v3[3]; v3[1] = v3[1]/v3[3]; v3[2] = v3[2]/v3[3]; v3[3] = v3[3]/v3[3]; 
			
			a = 0.5*((v1[0]*v2[1] - v2[0]*v1[1]) + v2[0]*v3[1] - v3[0]*v2[1] + v3[0]*v1[1] - v1[0]*v3[1]);
			if ((a < 0 and ccw) or (a > 0 and !ccw)or !backFaceGL) {
				
				vertexGL.push_back(v1[0]);
				vertexGL.push_back(v1[1]);
				vertexGL.push_back(v2[0]);
				vertexGL.push_back(v2[1]);
				vertexGL.push_back(v3[0]);
				vertexGL.push_back(v3[1]);
				
				mat4Vec(v1, viewPortGL);
				mat4Vec(v2, viewPortGL);
				mat4Vec(v3, viewPortGL);

				v1[3] = w1;
				v2[3] = w2;
				v3[3] = w3;
							
				vertexGL2.push_back(v1[0]); vertexGL2.push_back(v1[1]); vertexGL2.push_back(v1[2]); vertexGL2.push_back(v1[3]);
				vertexGL2.push_back(v2[0]); vertexGL2.push_back(v2[1]); vertexGL2.push_back(v2[2]); vertexGL2.push_back(v2[3]);
				vertexGL2.push_back(v3[0]); vertexGL2.push_back(v3[1]); vertexGL2.push_back(v3[2]); vertexGL2.push_back(v3[3]);
				
				

				vertexGL3.push_back(c1[0]); vertexGL3.push_back(c1[1]); vertexGL3.push_back(c1[2]);
				vertexGL3.push_back(c2[0]); vertexGL3.push_back(c2[1]); vertexGL3.push_back(c2[2]);
				vertexGL3.push_back(c3[0]); vertexGL3.push_back(c3[1]); vertexGL3.push_back(c3[2]);

				vertexTexturePositionsGL.push_back(text1[0]); vertexTexturePositionsGL.push_back(text1[1]); 
				vertexTexturePositionsGL.push_back(text2[0]); vertexTexturePositionsGL.push_back(text2[1]); 
				vertexTexturePositionsGL.push_back(text3[0]); vertexTexturePositionsGL.push_back(text3[1]); 


			}
		}	
	}
}


void setColorMipMapping(float *colorF, int level, float px, float py, float w){
    float c1, c2, c3;
    switch (level){
    	case 1: {
    		float px_ = px*(h1_ - 1);
    		float py_ = py*(w1_ - 1);
    		int x = std::floor(px_);
    		int y = std::floor(py_);
    		float u = px_ - x;
    		float v = py_ - y;

    	 	c1 = ((1-u)*data1[(w1_*y + x)*3]/w + u*data1[(w1_*y + (x+1))*3]/w)*(1-v) + ((1-u)*data1[(w1_*(y+1) + x)*3]/w + u*data1[(w1_*(y+1) + (x+1))*3]/w)*v;
    	 	c2 = ((1-u)*data1[(w1_*y + x)*3+1]/w + u*data1[(w1_*y + (x+1))*3+1]/w)*(1-v) + ((1-u)*data1[(w1_*(y+1) + x)*3+1]/w + u*data1[(w1_*(y+1) + (x+1))*3+1]/w)*v;
    	 	c3 = ((1-u)*data1[(w1_*y + x)*3+2]/w + u*data1[(w1_*y + (x+1))*3+2]/w)*(1-v) + ((1-u)*data1[(w1_*(y+1) + x)*3+2]/w + u*data1[(w1_*(y+1) + (x+1))*3+2]/w)*v;
    		break;	
    	}
    	case 2: {
    		float px_ = px*(h2_ - 1);
    		float py_ = py*(w2_ - 1);
    		int x = std::floor(px_);
    		int y = std::floor(py_);
    		float u = px_ - x;
    		float v = py_ - y;

    	 	c1 = ((1-u)*data2[(w2_*y + x)*3]/w + u*data2[(w2_*y + (x+1))*3]/w)*(1-v) + ((1-u)*data2[(w2_*(y+1) + x)*3]/w + u*data2[(w2_*(y+1) + (x+1))*3]/w)*v;
    	 	c2 = ((1-u)*data2[(w2_*y + x)*3+1]/w + u*data2[(w2_*y + (x+1))*3+1]/w)*(1-v) + ((1-u)*data2[(w2_*(y+1) + x)*3+1]/w + u*data2[(w2_*(y+1) + (x+1))*3+1]/w)*v;
    	 	c3 = ((1-u)*data2[(w2_*y + x)*3+2]/w + u*data2[(w2_*y + (x+1))*3+2]/w)*(1-v) + ((1-u)*data2[(w2_*(y+1) + x)*3+2]/w + u*data2[(w2_*(y+1) + (x+1))*3+2]/w)*v;
    		break;
    	}
    	case 3: {
    		float px_ = px*(h3_ - 1);
    		float py_ = py*(w3_ - 1);
    		int x = std::floor(px_);
    		int y = std::floor(py_);
    		float u = px_ - x;
    		float v = py_ - y;

    	 	c1 = ((1-u)*data3[(w3_*y + x)*3]/w + u*data3[(w3_*y + (x+1))*3]/w)*(1-v) + ((1-u)*data3[(w3_*(y+1) + x)*3]/w + u*data3[(w3_*(y+1) + (x+1))*3]/w)*v;
    	 	c2 = ((1-u)*data3[(w3_*y + x)*3+1]/w + u*data3[(w3_*y + (x+1))*3+1]/w)*(1-v) + ((1-u)*data3[(w3_*(y+1) + x)*3+1]/w + u*data3[(w3_*(y+1) + (x+1))*3+1]/w)*v;
    	 	c3 = ((1-u)*data3[(w3_*y + x)*3+2]/w + u*data3[(w3_*y + (x+1))*3+2]/w)*(1-v) + ((1-u)*data3[(w3_*(y+1) + x)*3+2]/w + u*data3[(w3_*(y+1) + (x+1))*3+2]/w)*v;
    		break;
    	}
    	case 4: {
    		float px_ = px*(h4_ - 1);
    		float py_ = py*(w4_ - 1);
    		int x = std::floor(px_);
    		int y = std::floor(py_);
    		float u = px_ - x;
    		float v = py_ - y;

    	 	c1 = ((1-u)*data4[(w4_*y + x)*3]/w + u*data4[(w4_*y + (x+1))*3]/w)*(1-v) + ((1-u)*data4[(w4_*(y+1) + x)*3]/w + u*data4[(w4_*(y+1) + (x+1))*3]/w)*v;
    	 	c2 = ((1-u)*data4[(w4_*y + x)*3+1]/w + u*data4[(w4_*y + (x+1))*3+1]/w)*(1-v) + ((1-u)*data4[(w4_*(y+1) + x)*3+1]/w + u*data4[(w4_*(y+1) + (x+1))*3+1]/w)*v;
    	 	c3 = ((1-u)*data4[(w4_*y + x)*3+2]/w + u*data4[(w4_*y + (x+1))*3+2]/w)*(1-v) + ((1-u)*data4[(w4_*(y+1) + x)*3+2]/w + u*data4[(w4_*(y+1) + (x+1))*3+2]/w)*v;
    		break;
    	}
    	case 5: {    		
    		float px_ = px*(h5_ - 1);
    		float py_ = py*(w5_ - 1);
    		int x = std::floor(px_);
    		int y = std::floor(py_);
    		float u = px_ - x;
    		float v = py_ - y;

    	 	c1 = ((1-u)*data5[(w5_*y + x)*3]/w + u*data5[(w5_*y + (x+1))*3]/w)*(1-v) + ((1-u)*data5[(w5_*(y+1) + x)*3]/w + u*data5[(w5_*(y+1) + (x+1))*3]/w)*v;
    	 	c2 = ((1-u)*data5[(w5_*y + x)*3+1]/w + u*data5[(w5_*y + (x+1))*3+1]/w)*(1-v) + ((1-u)*data5[(w5_*(y+1) + x)*3+1]/w + u*data5[(w5_*(y+1) + (x+1))*3+1]/w)*v;
    	 	c3 = ((1-u)*data5[(w5_*y + x)*3+2]/w + u*data5[(w5_*y + (x+1))*3+2]/w)*(1-v) + ((1-u)*data5[(w5_*(y+1) + x)*3+2]/w + u*data5[(w5_*(y+1) + (x+1))*3+2]/w)*v;
    		break;
    	}
    	default:{
    		float px_ = px*(h_ - 1);
    		float py_ = py*(w_ - 1);
    		int x = std::floor(px_);
    		int y = std::floor(py_);
    		float u = px_ - x;
    		float v = py_ - y;

    	 	c1 = ((1-u)*data[(w_*y + x)*3]/w + u*data[(w_*y + (x+1))*3]/w)*(1-v) + ((1-u)*data[(w_*(y+1) + x)*3]/w + u*data[(w_*(y+1) + (x+1))*3]/w)*v;
    	 	c2 = ((1-u)*data[(w_*y + x)*3+1]/w + u*data[(w_*y + (x+1))*3+1]/w)*(1-v) + ((1-u)*data[(w_*(y+1) + x)*3+1]/w + u*data[(w_*(y+1) + (x+1))*3+1]/w)*v;
    	 	c3 = ((1-u)*data[(w_*y + x)*3+2]/w + u*data[(w_*y + (x+1))*3+2]/w)*(1-v) + ((1-u)*data[(w_*(y+1) + x)*3+2]/w + u*data[(w_*(y+1) + (x+1))*3+2]/w)*v;
    	}
    }

    colorF[0] = (float) (c1/255.0f)/(1/w);
	colorF[1] = (float) (c2/255.0f)/(1/w);
	colorF[2] = (float) (c3/255.0f)/(1/w);

}

void setColorTexture(float px, float py, float w, int type, float *colorF, float level){
	float px_; 
    float py_; 
    int pos; 

    //Nearest-Neighbor Resampling
    if (type == 0){
    	px_ = round(px*(h_ - 1));
    	py_ = round(py*(w_ - 1));
    	pos = (w_*py_ + px_)*3; 
    	colorF[0] = (float) (data[pos]/255.0f);
		colorF[1] = (float) (data[pos+1]/255.0f);
		colorF[2] = (float) (data[pos+2]/255.0f);

    } else if (type == 1){ //Bilinear Resampling    	
    	px_ = px*(h_ - 1);
    	py_ = py*(w_ - 1);
    	int x = std::floor(px_);
    	int y = std::floor(py_);
    	float u = px_ - x;
    	float v = py_ - y;

    	float c1 = ((1-u)*data[(w_*y + x)*3]/w + u*data[(w_*y + (x+1))*3]/w)*(1-v) + ((1-u)*data[(w_*(y+1) + x)*3]/w + u*data[(w_*(y+1) + (x+1))*3]/w)*v;
    	float c2 = ((1-u)*data[(w_*y + x)*3+1]/w + u*data[(w_*y + (x+1))*3+1]/w)*(1-v) + ((1-u)*data[(w_*(y+1) + x)*3+1]/w + u*data[(w_*(y+1) + (x+1))*3+1]/w)*v;
    	float c3 = ((1-u)*data[(w_*y + x)*3+2]/w + u*data[(w_*y + (x+1))*3+2]/w)*(1-v) + ((1-u)*data[(w_*(y+1) + x)*3+2]/w + u*data[(w_*(y+1) + (x+1))*3+2]/w)*v;

    	colorF[0] = (float) (c1/255.0f)/(1/w);
		colorF[1] = (float) (c2/255.0f)/(1/w);
		colorF[2] = (float) (c3/255.0f)/(1/w);
    }else if (type == 2){ //Mip-mapping Resampling  
    	int l1 = std::floor (level);
    	int l2 = l1+1;
    	float f = level - l1;
    	float color1[3] = {0.0f, 0.0f, 0.0f};
    	float color2[3] = {0.0f, 0.0f, 0.0f};
    	setColorMipMapping(color1, l1, px, py, w);
    	setColorMipMapping(color2, l2, px, py, w);
    	colorF[0] = (color2[0]/w*f + (1.0f-f)*color1[0]/w)*w;
    	colorF[1] = (color2[1]/w*f + (1.0f-f)*color1[1]/w)*w;
    	colorF[2] = (color2[2]/w*f + (1.0f-f)*color1[2]/w)*w;
    }
}

//Bresenham’s Line Drawing Algorithm
void line(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, Vec3 color1, Vec3 color2, float tx1, float ty1, float tx2, float ty2) { 
	ty1 = 1 - ty1;
	ty2 = 1 - ty2;

    if (x0 > x1) { 
    	std::swap(x0, x1); 
    	std::swap(y0, y1); 
    	std::swap(z0, z1); 
    	std::swap(color1, color2);
    	std::swap(tx1, tx2);
    	std::swap(ty1, ty2);
	}     

    float dx = x1-x0; float dz = z1-z0; float dw = w1-w0;
    int y = y0; 
    
    float derrorZ = 0;	
    float derrorW = 0;
    float derrorTx = 0;	
    float derrorTy = 0;
    
    float z = z0; 
  	float w = w0;

  	float colorF[3] = {1, 1, 1};
  	float colorT[3] = {1, 1, 1};
  	double f;
	Vec3 n;

	float px = 0;
    float py = 0;

  	if (dx != 0.0f) {
    	derrorZ = std::abs(dz/float(dx)); 
    	derrorW = std::abs(dw/float(dx)); 
    	derrorTx = std::abs((tx2-tx1)/float(dx)); 
    	derrorTy = std::abs((ty2-ty1)/float(dx));
    }

    Vec3 n1 = color1;
    Vec3 n2 = color2;

    float max = (std::max(derrorTx/(1/w)*256, derrorTy/(1/w)*256));
    float level = 0;
    if (max > 0.0f) {level = (std::log(max))/std::log(2) - 1;}
    
    for (int x=x0; x<=x1; x++) {     	
    	if (getZBuffer(x, y) > z){
    		colorT[0] = 1; colorT[1] = 1; colorT[2] = 1;
	    	colorF[0] = 1; colorF[1] = 1; colorF[2] = 1;
    	
    		if (x1==x0) {f = 0;} else {f = ((double(x-x0)/double(dx)));}
    		
    		if (texture == 'Y'){    			
    			px = (tx2/w*f + (1.0f-f)*tx1/w); 
    			py = (ty2/w*f + (1.0f-f)*ty1/w); 

    			px = px*w;
    			py = py*w;	
    			if (px > 1.0f) {px = 1.0f;} else if (px < 0.0f){px = 0.0f;}
    			if (py > 1.0f) {py = 1.0f;} else if (py < 0.0f){py = 0.0f;}
    			setColorTexture(px, py, w, textureType, colorT, level);
    		}     		
    		if (phongGL==2){    			
    			n[0] = n2[0]/w*f + (1.0f-f)*n1[0]/w;
	    		n[1] = n2[1]/w*f + (1.0f-f)*n1[1]/w;
    			n[2] = n2[2]/w*f + (1.0f-f)*n1[2]/w;

    			n[0] = n[0]*w; n[1] = n[1]*w; n[2] = n[2]*w;
    			Vec3 c = shading(x, y, z, n[0], n[1], n[2]);	
    			colorF[0] = c[0]; colorF[1] = c[1]; colorF[2] = c[2];
    		}
    		else if (phongGL == 1 ){
    			colorF[0] = (color2[0]/w*f + (1.0f-f)*color1[0]/w);
    			colorF[1] = (color2[1]/w*f + (1.0f-f)*color1[1]/w);
    			colorF[2] = (color2[2]/w*f + (1.0f-f)*color1[2]/w);

    			colorF[0] = colorF[0]*w;
    			colorF[1] = colorF[1]*w;
    			colorF[2] = colorF[2]*w;
    		}
	    		
    		setColorBuffer(x, y, Vec4(std::pow((colorF[0]), 1.0/2.2)*colorT[0], std::pow(colorF[1], 1.0/2.2)*colorT[1], std::pow(colorF[2], 1.0/2.2)*colorT[2], 1.0f)); 
    		setZBuffer(x, y, z);
    	} 
       	
       	z += derrorZ;
       	w += derrorW;
    } 	
}

void fillBottomFlatTriangle(Vec4 v1, Vec3 c1, float tx1, float ty1, Vec4 v2, Vec3 c2, float tx2, float ty2, Vec4 v3, Vec3 c3, float tx3, float ty3){
	float invslope1 = (v2[0] - v1[0]) / (v2[1] - v1[1]);
  	float invslope2 = (v3[0] - v1[0]) / (v3[1] - v1[1]);

  	float curx1 = v1[0];
  	float curx2 = v1[0];

  	float dy = v2[1] - v1[1];

  	float f = 0;
  	float w1, z1;
  	float w2, z2;
  	for (int scanlineY = v1[1]; scanlineY <= v2[1]; scanlineY++){
  		f = (scanlineY - v1[1])/dy;
  		
  		w1 = f*v2[3]+(1-f)*v1[3]; 
  		z1 = f*(v2[2]*w1)+(1-f)*(v1[2]/w1);

  		w2 = f*v3[3]+(1-f)*v1[3]; 
  		z2 = f*(v3[2]/w2)+(1-f)*(v1[2]/w2);

  		line((int)curx1, scanlineY, z1/(1/w1), w1, (int)curx2, scanlineY, z2/(1/w2), w2, (f*(c2/w1)+(1-f)*(c1/w1))/(1/w1), (f*(c3/w2)+(1-f)*(c1/w2))/(1/w2), (f*(tx2/w1)+(1-f)*(tx1/w1))/(1/w1), (f*(ty2/w1)+(1-f)*(ty1/w1))/(1/w1), (f*(tx3/w2)+(1-f)*(tx1/w2))/(1/w2), (f*(ty3/w2)+(1-f)*(ty1/w2))/(1/w2));
    	curx1 += invslope1;
    	curx2 += invslope2;
  	}
}

void fillTopFlatTriangle(Vec4 v1, Vec3 c1, float tx1, float ty1, Vec4 v2, Vec3 c2, float tx2, float ty2, Vec4 v3, Vec3 c3, float tx3, float ty3){
	float invslope1 = (v3[0] - v1[0]) / (v3[1] - v1[1]);
  	float invslope2 = (v3[0] - v2[0]) / (v3[1] - v2[1]);

  	float curx1 = v3[0];
  	float curx2 = v3[0];

  	float dy = v3[1] - v1[1];

  	float f = 0;
  	float w1, z1;
  	float w2, z2;
  	for (int scanlineY = v3[1]; scanlineY >= v1[1]; scanlineY--){
  		f = (scanlineY - v1[1])/dy;
  		w1 = f*v3[3]+(1-f)*v1[3];
  		z1 = f*(v3[2]/w1)+(1-f)*(v1[2]/w1);

  		w2 = f*v3[3]+(1-f)*v2[3];
  		z2 = f*(v3[2]/w2)+(1-f)*(v2[2]/w2);
  		line((int)curx1, scanlineY, z1/(1/w1), w1, (int)curx2, scanlineY, z2/(1/w2), w2, (f*(c3/w1)+(1-f)*(c1/w1))/(1/w1), (f*(c3/w2)+(1-f)*(c2/w2))/(1/w2), (f*(tx3/w1)+(1-f)*(tx1/w1))/(1/w1), (f*(ty3/w1)+(1-f)*(ty1/w1))/(1/w1), (f*(tx3/w2)+(1-f)*(tx2/w2))/(1/w2), (f*(ty3/w2)+(1-f)*(ty2/w2))/(1/w2));
    	curx1 -= invslope1;
    	curx2 -= invslope2;
  	}
}

void drawTriangle(Vec4 v1, Vec3 c1, float tx1, float ty1, Vec4 v2, Vec3 c2, float tx2, float ty2, Vec4 v3, Vec3 c3, float tx3, float ty3){
	if (v2[1] == v3[1]) {
		fillBottomFlatTriangle(v1, c1, tx1, ty1, v2, c2, tx2, ty2, v3, c3, tx3, ty3);
	} else if (v1[1] == v2[1]) {
		fillTopFlatTriangle(v1, c1, tx1, ty1, v2, c2, tx2, ty2, v3, c3, tx3, ty3);
	}else{
		float t = (v2[1] - v1[1])/(v3[1]-v1[1]);
		Vec4 v4 = v1 + t*(v3-v1);
		Vec3 c4 = c1 + t*(c3-c1);
		float tx4 = tx1 + t*(tx3-tx1);
		float ty4 = ty1 + t*(ty3-ty1);
		fillBottomFlatTriangle(v1, c1, tx1, ty1, v2, c2, tx2, ty2, v4, c4, tx4, ty4);
		fillTopFlatTriangle(v2, c2, tx2, ty2, v4, c4, tx4, ty4, v3, c3, tx3, ty3);
		
	}
}

void raster(){
	float u1, u2, u3, v1, v2, v3, n1, n2, n3, w1, w2, w3, tx1, tx2, tx3, ty1, ty2, ty3;
	
	for (unsigned int i = 0, j = 0, t = 0; i < vertexGL2.size(); i+=12, j+=9, t+=6){

		u1 = (int)round(vertexGL2[i]);
		v1 = (int)round(vertexGL2[i + 1]);
		n1 = vertexGL2[i + 2];
		w1 = vertexGL2[i + 3];

		u2 = (int)round(vertexGL2[i + 4]);
		v2 = (int)round(vertexGL2[i + 5]);
		n2 = vertexGL2[i + 6];
		w2 = vertexGL2[i + 7];

		u3 = (int)round(vertexGL2[i + 8]);
		v3 = (int)round(vertexGL2[i + 9]);
		n3 = vertexGL2[i + 10];
		w3 = vertexGL2[i + 11];

		tx1 = vertexTexturePositionsGL[t];
		ty1 = vertexTexturePositionsGL[t+1];
		tx2 = vertexTexturePositionsGL[t+2];
		ty2 = vertexTexturePositionsGL[t+3];
		tx3 = vertexTexturePositionsGL[t+4];
		ty3 = vertexTexturePositionsGL[t+5];

		Vec3 color1, color2, color3;

		color1 = Vec3(vertexGL3[j], vertexGL3[j+1], vertexGL3[j+2]);
		color2 = Vec3(vertexGL3[j+3], vertexGL3[j+4], vertexGL3[j+5]);
		color3 = Vec3(vertexGL3[j+6], vertexGL3[j+7], vertexGL3[j+8]);

		listV0.clear();
		listV1.clear();
		listT0.clear();
		listT1.clear();
		colorsV0.clear();
		colorsV1.clear();

		if (typeFormRender == 0){

			if (v1 > v2) { std::swap(u1, u2); std::swap(v1, v2); std::swap(n1, n2); std::swap(w1, w2); std::swap(color1, color2); std::swap(tx1, tx2); std::swap(ty1, ty2); }
			if (v1 > v3) { std::swap(u1, u3); std::swap(v1, v3); std::swap(n1, n3); std::swap(w1, w3); std::swap(color1, color3); std::swap(tx1, tx3); std::swap(ty1, ty3); }
			if (v2 > v3) { std::swap(u2, u3); std::swap(v2, v3); std::swap(n2, n3); std::swap(w2, w3); std::swap(color2, color3); std::swap(tx2, tx3); std::swap(ty2, ty3); }

			drawTriangle(Vec4(u1, v1, n1, w1), color1, tx1, ty1, Vec4(u2, v2, n2, w2), color2, tx2, ty2, Vec4(u3, v3, n3, w3), color3, tx3, ty3);
		}else if (typeFormRender == 1) {		
		//	line(u1, v1, n1, w1, u2, v2, n2, w2, 3, color1, color2);
			//line(u1, v1, n1, w1, u3, v3, n3, w3, 3, color1, color3);
			//line(u2, v2, n2, w2, u3, v3, n3, w3, 3, color2, color3);			
		}
		else if(typeFormRender == 2){
			//if (getZBuffer(u1, v1) > n1){ setColorBuffer(u1, v1, Vec4(color[0], color[1], color[2], 1.0f)); setZBuffer(u1, v1, n1);}
			//if (getZBuffer(u2, v2) > n2){ setColorBuffer(u2, v2, Vec4(color[0], color[1], color[2], 1.0f)); setZBuffer(u2, v2, n2);}
			//if (getZBuffer(u3, v3) > n3){ setColorBuffer(u3, v3, Vec4(color[0], color[1], color[2], 1.0f)); setZBuffer(u3, v3, n3);}
		}
	}
}

void setTextureType(){
	switch (textureType){
		case 0:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    	std::cout << "Nearest\n";
	    	break;
	    case 1:
	    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   	    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   	    	std::cout << "Linear\n";
   	    	break;
   	    case 2:
   	    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   	    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   	    	glGenerateMipmap(GL_TEXTURE_2D);
   	    	std::cout << "MipMap\n";
	}
}

void updateMVP(void){	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-center[0], -center[1], -center[2])); 
	view = glm::lookAt( eye, eye + lookDir, up);
	proj = glm::perspective((float)( 30.0f * PI / 180.0f ), 1.3333f,zNear, zFar);
	glUniform3fv(uniEye, 1, glm::value_ptr(eye));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(proj*view*model));

	modelGL = Mat4GL(1.0f)*Transformation::translation(-center[0], -center[1], -center[2]);
	glm::vec3 p = eye + lookDir;
	viewGl = Mat4GL::lookAt(Vec3(eye[0], eye[1], eye[2]), Vec3(p[0], p[1], p[2]), Vec3(up[0],up[1],up[2]));
	projGl = Mat4GL::perspective(30.0f, width/((float)height), zNear, zFar);
	viewPortGL = Mat4GL::viewPort(0, width, height, 0);
	
	glUniform3fv(uniEye, 1, glm::value_ptr(eye));

	if (closeGL){
		compute (mesh->getVertexPositions(),projGl, viewGl, modelGL, mesh->getNumTriangles(), mesh->getVertexNormal(), mesh->getVertexTexturePositions());
		
		float vertexData[]  = { -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 
								1.0f, -1.0f, -1.0f, -1.0f, -1.0f,1.0f};

		float textureCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // tex coords 1sttriangle
								   1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};  // tex coords 1sttriangle

		initBuffers();
		raster();
		unsigned char texdata[colorBuffer.size() * 4];
		for(unsigned int i=0; i< colorBuffer.size(); i++){			
        	texdata[i*4] = colorBuffer[i][0]*255;
        	texdata[i*4+1] = colorBuffer[i][1]*255;
        	texdata[i*4+2] = colorBuffer[i][2]*255;
        	texdata[i*4+3] = colorBuffer[i][3]*255;
    	}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
   		glUniform1i(glGetUniformLocation(program, "texture1"), 0);

		glGenVertexArrays(1, vao);
    	glBindVertexArray(vao[0]);

    	glGenBuffers(1, vbo);
    	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData) + sizeof(textureCoords), NULL, GL_STATIC_DRAW );
    	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData);
    	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexData), sizeof(textureCoords), textureCoords);


    	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertexData));
    	glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
    	/*glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    	glEnableVertexAttribArray(0);*/

    	glUniform1i(uniCloseGl, 1);
		//glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingNormal);

	}else{
		GLfloat vertexPositions[mesh->getNumVertex()*3];
		setVertex(vertexPositions, mesh->getVertexPositions(), mesh->getNumVertex()*3);
		GLfloat vertexNormal[mesh->getNumVertex()*3];
		setVertex(vertexNormal, mesh->getVertexNormal(), mesh->getNumVertex()*3);
		GLfloat vertexTexturePositions [mesh->getNumVertex()*2];
		setVertex(vertexTexturePositions, mesh->getVertexTexturePositions(), mesh->getNumVertex()*2);
		for (int i = 1; i < mesh->getNumVertex()*2; i+=2){
			vertexTexturePositions[i] = 1 - vertexTexturePositions[i];
		}



		glGenTextures(1, &textureID);

		glActiveTexture(GL_TEXTURE0);		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_, h_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		setTextureType();

		


		
		glBindTexture(GL_TEXTURE_2D, textureID);
   		glUniform1i(glGetUniformLocation(program, "texture1"), 0);


		glGenVertexArrays(1, vao);
    	glBindVertexArray(vao[0]);

    	glGenBuffers(1, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions) + sizeof(vertexTexturePositions) + sizeof(vertexNormal), NULL, GL_STATIC_DRAW ); //
    	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), vertexPositions);
    	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexTexturePositions), vertexTexturePositions);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions) + sizeof(vertexTexturePositions), sizeof(vertexNormal), vertexNormal);

    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertexPositions));
    	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(sizeof(vertexPositions) + sizeof(vertexTexturePositions)));
    	glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
    	glUniform1i(uniCloseGl, 0);    	
	}	
}

void display( void ){
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindVertexArray( vao[0] );

	if (closeGL){ glDrawArrays(type, 0, vertexGL.size()/2); } else {glDrawArrays(type, 0, mesh->getNumVertex());}
	glutSwapBuffers();

	frameCount++;
	finalTime = time(NULL);
	
	if (finalTime - initialTime > 0){
		fps = frameCount/(finalTime - initialTime);
		frameCount = 0;
		initialTime = finalTime;
		
		glutSetWindowTitle(("FPS: " + std::to_string(fps)).c_str());
	}
	glutPostRedisplay();
}


void init (std::string pathMesh){
	ShaderInfo  shaders[] = { { GL_VERTEX_SHADER, "shader/triangles.vert" },
        					  { GL_FRAGMENT_SHADER, "shader/triangles.frag" },
						      { GL_NONE, NULL } };
	
	program = LoadShaders(shaders);
	glUseProgram(program);

	uniColor = glGetUniformLocation(program, "color");
	
	uniAmbientColor = glGetUniformLocation(program, "aColor");
	uniDiffuseColor = glGetUniformLocation(program, "dColor");
	uniSpecularColor = glGetUniformLocation(program, "sColor");
	uniMaterialShine = glGetUniformLocation(program, "mShine");
	uniEye = glGetUniformLocation(program, "eye");
	
	mesh = new Mesh(pathMesh);	

	setVertex(ambientColor, mesh->getAmbientColor(), mesh->getnumberMaterials()*3);
	setVertex(diffuseColor, mesh->getDiffuseColor(), mesh->getnumberMaterials()*3);
	setVertex(specularColor, mesh->getSpecularColor(), mesh->getnumberMaterials()*3);
	materialShine = mesh->getMaterialShine()[0];

	GLfloat faceNormal[mesh->getNumVertex()];
	setVertex(faceNormal, mesh->getFaceNormal(), mesh->getNumVertex());

	center = mesh->box.getCenter();
	float *max = mesh->box.getMax();
	float *min = mesh->box.getMin();
	r = std::sqrt(std::pow((max[0]-min[0]), 2) + std::pow((max[1]-min[1]), 2)+std::pow((max[2]-min[2]), 2));
	fDistance =  r/tan( 30 * PI / 180.0f );
	zNear = fDistance - r;
	zFar = fDistance + r;
	
	eye = glm::vec3(0.0f, 0.0f, fDistance);
	lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0,1,0);
	right = glm::vec3(1.0f ,0.0f, 0.0f);

	uniModel = glGetUniformLocation(program, "model");
	uniView = glGetUniformLocation(program, "view");
	uniMVP = glGetUniformLocation(program, "mvp");
	uniCloseGl = glGetUniformLocation(program, "closeGL");
	updateMVP();
		
	glUniform3fv(uniColor, 1, color);
	glUniform3fv(uniAmbientColor, 1, ambientColor);
	glUniform3fv(uniDiffuseColor, 1, diffuseColor);
	glUniform3fv(uniSpecularColor, 1, specularColor);
	glUniform1fv(uniMaterialShine, 1, &materialShine);
	

	shadingNormal = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "normal");
	shadingGoroud = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "goroud");
	shadingGoroudFull = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "goroudFull");
	shadingPhong = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "phong");
	shadingPhongF = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "phong");
	shadingNormalF = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "normal");

   	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingNormal);
   	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &shadingNormalF);	
}

#include "include/keyboard.h"

int main( int argc, char** argv ){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitContextVersion(4, 6);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);
	glutSetWindowTitle("FPS: ");
	
	glewExperimental = GL_TRUE; 

	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	 
	glFrontFace(typeFrontFace);
	init(argv[1]);
	glutDisplayFunc(display);	
	glutKeyboardFunc(inputKeyboard);
	glutMainLoop();
}