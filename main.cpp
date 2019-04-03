#include "include/vgl.h"
#include "src/LoadShaders.cpp"
#include "src/Model.cpp"

GLuint vao[1];
GLuint vbo[1];

void display( void ){
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray( vao[0] );
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();
}

void init (void){
	ShaderInfo  shaders[] = { { GL_VERTEX_SHADER, "shader/triangles.vert" },
        					  { GL_FRAGMENT_SHADER, "shader/triangles.frag" },
						      { GL_NONE, NULL } };
	
	GLuint program = LoadShaders( shaders );
    glUseProgram( program );
	Model model ("model/cube.in");
/*    GLfloat vertex_positions[] = { -0.90f, -0.90f,   
							 		0.00f,  0.90f,  
							 		0.90f, -0.90f };*/

	GLfloat *vertex_positions = model.getVertexPositions();
    GLfloat vertex_colors[] = { 1.00f, 0.00f, 0.00f,  
						  		0.00f, 1.00f, 0.00f, 
						  		0.00f, 0.00f, 1.00f };
	int sizeVertex = sizeof(float)*model.getNumVertex();

    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );

    glGenBuffers( 1, vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );

    glBufferData( GL_ARRAY_BUFFER, sizeVertex + sizeof(vertex_colors), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeVertex, vertex_positions );
    glBufferSubData( GL_ARRAY_BUFFER, sizeVertex, sizeof(vertex_colors), vertex_colors );

    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeVertex);
    glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
}

int main( int argc, char** argv ){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
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

