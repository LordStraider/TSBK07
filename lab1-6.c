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
#include "loadobj.h"

/* Globals*/
Model *m;
m = LoadModel("bunny.obj");

GLfloat myRotationMatrix[] = {  1.0f, 0.0f, 0.0f, 0.0f,
		                        0.0f, 1.0f, 0.0f, 0.0f,
		                        0.0f, 0.0f, 1.0f, 0.0f,
		                        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myRotationMatrix2[] = {  1.0f, 0.0f, 0.0f, 0.0f,
		                        0.0f, 1.0f, 0.0f, 0.0f,
		                        0.0f, 0.0f, 1.0f, 0.0f,
		                        0.0f, 0.0f, 0.0f, 1.0f };

GLfloat myTranslationMatrix[] = {	1.0f, 0.0f, 0.0f, 0.0f,
				                    0.0f, 1.0f, 0.0f, 0.0f,
				                    0.0f, 0.0f, 1.0f, -2.0f,
				                    0.0f, 0.0f, 0.0f, 1.0f };

/* vertex array object*/
unsigned int vertexArrayObjID;

GLuint program;

#define near 1.0
#define far 30.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat myProjMatrix[] = {	2.0f*near/(right-left), 0.0f, 0.0f, 0.0f,
				            0.0f, 2.0f*near/(top-bottom), 0.0f, 0.0f,
				            0.0f, 0.0f, -(far+near)/(far-near), (-2*far*near)/(far-near),
				            0.0f, 0.0f, -1.0f, 0.0f };

void set_sincos(GLfloat* m, float alpha) { 
	m[0] = cos(alpha);
	m[1] = -sin(alpha);
	m[4] = sin(alpha);
	m[5] = cos(alpha);
}

void set_sincos2(GLfloat* m, float alpha) { 
	m[5] = cos(alpha);
	m[6] = sin(alpha);
	m[9] = -sin(alpha);
	m[10] = cos(alpha);
}

void init(void) {
	/* two vertex buffer objects, used for uploading the*/
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	/* Reference to shader program*/

	dumpInfo();

	/* GL inits*/
	glClearColor(0.9,0.9,0.9,1.0);
    glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	/* Load and compile shader*/
	program = loadShaders("lab1-1.vert", "lab1-1.frag");
	printError("init shader");
	/* Upload geometry to the GPU:*/
	
	/* Allocate and activate Vertex Array Object*/
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	/* Allocate Vertex Buffer Objects*/
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &colorBufferObjID);
	
	/* VBO for vertex data*/
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	/* VBO for colour data*/
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));


	glUniformMatrix4fv(glGetUniformLocation(program, "myTranslationMatrix"), 1, GL_TRUE, myTranslationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myProjMatrix"), 1, GL_TRUE, myProjMatrix);
 	/* End of upload of geometry*/
	
	printError("init arrays");
}

void display(void) {
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	set_sincos(&myRotationMatrix, t/900);
	set_sincos2(&myRotationMatrix2, t/1300);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotationMatrix"), 1, GL_TRUE, myRotationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotationMatrix2"), 1, GL_TRUE, myRotationMatrix2);

	glBindVertexArray(vertexArrayObjID);	/* Select VAO*/
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	/* draw object*/
	
	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutCreateWindow ("GL3 white triangle example");
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutDisplayFunc(display); 
	glutTimerFunc(20, &OnTimer, 0);
	init ();
	glutMainLoop();
	return 0;
}
