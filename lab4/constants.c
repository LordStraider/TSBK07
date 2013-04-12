#include "constants.h"


GLfloat camPos;
GLfloat yCamPos;
GLfloat camMod;
GLfloat xModify;
GLfloat xValue;
GLfloat yModify;
GLfloat yValue;
GLfloat zModify;
GLfloat zValue;

float gravity;
float angle;
float angleMod;
float rotate;
float speed;

mat4 projectionMatrix, trans, shear, total;
GLuint texWidth;
Model *groundSphere;
GLfloat *vertexArray;
GLuint *indexArray;
GLfloat *ballY;

// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint tex1, tex2;

Point3D p,l;
GLuint program;



TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");


    groundSphere = LoadModelPlus("groundsphere.obj");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);
	
// Load terrain data
	
	LoadTGATexture("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}



