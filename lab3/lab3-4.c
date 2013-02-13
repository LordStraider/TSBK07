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


Point3D lightSourcesColorsArr[] = {  {1.0f, 0.0f, 0.0f}, // Red light
                                     {0.0f, 1.0f, 0.0f}, // Green light
                                     {0.0f, 0.0f, 1.0f}, // Blue light
                                     {1.0f, 1.0f, 1.0f} }; // White light

GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};

Point3D lightSourcesDirectionsPositions[] = {  {10.0f, 5.0f, 0.0f}, // Red light, positional
                                               {0.0f, 5.0f, 10.0f}, // Green light, positional
                                               {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                               {0.0f, 0.0f, -1.0f} }; // White light along Z
                                       
#define near 1.0
#define far 90.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projMatrix[] = {        2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                0.0f, 0.0f, -1.0f, 0.0f };

GLfloat camPos;
GLfloat camMod;
GLfloat xModify;
GLfloat xValue;
GLfloat zModify;
GLfloat zValue;
GLfloat xLookAt;
GLfloat yLookAt;

GLuint program;
GLuint programShade;
Point3D p,l;

Model *blade;
Model *windmillWalls;
Model *windmillRoof;
Model *windmillBalcony;
Model *skyBox;
Model *ground;

GLuint bunnyTex;
GLuint groundTex;
GLuint skyBoxTex;





void init(void) {
    /* GL inits*/
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_TRUE);
    glActiveTexture(GL_TEXTURE0);
    printError("GL inits");

    xValue = 0.0;
    zValue = -2.0;
    yLookAt = 0.0;
    xLookAt = 0.0;

    /* Load and compile shader*/
    program = loadShaders("lab3-1.vert", "lab3-1.frag");
    programShade = loadShaders("Shade.vert", "Shade.frag");
    glUseProgram(program);
    printError("init shader");
    /* Upload geometry to the GPU:*/

    windmillBalcony = LoadModelPlus("windmill-balcony.obj");
    windmillRoof = LoadModelPlus("windmill-roof.obj");
    windmillWalls = LoadModelPlus("windmill-walls.obj");
    blade = LoadModelPlus("blade.obj");
    skyBox = LoadModelPlus("skybox.obj");
    ground = LoadModelPlus("skybox.obj");

    LoadTGATextureSimple("grass.tga", &groundTex);
    LoadTGATextureSimple("maskros512.tga", &bunnyTex);
    LoadTGATextureSimple("skyBox512.tga", &skyBoxTex);

    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

    /* End of upload of geometry*/
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programShade);
    glUniformMatrix4fv(glGetUniformLocation(programShade, "projMatrix"), 1, GL_TRUE, projMatrix);



    glUniform3fv(glGetUniformLocation(programShade, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(programShade, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(programShade, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(programShade, "isDirectional"), 4, isDirectional);



    printError("init arrays");
}

void display(void) {
    printError("pre display");

    /* clear the screen*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    camPos += camMod;
    SetVector(xValue + 3 * cos(camPos), 2.0, zValue + 3 * sin(camPos), &p);
    SetVector(xValue, 2.5, zValue, &l);


    lookAt(&p, &l, 0.0, 1.0, 0.0, &cam);
    cam[3] = 0;
    cam[7] = 0;
    cam[11] = 0;
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);
  

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    T(0, 0, 0, trans);
    glBindTexture(GL_TEXTURE_2D, skyBoxTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans);
    DrawModel(skyBox, program, "inPosition", "inNormal", "inTexCoord");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    lookAt(&p, &l, 0.0, 1.0, 0.0, &cam);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);

//    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");
//    DrawModel(cow, program, "inPosition", "inNormal", "inTexCoord");

    T(0, 1, 0, trans);
    S(70,0, 70, shear);
    Mult(shear, trans, total);
    glBindTexture(GL_TEXTURE_2D, groundTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(ground, program, "inPosition", "inNormal", "inTexCoord");



    glUseProgram(programShade);
    glUniformMatrix4fv(glGetUniformLocation(programShade, "camMatrix"), 1, GL_TRUE, cam);
    T(-3.9, 0, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
    glUniformMatrix4fv(glGetUniformLocation(programShade, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(windmillRoof, programShade, "inPosition", "inNormal", "inTexCoord");

    T(-3.9, 0, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
    glUniformMatrix4fv(glGetUniformLocation(programShade, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(windmillBalcony, programShade, "inPosition", "inNormal", "inTexCoord");

    T(-3.9, 0, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
    glUniformMatrix4fv(glGetUniformLocation(programShade, "mdlMatrix"), 1, GL_TRUE, total);    
    DrawModel(windmillWalls, programShade, "inPosition", "inNormal", "inTexCoord");

    int i;
    for (i = 0; i < 4; i++) {
        T(0, 7.4, 0, trans);
        S(0.5, 0.5, 0.5, shear);
        Mult(trans, shear, total);
        Rx(i * PI / 2 + t/1000, rot);
        Mult(total, rot, total);
        glUniformMatrix4fv(glGetUniformLocation(programShade, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(blade, programShade, "inPosition", "inNormal", "inTexCoord");
    }


    printError("display");

    glutSwapBuffers();
}

void MouseController(int x, int y){
    if (x > 200) {
        xLookAt += 0.15;
    } else {
        xLookAt -= 0.15;
    }

    if (y > 200) {
        yLookAt -= 0.15;
    } else {
        yLookAt += 0.15;
    }
    
//  glutWarpPointer(100, 100);
    SetVector(xLookAt, yLookAt, 0, &l);
}

void OnTimer(int value) {
    xModify = 0.0;
    zModify = 0.0;
    camMod = 0.0;
    
    
    if (keyIsDown('w')){
        zModify = -0.15;
    } 
    if (keyIsDown('s')){
        zModify = 0.15;
    } 
    if (keyIsDown('a')){
        xModify = -0.15;
    } 
    if (keyIsDown('d')){
        xModify = 0.15;
    }

    if (keyIsDown('x')) {
        camMod = 0.08;
    } else if (keyIsDown('z')) {
        camMod = -0.08;
    }

    xValue += xModify;
    zValue += zModify;

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[]) {
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (800, 640);
    
    glutInit(&argc, argv);
    glutCreateWindow ("GL3 white triangle example");
    glutDisplayFunc(display);
    initKeymapManager();
    //glutPassiveMotionFunc(MouseController);
    glutTimerFunc(20, &OnTimer, 0);
    init ();
    glutMainLoop();
}
