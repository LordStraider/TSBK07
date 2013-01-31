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
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "LoadTGA.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils2.h"

/* Globals*/
#define PI 3.14159

GLfloat rot[16], trans[16], total[16], cam[16];


#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projMatrix[] = {        2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
		                        0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
		                        0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
		                        0.0f, 0.0f, -1.0f, 0.0f };

unsigned int bunnyVertexArrayObjID;
Model *bunny;
Model *cow;
GLuint program;
GLuint myTex;





void init(void) {
	/* two vertex buffer objects, used for uploading the*/
	unsigned int bunnyTexCoordBufferObjID;

	/* Reference to shader program*/

	/* GL inits*/
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);
	glActiveTexture(GL_TEXTURE0);
	printError("GL inits");

	/* Load and compile shader*/
	program = loadShaders("lab2-1.vert", "lab2-1.frag");
	glUseProgram(program);
	printError("init shader");
	/* Upload geometry to the GPU:*/

	bunny = LoadModelPlus("bunnyplus.obj");
    cow = LoadModelPlus("cow.obj");

	LoadTGATextureSimple("maskros512.tga", &myTex);

    glGenBuffers(1, &bunnyTexCoordBufferObjID);

    glBindVertexArray(bunnyVertexArrayObjID);

	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0


    if (bunny->texCoordArray != NULL)
    {
	    glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
	    glBufferData(GL_ARRAY_BUFFER, bunny->numVertices*2*sizeof(GLfloat), bunny->texCoordArray, GL_STATIC_DRAW);
	    glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	}

 	/* End of upload of geometry*/
 	Point3D p,l;
 	SetVector(0.0, 0.0, 1.0, &p);
 	SetVector(0.0, 0.0, -3.0, &l);

	lookAt(&p, &l, 0.0, 1.0, 0.0, &cam);

	T(0, 0, -2, trans);
	Ry(0.0, rot);
    Mult(rot, trans, total);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);


	printError("init arrays");
}

void display(void) {
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	T(0, 0, -2, trans);
	Ry(t/1000, rot);
    Mult(trans, rot, total);
	Rx(t/1000, rot);
    Mult(total, rot, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);

    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");
    DrawModel(cow, program, "inPosition", "inNormal", "inTexCoord");

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
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);
	init ();
	glutMainLoop();
}
