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
/*
Model* GenerateTerrain(TextureData *tex);
bool SameSide(Point3D p1, Point3D p2, Point3D a, Point3D b);
bool PointInTriangle(Point3D p, Point3D a, Point3D b, Point3D c);
GLfloat findY(int x, int z);
void testCollision(GLfloat y);
void keyController();
*/