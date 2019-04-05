#include "include/vgl.h"
#include "src/LoadShaders.cpp"
#include "src/Model.cpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#define PI 3.14159265

GLuint vao[1];
GLuint vbo[1];
Model *mesh;
int width = 800;
int height = 600;

void display( void ){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray( vao[0] );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); wireframe
	// disableWireframe
	glDrawArrays(GL_POINTS, 0, mesh->getNumVertex()); //GL_POINTS GL_TRIANGLES

	glFlush();
}

void init (std::string pathModel){
	ShaderInfo  shaders[] = { { GL_VERTEX_SHADER, "shader/triangles.vert" },
        					  { GL_FRAGMENT_SHADER, "shader/triangles.frag" },
						      { GL_NONE, NULL } };
	
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	mesh = new Model(pathModel);

	GLfloat vertex_positions[mesh->getNumVertex()*3];
	mesh->setVertexPositions(vertex_positions);
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
	glm::mat4 model_, view_, proj_;

	float *center = mesh->box.getCenter();
	float *max = mesh->box.getMax();
	
	float r = sqrt((pow((max[0]-center[0]), 2) + pow((max[1]-center[1]), 2) + pow((max[2]-center[2]), 2)));
	float fDistance =  r/tan( 30 * PI / 180.0f );

	float zNear = fDistance ;
	float zFar = fDistance + r;

	model_ = glm::mat4(1.0f);
	model_ = glm::translate(model_, glm::vec3(-center[0], -center[1], -center[2])); 
	view_ = glm::lookAt( glm::vec3(0.0f, 0.0f, fDistance), glm::vec3(0.0f ,0.0f, 0.0f), glm::vec3(0,1,0));
	proj_ = glm::frustum(-r, r, -r, r, zNear, zFar);
	glm::mat4 mvp = proj_*view_*model_;
	GLint uniMVP = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(mvp));
}

int main( int argc, char** argv ){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH );
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
	
	init(argv[1]);
	glutDisplayFunc(display);
	glutMainLoop();
}

