#include "include/vgl.h"
#include "src/LoadShaders.cpp"
#include "src/Mesh.cpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#define PI 3.14159265


void setVertexPositions(float vertex[], float * vertex_, int numVertex){
	for (int i =0; i < numVertex*3; i++){
		vertex[i] = vertex_[i];
	}	
}


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

GLint uniMVP;
glm::mat4 mvp;
GLint uniColor;
GLfloat color[] = {1.0f, 1.0f, 1.0f, 1.0f};

glm::mat4 model, view, proj;
int type = GL_TRIANGLES;
int typePolMode = GL_FILL;
int typeFrontFace = GL_CCW;

//camera 
glm::vec3 eye;
glm::vec3 lookDir;
glm::vec3 up ;

void display( void ){
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindVertexArray( vao[0] );
	 ///wireframe
	// disableWireframe
	glDrawArrays(type, 0, mesh->getNumVertex());
	glutSwapBuffers();

}



void updateMVP(void){
	std::cout << glm::to_string(eye) << std::endl;
	std::cout << glm::to_string(lookDir) << std::endl;
	std::cout << glm::to_string(eye+lookDir) << std::endl;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-center[0], -center[1], -center[2])); 
	proj = glm::perspective((float)( 30.0f * PI / 180.0f ), 1.3333f,zNear, zFar);//glm::frustum(-r, r, -r, r, zNear, zFar);
	view = glm::lookAt( eye, eye + lookDir, up);
	mvp = proj*view*model;
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(mvp));
}

void init (std::string pathMesh){
	ShaderInfo  shaders[] = { { GL_VERTEX_SHADER, "shader/triangles.vert" },
        					  { GL_FRAGMENT_SHADER, "shader/triangles.frag" },
						      { GL_NONE, NULL } };
	
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	mesh = new Mesh(pathMesh);

	GLfloat vertex_positions[mesh->getNumVertex()*3];
	setVertexPositions(vertex_positions, mesh->getVertexPositions(), mesh->getNumVertex());
    GLfloat vertex_colors[] = { 1.00f, 0.00f, 0.00f,  
						  		0.00f, 1.00f, 0.00f, 
						  		0.00f, 0.00f, 1.00f };
	
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_positions));
    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	

	center = mesh->box.getCenter();
	float *max = mesh->box.getMax();
	float *min = mesh->box.getMin();
	r = std::max(std::max((max[0]-min[0]), (max[1]-min[1])), (max[2]-min[2]));
	//r = sqrt((pow((max[0]-center[0]), 2) + pow((max[1]-center[1]), 2) + pow((max[2]-center[2]), 2)));
	fDistance =  r/tan( 30 * PI / 180.0f );

	zNear = fDistance - r;
	zFar = fDistance + r;
	
	eye = glm::vec3(0.0f, 0.0f, fDistance);
	lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0,1,0);

	uniMVP = glGetUniformLocation(program, "mvp");
	updateMVP();
	uniColor = glGetUniformLocation(program, "color");
	glUniform4fv(uniColor, 1, color);
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
			glUniform4fv(uniColor, 1, color);
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
			proj = glm::frustum(-r, r, -r, r, zNear, zFar);
			updateMVP();
			break;}
		case 'n' :
		case 'N' : {
			std::cout << "The actual valor of Near Clipping Planes: " <<  zNear << std::endl;
			std::cout << "Enter the new value of Near Clipping Planes:" << std::endl;
			std::cin >> zNear;
			std::cout << "The actual valor of Near Clipping Planes: " <<  zNear << std::endl;			
			proj = glm::frustum(-r, r, -r, r, zNear, zFar);
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
 			r = std::max(std::max(center[0], center[1]), center[2]);
			fDistance =  r/tan( 30 * PI / 180.0f );
			zNear = fDistance - r;
			zFar = fDistance + r;
			eye = glm::vec3(0.0f, 0.0f, fDistance);
			lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
			up = glm::vec3(0,1,0);	
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
		case '1' :
			std:: cout << "Translate Pitch : " << -step << std::endl;	
			eye -= glm::vec3(0.0f, 0.0f, step);
			lookDir -= glm::vec3(0.0f, 0.0f, step);
			updateMVP();
			break;	
		case '2' :
			std:: cout << "Translate Yaw : " << -step << std::endl;	
			eye -= glm::vec3(0.0f, step, 0.0f);
			updateMVP();
			break;	
		case '3' :
			std:: cout << "Translate Pitch : " << step << std::endl;	
			eye += glm::vec3(0.0f, 0.0f, step);
			updateMVP();
			break;	
		case '4' :{
			std:: cout << "Translate Roll : " << -step << std::endl;
			eye -= glm::vec3(step, 0.0f, 0.0f);
			updateMVP();
			break;}
		case '6' :
			std:: cout << "Translate Roll : " << step << std::endl;
			eye += glm::vec3(step, 0.0f, 0.0f);
			updateMVP();
			break;
		case '8' :
			std:: cout << "Translate Yaw : " << step << std::endl;	
			eye += glm::vec3(0.0f, step, 0.0f);
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
	glutSetWindowTitle("Exe_01");
	
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

