// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"
#include "constants.h"
#include "controller.h"

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	ballY = malloc(sizeof(GLfloat) * 100);
	
	texWidth = tex->width;

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 50.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}

	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
/*
	Point3D *TmpCrossArray[3 * 9];
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++) {
			//vår punkt!
			Point3D p = vertexArray[(x + z * tex->width)*3 + 0], vertexArray[(x + z * tex->width)*3 + 1], vertexArray[(x + z * tex->width)*3 + 2]);
			//Hämta alla vertices som är grannar med oss och ta kryssprodukten av dem!
			for (k = -1; k < 1; k++){
				for (j = -1; j < 1; j++){
					// Funkar (?) men problem vid hörn och kanter!! 
					TmpCrossArray[k + 1 + (j + 1) * 3] = CrossProduct(
Point3D(vertexArray[((x+k) + (z+j) * tex->width)*3 + 0], vertexArray[((x+k) + (z+j) * tex->width)*3 + 1], vertexArray[((x+k) + (z+j) * tex->width)*3 + 2]), p);

				}
			}
			//Ta medelvärde av TmpCrossArray för alla x, y, z och lägg in i normalArray.

			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
		}*/

	Point3D p;
	Point3D cross;
	Point3D sum;
	Point3D tmp;
	int index;
	for (x = 1; x < tex->width - 1; x++)
		for (z = 1; z < tex->height - 1; z++)
		{
			//vår punkt!
			index = (x + z * tex->width)*3;
			SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2], &p);

			index = (x + 1 + z * tex->width) * 3;
			SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2], &tmp);
			CrossProduct(&tmp, &p, &cross);
			VectorAdd(&cross, &sum, &sum);

			index = (x + 1 + (z - 1) * tex->width) * 3;
			SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2], &tmp);
			CrossProduct(&tmp, &p, &cross);
			VectorAdd(&cross, &sum, &sum);

			index = (x - 1 + (z - 1) * tex->width) * 3;
			SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2], &tmp);
			CrossProduct(&tmp, &p, &cross);
			VectorAdd(&cross, &sum, &sum);

			Normalize(&sum);
			normalArray[(x + z * tex->width)*3 + 0] = sum.x;
			normalArray[(x + z * tex->width)*3 + 1] = -sum.y;
			normalArray[(x + z * tex->width)*3 + 2] = sum.z;
		}

	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0, projectionMatrix);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");


    groundSphere = LoadModelPlus("groundsphere.obj");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("grass.tga", &tex1);
	
// Load terrain data
	
	LoadTGATexture("ok1.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}

bool SameSide(Point3D *p1, Point3D *p2, Point3D *a, Point3D *b) {
	Point3D sub1, sub2, sub3, result, result2;
	VectorSub(b, a, &sub1);
	VectorSub(p1, a, &sub2);
	VectorSub(p2, a, &sub3);
    CrossProduct(&sub1, &sub2, &result);
    CrossProduct(&sub1, &sub3, &result2);
    
    if (DotProduct(&result, &result2) >= 0) {
    	return true;
    }

    return false;
}

bool PointInTriangle(Point3D *p, Point3D *a, Point3D *b, Point3D *c) {
    if (SameSide(p,a, b,c) && SameSide(p,b, a,c) && SameSide(p,c, a,b)) {
    	return true;
    }

  	return false;
}

GLfloat findY(int x, int z) {
	GLuint triangle1[3]; 
	GLuint triangle2[3]; 
	GLfloat y = 0.0;

	triangle1[0] = indexArray[(x + z * (texWidth-1))*6 + 0]; //x
	triangle1[1] = indexArray[(x + z * (texWidth-1))*6 + 1]; //y
	triangle1[2] = indexArray[(x + z * (texWidth-1))*6 + 2]; //z
	triangle2[0] = indexArray[(x + z * (texWidth-1))*6 + 3]; //x
	triangle2[1] = indexArray[(x + z * (texWidth-1))*6 + 4]; //y
	triangle2[2] = indexArray[(x + z * (texWidth-1))*6 + 5]; //z

	Point3D p, v1, v2, v3, norm;

	SetVector(x, 0, z, &p);
	SetVector(vertexArray[triangle1[0]*3 + 0], 0, vertexArray[triangle1[0]*3 + 2], &v1);
	SetVector(vertexArray[triangle1[1]*3 + 0], 0, vertexArray[triangle1[1]*3 + 2], &v2);
	SetVector(vertexArray[triangle1[2]*3 + 0], 0, vertexArray[triangle1[2]*3 + 2], &v3);

	GLfloat d; 
	if (PointInTriangle(&p, &v1, &v2, &v3)) {
		SetVector(vertexArray[triangle1[0]*3 + 0], vertexArray[triangle1[0]*3 + 1], vertexArray[triangle1[0]*3 + 2], &v1);
		SetVector(vertexArray[triangle1[1]*3 + 0], vertexArray[triangle1[1]*3 + 1], vertexArray[triangle1[1]*3 + 2], &v2);
		SetVector(vertexArray[triangle1[2]*3 + 0], vertexArray[triangle1[2]*3 + 1], vertexArray[triangle1[2]*3 + 2], &v3);
	} else {
		SetVector(vertexArray[triangle2[0]*3 + 0], vertexArray[triangle2[0]*3 + 1], vertexArray[triangle2[0]*3 + 2], &v1);
		SetVector(vertexArray[triangle2[1]*3 + 0], vertexArray[triangle2[1]*3 + 1], vertexArray[triangle2[1]*3 + 2], &v2);
		SetVector(vertexArray[triangle2[2]*3 + 0], vertexArray[triangle2[2]*3 + 1], vertexArray[triangle2[2]*3 + 2], &v3);
	}

	CalcNormalVector(&v1, &v2, &v3, &norm);
	Normalize(&norm);
	// A * x + B * y + C * z - D = 0 => y = (D - A*x - C*z) / B
	d = norm.x * v1.x + norm.y * v1.y + norm.z * v1.z;
	y = (d - norm.x * v1.x - norm.z * v1.z) / norm.y;
	
	return y;
}

void testCollision(GLfloat y) {
	int i, j;
	for (i = 1; i < 11; i++){
		for (j = 1; j < 11; j++){
			Point3D center1, center2, result;
			SetVector(xValue, y, zValue, &center1);
			SetVector(i*10, ballY[(i-1) + (j-1)*10], j*10, &center2);

			VectorSub(&center2, &center1, &result);
			//printf("x: %f, y: %f, z: %f, i: %d, j: %d, sqrt: %f\n", result.x, result.y, result.z, i, j, sqrt(result.x * result.x + result.y * result.y + result.z * result.z));

			if (sqrt(result.x * result.x + result.y * result.y + result.z * result.z) < 1.99) {
			    xValue -= xModify * speed;
			    zValue -= zModify * speed;
			    return 0;
			}
		}		
	}
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat total[16], modelView[16], camMatrix[16], tmp[16], rot[16];
	
    camPos += camMod;
    GLfloat y,y2;
    
    xValue += xModify * speed;
    yValue += yModify;
    zValue += zModify * speed;
    y = findY(xValue, zValue);
	testCollision(y);

    y2 = findY(xValue + 5 * cos(camPos), zValue + 5 * sin(camPos));

    if (y > 7) {
    	SetVector(xValue + 5 * cos(camPos), y+1, zValue + 5 * sin(camPos), &p);
    } else {
    	SetVector(xValue + 5 * cos(camPos), 4, zValue + 5 * sin(camPos), &p);
    }
    SetVector(xValue, 4, zValue, &l);

    
    lookAt(&p, &l, 0.0, 1.0, 0.0, camMatrix);

	printError("pre display");
	
	glUseProgram(program);

	p.y += 14;
	int b = 0;
    glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &p);
    glUniform1fv(glGetUniformLocation(program, "useSpec"), 1, &b);

	// Build matrix
	
	IdentityMatrix(modelView);
	Mult(camMatrix, modelView, total);
	IdentityMatrix(tmp);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, tmp);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	
	b = 1;
    glUniform1fv(glGetUniformLocation(program, "useSpec"), 1, &b);
    T(xValue, findY(xValue, zValue), zValue, trans);
    Ry(rotate+angle, rot);
    Mult(trans, rot, total);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(groundSphere, program, "inPosition", "inNormal", "inTexCoord");

    int i, j;
    for (i = 1; i < 11; i++) {
	    for (j = 1; j < 11; j++) {
	    	y = findY(i * 10, j * 10);
	    	ballY[(i-1) + (j-1)*10] = y;
		    T(i * 10, y, j * 10, trans);
		    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans);
		    DrawModel(groundSphere, program, "inPosition", "inNormal", "inTexCoord");
	    }
	}

	printError("display 2");
	
	glutSwapBuffers();
}

void timer(int i)
{
    keyController();

	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	//printf("%d %d\n", x, y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
