#include "draw.h"

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
	
/*	b = 1;
    glUniform1fv(glGetUniformLocation(program, "mode"), 1, &b);
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
*/
	printError("display 2");
	
    glFlush();
	//glutSwapBuffers();
}
