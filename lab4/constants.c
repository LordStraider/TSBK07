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

GLfloat projectionMatrix[16], trans[16], shear[16], total[16];
GLuint texWidth;
Model *groundSphere;
GLfloat *vertexArray;
GLuint *indexArray;

// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint tex1, tex2;

Point3D p,l;
GLuint program;





