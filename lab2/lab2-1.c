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
/* Globals*/
#define PI 3.14159

GLfloat rotationMatrixX[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat rotationMatrixY[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat rotationMatrixZ[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat translationMatrix[] = { 1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, -2.0f,
								0.0f, 0.0f, 0.0f, 1.0f };


#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                            0.0f, 0.0f, -1.0f, 0.0f };

unsigned int bunnyVertexArrayObjID;
Model *m;
GLuint program;
GLuint myTex;





void setSincosX(GLfloat* m, float alpha) { 
	m[5] = cos(alpha);
	m[6] = -sin(alpha);
	m[9] = sin(alpha);
	m[10] = cos(alpha);
}

void setSincosY(GLfloat* m, float alpha) { 
	m[0] = cos(alpha);
	m[2] = sin(alpha);
	m[8] = -sin(alpha);
	m[10] = cos(alpha);
}

void setSincosZ(GLfloat* m, float alpha) { 
	m[0] = cos(alpha);
	m[1] = -sin(alpha);
	m[4] = sin(alpha);
	m[5] = cos(alpha);
}

void init(void) {	
	/* two vertex buffer objects, used for uploading the*/

	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;

	/* Reference to shader program*/

	/* GL inits*/
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);
//	glActiveTexture(GL_TEXTURE0);
	printError("GL inits");

	/* Load and compile shader*/
	program = loadShaders("lab2-1.vert", "lab2-1.frag");
	glUseProgram(program);
	printError("init shader");
	/* Upload geometry to the GPU:*/

	m = LoadModel("bunnyplus.obj");
	LoadTGATextureSimple("maskros512.tga", &myTex);

    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    glGenBuffers(1, &bunnyTexCoordBufferObjID);
    
    glBindVertexArray(bunnyVertexArrayObjID);
	
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0


    if (m->texCoordArray != NULL)
    {
	    glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
	    glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
	    glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	}

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

 	/* End of upload of geometry*/
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixX"), 1, GL_TRUE, rotationMatrixX);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixZ"), 1, GL_TRUE, rotationMatrixZ);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);


	printError("init arrays");
}

void display(void) {
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	setSincosY(&rotationMatrixY, t/1000);
	setSincosZ(&rotationMatrixZ, t/1000);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixZ"), 1, GL_TRUE, rotationMatrixZ);

    glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
	
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
