#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
#endif
#import <ApplicationServices/ApplicationServices.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "VectorUtils2.h"



#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

extern GLfloat camPos;
extern GLfloat yCamPos;
extern GLfloat camMod;
extern GLfloat xModify;
extern GLfloat xValue;
extern GLfloat yModify;
extern GLfloat yValue;
extern GLfloat zModify;
extern GLfloat zValue;

extern float gravity;
extern float angle;
extern float angleMod;
extern float rotate;
extern float speed;

extern Point3D p,l;
extern GLuint program;