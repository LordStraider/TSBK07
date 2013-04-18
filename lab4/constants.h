#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
#endif
#import <ApplicationServices/ApplicationServices.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA2.h"
#include "VectorUtils3.h"



#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

extern mat4 rot, trans, shear, total, cam, projectionMatrix;

extern TextureData ttex; // terrain

#define near 1.0
#define far 300.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0

extern GLfloat camPos, yCamPos, camMod, xModify, xValue, yModify, yValue, zModify, zValue;

extern GLfloat *ballY;
extern GLuint texWidth;
extern GLfloat *vertexArray;
extern GLuint *indexArray;

extern float gravity, angle, angleMod, rotate, speed;
extern bool menuPressed;

extern Point3D p, l;
extern GLuint program;

extern GLuint tex1;

extern Model *groundSphere, *tm;
//extern Model *windmill2;

//void init();