
// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
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
	GLfloat y;

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x;
			y = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 25.0;
			if (y < 1.5) {
				y = 1.5;
			}
			vertexArray[(x + z * tex->width)*3 + 1] = y;
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
			p = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);

			index = (x + 1 + z * tex->width) * 3;
			tmp = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);
			cross = CrossProduct(tmp, p);
			sum = VectorAdd(cross, sum);

			index = (x + 1 + (z - 1) * tex->width) * 3;
			tmp = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);
			cross = CrossProduct(tmp, p);
			sum = VectorAdd(cross, sum);

			index = (x - 1 + (z - 1) * tex->width) * 3;
			tmp = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);
			cross = CrossProduct(tmp, p);
			sum = VectorAdd(cross, sum);

			sum = Normalize(sum);
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

bool SameSide(Point3D p1, Point3D p2, Point3D a, Point3D b) {
	Point3D sub1, sub2, sub3, result, result2;
	sub1 = VectorSub(b, a);
	sub2 = VectorSub(p1, a);
	sub3 = VectorSub(p2, a);
    result = CrossProduct(sub1, sub2);
    result2 = CrossProduct(sub1, sub3);
    
    if (DotProduct(result, result2) >= 0) {
    	return true;
    }

    return false;
}

bool PointInTriangle(Point3D p, Point3D a, Point3D b, Point3D c) {
    if (SameSide(p,a, b,c) && SameSide(p,b, a,c) && SameSide(p,c, a,b)) {
    	return true;
    }

  	return false;
}

GLfloat findY(int x, int z) {
	GLuint triangle1[3]; 
	GLuint triangle2[3]; 
	GLfloat y = 0.0;

	triangle1[0] = indexArray[(x + z * (texWidth-1))*6 + 0] * 3; //x
	triangle1[1] = indexArray[(x + z * (texWidth-1))*6 + 1] * 3; //y
	triangle1[2] = indexArray[(x + z * (texWidth-1))*6 + 2] * 3; //z
	triangle2[0] = indexArray[(x + z * (texWidth-1))*6 + 3] * 3; //x
	triangle2[1] = indexArray[(x + z * (texWidth-1))*6 + 4] * 3; //y
	triangle2[2] = indexArray[(x + z * (texWidth-1))*6 + 5] * 3; //z

	Point3D p, v1, v2, v3, norm;

	p = SetVector(x, 0, z);
	v1 = SetVector(vertexArray[triangle1[0]], 0, vertexArray[triangle1[0] + 2]);
	v2 = SetVector(vertexArray[triangle1[1]], 0, vertexArray[triangle1[1] + 2]);
	v3 = SetVector(vertexArray[triangle1[2]], 0, vertexArray[triangle1[2] + 2]);

	GLfloat d; 
	if (PointInTriangle(p, v1, v2, v3)) {
		v1 = SetVector(vertexArray[triangle1[0]], vertexArray[triangle1[0] + 1], vertexArray[triangle1[0] + 2]);
		v2 = SetVector(vertexArray[triangle1[1]], vertexArray[triangle1[1] + 1], vertexArray[triangle1[1] + 2]);
		v3 = SetVector(vertexArray[triangle1[2]], vertexArray[triangle1[2] + 1], vertexArray[triangle1[2] + 2]);
	} else {
		v1 = SetVector(vertexArray[triangle2[0]], vertexArray[triangle2[0] + 1], vertexArray[triangle2[0] + 2]);
		v2 = SetVector(vertexArray[triangle2[1]], vertexArray[triangle2[1] + 1], vertexArray[triangle2[1] + 2]);
		v3 = SetVector(vertexArray[triangle2[2]], vertexArray[triangle2[2] + 1], vertexArray[triangle2[2] + 2]);
	}

	norm = CalcNormalVector(v1, v2, v3);
	norm = Normalize(norm);
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
			center1 = SetVector(xValue, y+0.5, zValue);
			center2 = SetVector(i*10, ballY[(i-1) + (j-1)*10]+0.5, j*10);

			result = VectorSub(center2, center1);
			//printf("x: %f, y: %f, z: %f, i: %d, j: %d, sqrt: %f\n", result.x, result.y, result.z, i, j, sqrt(result.x * result.x + result.y * result.y + result.z * result.z));

			if (sqrt(result.x * result.x + result.y * result.y + result.z * result.z) < 1.97) {
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

	mat4 modelView, camMatrix, tmp, rot;
	
    camPos += camMod;
    GLfloat y;
    
    xValue += xModify * speed;
    yValue += yModify;
    zValue += zModify * speed;
    y = findY(xValue, zValue);
	testCollision(y);

   	p = SetVector(xValue + 9 * cos(camPos), y+2, zValue + 9 * sin(camPos));
    l = SetVector(xValue, y+3, zValue);

    vec3 v = SetVector(0.0, 1.0, 0.0);
    camMatrix = lookAtv(p, l, v);

	printError("pre display");
	
	glUseProgram(program);

	p.y += 14;
	int b = 0;
    glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &p);
    glUniform1fv(glGetUniformLocation(program, "mode"), 1, &b);

	// Build matrix
	
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	tmp = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, tmp.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	
	b = 1;
    glUniform1fv(glGetUniformLocation(program, "mode"), 1, &b);
    trans = T(xValue, findY(xValue, zValue), zValue);
    rot = Ry(rotate+angle);
    total = Mult(trans, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(groundSphere, program, "inPosition", "inNormal", "inTexCoord");

    int i, j;
    for (i = 1; i < 11; i++) {
	    for (j = 1; j < 11; j++) {
	    	y = findY(i * 10, j * 10);
	    	ballY[(i-1) + (j-1)*10] = y;
		    trans = T(i * 10, y, j * 10);
		    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans.m);
		    DrawModel(groundSphere, program, "inPosition", "inNormal", "inTexCoord");
	    }
	}

	printError("display 2");
	
    glFlush();
	//glutSwapBuffers();
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
