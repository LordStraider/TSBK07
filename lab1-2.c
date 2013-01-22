/* Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.*/
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	/* A little linking trick for Lightweight IDE*/
	#if 0
		#import <Cocoa/Cocoa.h>
	#endif
#endif
#include "GL_utilities.h"
#include <math.h>

/* Globals
// Data would normally be read from files*/
GLfloat vertices[] = {	 1.0f,  1.0f, 1.0f,
						-1.0f, -1.0f, 1.0f,
						-0.5f, -1.0f, 1.0f };

GLfloat myMatrix[] = {  1.0f, 0.0f, 0.0f, 0.5f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat rotationMatrix[] = {	0.7f, -0.7f, 0.0f, 0.0f,
								0.7f, 0.7f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };
/* vertex array object*/
unsigned int vertexArrayObjID;

void set_sincos(GLfloat* m, double alpha) { 
	m[0] = cos(alpha);
	m[1] = -sin(alpha);
	m[4] = sin(alpha);
	m[5] = cos(alpha);
}

void init(void)
{
	/* two vertex buffer objects, used for uploading the*/
	unsigned int vertexBufferObjID;
/*	unsigned int colorBufferObjID;*/
	/* Reference to shader program*/
	GLuint program;

	dumpInfo();

	/* GL inits*/
	glClearColor(0.5,0.5,0.1,0.3);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	/* Load and compile shader*/
	program = loadShaders("lab1-1.vert", "lab1-1.frag");
	printError("init shader");
	
	/* Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object*/
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	/* Allocate Vertex Buffer Objects*/
	glGenBuffers(1, &vertexBufferObjID);
	
	/* VBO for vertex data*/
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
    set_sincos(&rotationMatrix, 0.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotationMatrix"), 1, GL_TRUE, rotationMatrix);
	/* End of upload of geometry*/
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	/* Select VAO*/
	glDrawArrays(GL_TRIANGLES, 0, 3);	/* draw object*/
	
	printError("display");
	
	glFlush();
}

int main(int argc, const char *argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
	return 0;
}
