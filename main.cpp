#include "include/vgl.h"
#include "src/LoadShaders.cpp"
#include "src/Model.cpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

GLuint vao[1];
GLuint vbo[1];


void display( void ){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindVertexArray( vao[0] );
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glFlush();
}

void init (void){
	ShaderInfo  shaders[] = { { GL_VERTEX_SHADER, "shader/triangles.vert" },
        					  { GL_FRAGMENT_SHADER, "shader/triangles.frag" },
						      { GL_NONE, NULL } };
	
	GLuint program = LoadShaders( shaders );
	glUseProgram( program );

	Model model ("model/cube.in");

    
	

	
	GLfloat *vertex_positions = model.getVertexPositions();
    GLfloat vertex_colors[] = { 1.00f, 0.00f, 0.00f,  
						  		0.00f, 1.00f, 0.00f, 
						  		0.00f, 0.00f, 1.00f };
	int sizeVertex = sizeof(float)*model.getNumVertex();
	model.toString();
	std::cout << model.getNumVertex() << std::endl;
	
	
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );

    glGenBuffers( 1, vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

    glBufferData( GL_ARRAY_BUFFER, sizeVertex + sizeof(vertex_colors), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeVertex, &vertex_positions );
    glBufferSubData( GL_ARRAY_BUFFER, sizeVertex, sizeof(vertex_colors), vertex_colors );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeVertex);
    glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	glm::mat4 view = glm::lookAt( glm::vec3(0, 0, 0), // Camera is at (4,3,3), in World Space
								  glm::vec3(0,2,-3), // and looks at the origin
    							  glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    						);
	
	GLint uniView = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	
}

int main( int argc, char** argv ){
	std::cout << "EXE\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(800, 600);
	glutInitContextVersion(4, 6);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);
	glutSetWindowTitle("Exe_01");
	
	glewExperimental = GL_TRUE; 
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutMainLoop();
}

