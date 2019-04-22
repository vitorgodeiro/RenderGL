#include "include/vgl.h"
#include "include/transformation.h"
#include "src/loadShaders.cpp"
#include "src/camera.cpp"
#include "src/mesh.cpp"
#include "include/mat4.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#define PI 3.14159265

int initialTime = time(NULL), finalTime, frameCount = 0, fps;

void setVertex(float vertex[], float * vertex_, int numVertex){
	for (int i =0; i < numVertex; i++){
		vertex[i] = vertex_[i];
	}	
}

#define THETA 0.0174533f
#define COSTHETA float(cos(THETA))
#define SINTHETA float(sin(THETA))

GLuint vao[1];
GLuint vbo[1];
Mesh *mesh;
int width = 800;
int height = 600;

float zNear, zFar;
float r;
float fDistance;
float *center = new float[3];


float step = 1.0f;

GLint uniModel;
GLint uniView;
GLint uniMVP;
GLint uniColor;
GLint uniEye;
GLfloat color[] = {1.0f, 1.0f, 1.0f};

glm::mat4 model, view, proj;
int type = GL_TRIANGLES;
int typePolMode = GL_FILL;
int typeFrontFace = GL_CCW;

//camera 
glm::vec3 eye;
glm::vec3 lookDir;
glm::vec3 up ;
glm::vec3 right;

void display( void ){
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindVertexArray( vao[0] );

	glDrawArrays(type, 0, mesh->getNumVertex());
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

void updateMVP(void){
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-center[0], -center[1], -center[2])); 
	proj = glm::perspective((float)( 30.0f * PI / 180.0f ), 1.3333f,zNear, zFar);
	view = glm::lookAt( eye, eye + lookDir, up);
	glUniform3fv(uniEye, 1, glm::value_ptr(eye));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(proj*view*model));
}

void init (std::string pathMesh){
	ShaderInfo  shaders[] = { { GL_VERTEX_SHADER, "shader/triangles.vert" },
        					  { GL_FRAGMENT_SHADER, "shader/triangles.frag" },
						      { GL_NONE, NULL } };
	
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	mesh = new Mesh(pathMesh);
	
	GLfloat vertexPositions[mesh->getNumVertex()*3];
	setVertex(vertexPositions, mesh->getVertexPositions(), mesh->getNumVertex()*3);
	GLfloat ambientColor[mesh->getnumberMaterials()*3];
	setVertex(ambientColor, mesh->getAmbientColor(), mesh->getnumberMaterials()*3);
	GLfloat diffuseColor[mesh->getnumberMaterials()*3];
	setVertex(diffuseColor, mesh->getDiffuseColor(), mesh->getnumberMaterials()*3);
	GLfloat specularColor[mesh->getnumberMaterials()*3];
	setVertex(specularColor, mesh->getSpecularColor(), mesh->getnumberMaterials()*3);
	GLfloat materialShine[mesh->getnumberMaterials()*3];
	setVertex(materialShine, mesh->getMaterialShine(), mesh->getnumberMaterials());

	GLfloat vertexNormal[mesh->getNumVertex()*3];
	setVertex(vertexNormal, mesh->getVertexNormal(), mesh->getNumVertex()*3);
	GLfloat faceNormal[mesh->getNumVertex()];
	setVertex(faceNormal, mesh->getFaceNormal(), mesh->getNumVertex());


   /* GLfloat vertex_colors[] = { 1.00f, 0.00f, 0.00f,  
						  		0.00f, 1.00f, 0.00f, 
						  		0.00f, 0.00f, 1.00f };*/


	
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
	updateMVP();

	uniColor = glGetUniformLocation(program, "color");
	glUniform3fv(uniColor, 1, color);

	GLint uniAmbientColor = glGetUniformLocation(program, "aColor");
	glUniform3fv(uniAmbientColor, 1, ambientColor);

	GLint uniDiffuseColor = glGetUniformLocation(program, "dColor");
	glUniform3fv(uniDiffuseColor, 1, diffuseColor);

	GLint uniSpecularColor = glGetUniformLocation(program, "sColor");
	glUniform3fv(uniSpecularColor, 1, specularColor);

	GLint uniMaterialShine = glGetUniformLocation(program, "mShine");
	glUniform1fv(uniMaterialShine, 1, materialShine);

	uniEye = glGetUniformLocation(program, "eye");
	glUniform3fv(uniEye, 1, glm::value_ptr(eye));
	//std::cout << "Size = " << sizeof(vertexPositions) << std::endl;
	//std::cout << vertexNormal[0] << " " << vertexNormal[1] << " " << vertexNormal[2] << std::endl;



}

void inputKeyboard(unsigned char key, int _x, int _y){
	std::system("clear");
	std::cin.clear();
	fflush(stdin);
	switch (key){
		case 'a' :
		case 'A' :
			if (typeFrontFace == GL_CCW){
				typeFrontFace = GL_CW;
				glFrontFace(typeFrontFace);
				std::cout << "Activate Clock-Wise" << std::endl;
			}
			else{
				typeFrontFace = GL_CCW;
				glFrontFace(typeFrontFace);
				std::cout << "Activate Counter Clock-Wise" << std::endl;
			}
		break;
	glFrontFace(typeFrontFace);
		case 'c' :
		case 'C' : {
			std::cout << "The actual colors in RGB is:" << std::endl;
			std::cout << "R = " << color[0] << " G = " << color[1] << " B = " << color[2] << std::endl;
			std::cout << "Enter the new value of R:" << std::endl;
			std::cin >> color[0];
			std::cout << "Enter the new values of G:" << std::endl;
			std::cin >> color[1];
			std::cout << "Enter the new values of B" << std::endl;
			std::cin >> color[2];
			std::cout << "The actual colors in RGB is:" << std::endl;
			std::cout << "R = " << color[0] << " G = " << color[1] << " B = " << color[2] << std::endl;
			glUniform3fv(uniColor, 1, color);
			break;}
		case 'e' :
		case 'E' :
			glEnable(GL_CULL_FACE);  
			glCullFace(GL_BACK); 
			std::cout << "Enable Back-Face Culling" << std::endl;
			break;
		case 'd' :
		case 'D' :
			glDisable(GL_CULL_FACE);  
			std::cout << "Disable Back-Face Culling" << std::endl;
			break;
		case 'f' :
		case 'F' : {
			std::cout << "The actual valor of Far Clipping Planes: " <<  zFar << std::endl;
			std::cout << "Enter the new value of Far Clipping Planes:" << std::endl;
			std::cin >> zFar;
			std::cout << "The actual valor of Far Clipping Planes: " <<  zFar << std::endl;			
			updateMVP();
			break;}
		case 'n' :
		case 'N' : {
			std::cout << "The actual valor of Near Clipping Planes: " <<  zNear << std::endl;
			std::cout << "Enter the new value of Near Clipping Planes:" << std::endl;
			std::cin >> zNear;
			std::cout << "The actual valor of Near Clipping Planes: " <<  zNear << std::endl;			
			updateMVP();
			break;}
		case 'p' :
		case 'P' : 
			type = GL_POINTS;
			break;
		case 'q' :
		case 'Q' : 
			exit(0);
			break;
		case 'r' :
		case 'R' : {
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
			updateMVP();
			break;}	
				
		case 't' :
		case 'T' : 
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			type = GL_TRIANGLES;
			break;
		case 'w' :
		case 'W' : 
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			type = GL_TRIANGLES;
			break;
/////// Roll (z)
		case 'u' :
		case 'U' : {
			glm::vec3 u = right, v = up;
			right = u*COSTHETA + v*SINTHETA;
			up = -u*SINTHETA + v*COSTHETA;
			updateMVP();
			break;}
		case 'j' :
		case 'J' : {
			glm::vec3 u = right, v = up;
			right = u*COSTHETA + -v*SINTHETA;
			up = u*SINTHETA + v*COSTHETA;
			updateMVP();
			break;}
/////////// Yaw (y)
		case 'h' :
		case 'H' : {
			glm::vec3 u = right, n = lookDir;
      		right = u*COSTHETA + n*SINTHETA;
			lookDir = -u*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
		case 'k' :
		case 'K' : {
			glm::vec3 u = right, n = lookDir;
      		right = u*COSTHETA + -n*SINTHETA;
			lookDir = u*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
/////////// Pitch (x)
		case 'l' :
		case 'L' :{
			glm::vec3 v = up, n = lookDir;
      		up = v*COSTHETA + n*SINTHETA;
			lookDir = -v*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
		case 'o' :
		case 'O' :{
			glm::vec3 v = up, n = lookDir;
      		up = v*COSTHETA + -n*SINTHETA;
			lookDir = v*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
//
		case '1' :
			std:: cout << "Translate Pitch Center : " << -step << std::endl;	
			eye += lookDir*step;
			updateMVP();
			break;	
		case '2' :
			std:: cout << "Translate Yaw Center : " << -step << std::endl;	
			eye -= up*step;
			lookDir = glm::normalize(-eye);
			up = glm::normalize(glm::cross(right, lookDir));
			updateMVP();
			break;	
		case '3' :
			std:: cout << "Translate Pitch Center : " << step << std::endl;	
			eye -= lookDir*step;
			updateMVP();
			break;	
		case '4' :{
			std:: cout << "Translate Roll Center : " << -step << std::endl;
			eye -= right*step;
			lookDir = glm::normalize(-eye);
			right = glm::normalize(glm::cross(lookDir, up));
			updateMVP();
			break;}
		case '6' :
			std:: cout << "Translate Roll Center : " << step << std::endl;
			eye += right*step;
			lookDir = glm::normalize(-eye);
			right = glm::normalize(glm::cross(lookDir, up));
			updateMVP();
			break;
		case '8' :
			std:: cout << "Translate Yaw Center : " << step << std::endl;	
			eye += up*step;
			lookDir = glm::normalize(-eye);
			up = glm::normalize(glm::cross(right, lookDir));
			updateMVP();
			break;
		case ';' :
			std:: cout << "Translate Yaw : " << -step << std::endl;	
			eye -= up*step;
			updateMVP();
			break;	
		case ',' :{
			std:: cout << "Translate Roll : " << -step << std::endl;
			eye -= right*step;
			updateMVP();
			break;}
		case '.' :
			std:: cout << "Translate Roll : " << step << std::endl;
			eye += right*step;
			updateMVP();
			break;
		case '/' :
			std:: cout << "Translate Yaw : " << step << std::endl;	
			eye += up*step;
			updateMVP();
			break;
			
	}

	glutPostRedisplay();
}

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

