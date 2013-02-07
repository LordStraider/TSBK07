#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "LoadTGA.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils2.h"

/* Globals*/
#define PI 3.14159

GLfloat rot[16], trans[16], shear[16], total[16], cam[16];


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

GLfloat xModify;
GLfloat xValue;
GLfloat zModify;
GLfloat zValue;
GLfloat xLookAt;
GLfloat yLookAt;

Model *bunny;
Model *cow;
Model *blade;
Model *windmillWalls;
Model *windmillRoof;
Model *windmillBalcony;
GLuint program;
GLuint myTex;
Point3D p,l;





void init(void) {
	/* GL inits*/
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);
	glActiveTexture(GL_TEXTURE0);
	printError("GL inits");

	xModify = 0.0;
	zModify = 0.0;
	xValue = 0.0;
	zValue = -2.0;
	yLookAt = 0.0;
	xLookAt = 0.0;

	/* Load and compile shader*/
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	glUseProgram(program);
	printError("init shader");
	/* Upload geometry to the GPU:*/

	bunny = LoadModelPlus("bunnyplus.obj");
    cow = LoadModelPlus("cow.obj");
    windmillBalcony = LoadModelPlus("windmill-balcony.obj");
    windmillRoof = LoadModelPlus("windmill-roof.obj");
    windmillWalls = LoadModelPlus("windmill-walls.obj");
    blade = LoadModelPlus("blade.obj");

	LoadTGATextureSimple("maskros512.tga", &myTex);

	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

 	/* End of upload of geometry*/
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);


	printError("init arrays");
}

void display(void) {
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	SetVector(xValue, 0.0, (zValue+3), &p);
	lookAt(&p, &l, 0.0, 1.0, 0.0, &cam);


	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);

    T(-3.9, -7.4, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(windmillRoof, program, "inPosition", "inNormal", "inTexCoord");

    T(-3.9, -7.4, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(windmillBalcony, program, "inPosition", "inNormal", "inTexCoord");

    T(-3.9, -7.4, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);    
	DrawModel(windmillWalls, program, "inPosition", "inNormal", "inTexCoord");

	int i;
	for (i = 0; i < 4; i++) {
	    T(0, 0, 0, trans);
	    S(0.5, 0.5, 0.5, shear);
	    Mult(trans, shear, total);
		Rx(i * PI / 2 + t/1000, rot);
	    Mult(rot, total, total);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	    DrawModel(blade, program, "inPosition", "inNormal", "inTexCoord");
	}

	printError("display");

	glutSwapBuffers();
}

void MouseController(int x, int y){
	if (x > 200) {
		xLookAt += 0.08;
	} else {
		xLookAt -= 0.08;
	}

	if (y > 200) {
		yLookAt -= 0.08;
	} else {
		yLookAt += 0.08;
	}

 	SetVector(xLookAt, yLookAt, 0, &l);
}

void OnTimer(int value) {
	xModify = 0.0;
	zModify = 0.0;
	
	
	if (keyIsDown('w')){
		zModify = -0.08;
	} 
	if (keyIsDown('s')){
		zModify = 0.08;
	} 
	if (keyIsDown('a')){
		xModify = -0.08;
	} 
	if (keyIsDown('d')){
		xModify = 0.08;
	}

	xValue += xModify;
	zValue += zModify;

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	initKeymapManager();
	//glutPassiveMotionFunc(MouseController);
	glutTimerFunc(20, &OnTimer, 0);
	init ();
	glutMainLoop();
}
