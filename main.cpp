#include "include/vgl.h"
#include "include/transformation.h"
#include "src/loadShaders.cpp"
#include "src/camera.cpp"
#include "src/mesh.cpp"
#include "include/mat4.h"
#include "include/vec4.h"
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

bool closeGL = true;
bool backFaceGL = true;
bool ccw = false;

int width = 800;
int height = 600;

int initialTime = time(NULL), finalTime, frameCount = 0, fps;
int type = GL_TRIANGLES;
int typePolMode = GL_FILL;
int typeFrontFace = GL_CCW;

float step = 100.0f;
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

void compute(float vertex[], Mat4GL mvp, int numtriangles){
	float v1[4], v2[4], v3[4];
	vertexGL.clear();
	vertexGL2.clear();
	float a;
	for (int i =0; i < numtriangles; i++){
		v1[0] = vertex[i*9];
		v1[1] = vertex[i*9 + 1];
		v1[2] = vertex[i*9 + 2];
		v1[3] = 1;

		v2[0] = vertex[i*9 + 3];
		v2[1] = vertex[i*9 + 4];
		v2[2] = vertex[i*9 + 5];
		v2[3] = 1;

		v3[0] = vertex[i*9 + 6];
		v3[1] = vertex[i*9 + 7];
		v3[2] = vertex[i*9 + 8];
		v3[3] = 1;
		
		mat4Vec(v1, mvp);
		mat4Vec(v2, mvp);
		mat4Vec(v3, mvp);

		//Verificando se está no NDC e se W > 0
		if (v1[3] > 0 && v2[3] > 0 && v3[3] > 0 && (v1[2]/v1[3]) <= 1.0f && -1 <= (v1[2]/v1[3]) && (v1[2]/v1[3]) <= 1.0f && -1.0f <= (v2[2]/v2[3]) && (v2[2]/v2[3]) <= 1.0f && -1.0f <= (v3[2]/v3[3]) && (v3[2]/v3[3]) <= 1.0f){

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
				
				vertexGL2.push_back(v1[0]); vertexGL2.push_back(v1[1]); vertexGL2.push_back(v1[2]); vertexGL2.push_back(v1[3]);
				vertexGL2.push_back(v2[0]); vertexGL2.push_back(v2[1]); vertexGL2.push_back(v2[2]); vertexGL2.push_back(v2[3]);
				vertexGL2.push_back(v3[0]); vertexGL2.push_back(v3[1]); vertexGL2.push_back(v3[2]); vertexGL2.push_back(v3[3]);
			}
		}	
	}	
}

//Bresenham’s Line Drawing Algorithm
/*void line(float x0, float y0, float z0,  float x1, float y1, float z1) { 
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
   	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
   	int dz = abs(z1-z0), sz = z0<z1 ? 1 : -1; 
   	int dm = std::max(std::max(abs(dx-dy), abs(dx-dz)), abs(dy-dz)), i = dm; 
   	x1 = y1 = z1 = dm/2; 

   	while(0 < i--) { 
   		std::cout << z0 << std::endl;
   		setColorBuffer(x0, y0, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
      	x1 -= dx; if (x1 < 0) { x1 += dm; x0 += sx; } 
      	y1 -= dy; if (y1 < 0) { y1 += dm; y0 += sy; } 
      	z1 -= dz; if (z1 < 0) { z1 += dm; z0 += sz; } 

   }	
}*/

std::vector<float> listV0;
std::vector<float> listV1;

//Bresenham’s Line Drawing Algorithm
void line(int x0, int y0, int z0, int x1, int y1, int z1, int a) { 
    bool steep = false; 
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) { std::swap(x0, y0); std::swap(x1, y1); steep = true; }
    if (x0 > x1) { std::swap(x0, x1); std::swap(y0, y1);} 
    float dx = x1-x0; float dy = y1-y0; float dz = z1-z0; 

    float derror = std::abs(dy/float(dx)); 
    float error = 0; 
    int y = y0; 
    int derrorZ = std::abs(dz/float(dx)); 
    int errorZ = 0; 
    int z = z0; 

   
    for (int x=x0; x<=x1; x++) { 
    	if (steep) { 
    		if (getZBuffer(y, x) > z){
    			setColorBuffer(y, x, Vec4(1.0f, 1.0f, 1.0f, 1.0f)); 
    			setZBuffer(y, x, z);
    		}
    	} 
    	else { 
    		if (getZBuffer(x, y) > z){
    			setColorBuffer(x, y, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    			setZBuffer(x, y, z);
    		} 
    	}
    	if ( a == 0 ){
    		if (steep){
    			listV0.push_back(y);
    			listV0.push_back(x);
    			listV0.push_back(z);
    		}else {
        		listV0.push_back(x);
        		listV0.push_back(y);
        		listV0.push_back(z);
        	}

        }else if (a == 1){
        	if (steep){
        		listV1.push_back(y);
    			listV1.push_back(x);
    			listV1.push_back(z);
    		}else{
        		listV1.push_back(x);
        		listV1.push_back(y);
        		listV1.push_back(z);
        	}
        }  


        error += derror; 
        errorZ += derrorZ; 
        
        if (error > 0.5) { 
        	y += (y1>y0?1:-1); 
        	error -= 1.0f;       	
        }
        if (errorZ > 0.5) { 
        	z += (z1>z0?1:-1); 
        	errorZ -= 1.0f; 
        }       
    } 	
}


/*void fillTriangle(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2){
	int dx0 = abs(x1-x0), sx0 = x0<x1 ? 1 : -1;
   	int dy0 = abs(y1-y0), sy0 = y0<y1 ? 1 : -1; 
   	int dz0 = abs(z1-z0), sz0 = z0<z1 ? 1 : -1; 
   	int dm0 = std::max(std::max(abs(dx0-dy0), abs(dx0-dz0)), abs(dy0-dz0)), i = dm0; 
   	x1 = y1 = z1 = dm/2; 

   	while(0 < i--) { 
   		std::cout << z0 << std::endl;
   		setColorBuffer(x0, y0, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
      	x1 -= dx; if (x1 < 0) { x1 += dm; x0 += sx; } 
      	y1 -= dy; if (y1 < 0) { y1 += dm; y0 += sy; } 
      	z1 -= dz; if (z1 < 0) { z1 += dm; z0 += sz; } 

   }	
}*/



void raster(){
	int u1, u2, u3, v1, v2, v3, n1, n2, n3;
	for (unsigned int i = 0; i < vertexGL2.size(); i+=12){//vertexGL2.size()
		u1 = (int)round(vertexGL2[i]);
		v1 = (int)round(vertexGL2[i + 1]);
		n1 = (int)round(vertexGL2[i + 2]);

		u2 = (int)round(vertexGL2[i + 4]);
		v2 = (int)round(vertexGL2[i + 5]);
		n2 = (int)round(vertexGL2[i + 6]);

		u3 = (int)round(vertexGL2[i + 8]);
		v3 = (int)round(vertexGL2[i + 9]);
		n3 = (int)round(vertexGL2[i + 10]);

		listV0.clear();
		listV1.clear();
		line(u1, v1, n1, u2, v2, n2, 0);
		line(u1, v1, n1, u3, v3, n3, 1);
		//line(u2, v2, n2, u3, v3, n3, 3);
		
		for (unsigned int i = 0; i < listV0.size(); i+=3){
			for (unsigned int j = 0; j < listV1.size(); j+=3){
				line(listV0[i], listV0[i + 1], listV0[i + 2], listV1[j], listV1[j + 1], listV1[j+2], 3);
			}
		}
		
		//line(u3, v3, n3, u1, v1, n1);
	}
}

void updateMVP(void){

	model = glm::mat4(1.0f);
	
	model = glm::translate(model, glm::vec3(-center[0], -center[1], -center[2])); 
	view = glm::lookAt( eye, eye + lookDir, up);
	proj = glm::perspective((float)( 30.0f * PI / 180.0f ), 1.3333f,zNear, zFar);
	glUniform3fv(uniEye, 1, glm::value_ptr(eye));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(proj*view*model));

	
	modelGL = Mat4GL(1.0f)*Transformation::translation(-center[0], -center[1], -center[2]);
	glm::vec3 p = eye + lookDir;
	viewGl = Mat4GL::lookAt(Vec3(eye[0], eye[1], eye[2]), Vec3(p[0], p[1], p[2]), Vec3(up[0],up[1],up[2]));
	projGl = Mat4GL::perspective(30.0f, 1.3333f, zNear, zFar);
	viewPortGL = Mat4GL::viewPort(0, width, height, 0);
	
	glUniform3fv(uniEye, 1, glm::value_ptr(eye));

	if (closeGL){
		compute (mesh->getVertexPositions(),projGl*viewGl*modelGL, mesh->getNumTriangles());
		GLfloat vertexPositions[vertexGL.size()];
		for (unsigned int i = 0; i < vertexGL.size(); i++){	vertexPositions[i] = vertexGL[i]; }

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

    	//glUniform1i(uniCloseGl, 1);
		//glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingNormal);

	}else{
		GLfloat vertexPositions[mesh->getNumVertex()*3];
		setVertex(vertexPositions, mesh->getVertexPositions(), mesh->getNumVertex()*3);
		GLfloat vertexNormal[mesh->getNumVertex()*3];
		setVertex(vertexNormal, mesh->getVertexNormal(), mesh->getNumVertex()*3);

		glGenVertexArrays(1, vao);
    	glBindVertexArray(vao[0]);

    	glGenBuffers(1, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions) + sizeof(vertexNormal), NULL, GL_STATIC_DRAW );
    	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), vertexPositions);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexNormal), vertexNormal);

    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertexPositions));
    	glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
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

	GLfloat ambientColor[mesh->getnumberMaterials()*3];
	setVertex(ambientColor, mesh->getAmbientColor(), mesh->getnumberMaterials()*3);
	GLfloat diffuseColor[mesh->getnumberMaterials()*3];
	setVertex(diffuseColor, mesh->getDiffuseColor(), mesh->getnumberMaterials()*3);
	GLfloat specularColor[mesh->getnumberMaterials()*3];
	setVertex(specularColor, mesh->getSpecularColor(), mesh->getnumberMaterials()*3);
	GLfloat materialShine[mesh->getnumberMaterials()*3];
	setVertex(materialShine, mesh->getMaterialShine(), mesh->getnumberMaterials());

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
	glUniform1fv(uniMaterialShine, 1, materialShine);
	

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