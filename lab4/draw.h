#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
#endif
#import <ApplicationServices/ApplicationServices.h>
#include "GL_utilities.h"
#include "LoadTGA2.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"
#include <stdio.h>
#include "constants.h"
#include "controller.h"


void display();
void displayTexture();
void displaySingleColor(GLfloat t);
void displayInvisible();
void displayModels(GLfloat t);
void displayShadows(GLfloat t);
void displayNoLight();