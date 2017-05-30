#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "OBJloader.h"
#include "camera.h"

#define WIDTH 1200
#define HEIGHT 800

#define WINX 0
#define WINY 0

static Camera camera;
static unsigned int width = 700;
static unsigned int height = 700;

static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX = 0, lastY = 0, lastZoom = 0;




void drawXYZ(int size){
	glPushMatrix();
		glBegin(GL_LINES);
			//red X
			glColor3f(1,0,0);
			glVertex3f(0,0,0);
			glVertex3f(size,0,0);
			//green y
			glColor3f(0,1,0);
			glVertex3f(0,0,0);
			glVertex3f(0,size,0);
			//blue z
			glColor3f(0,0,1);
			glVertex3f(0,0,0);
			glVertex3f(0,0,size);
		glEnd();

	glPopMatrix();
	return;
}
void drawOBJ(OBJECT obj){
	glPushMatrix();
	glColor3f(0.5,0.5,0);
		for(int i=0; i < obj.getFaceSize(); i++){
			int idx1 = obj.face(i,0);
			int idx2 = obj.face(i,1);
			int idx3 = obj.face(i,2);
			glBegin(GL_LINES);
				glVertex3f(obj.vertex(idx1,0),obj.vertex(idx1,1), obj.vertex(idx1,2));
				glVertex3f(obj.vertex(idx2,0),obj.vertex(idx2,1), obj.vertex(idx2,2));
				glVertex3f(obj.vertex(idx3,0),obj.vertex(idx3,1), obj.vertex(idx3,2));
				glVertex3f(obj.vertex(idx1,0),obj.vertex(idx1,1), obj.vertex(idx1,2));
			glEnd();
		}
	glPopMatrix();
	return;
}

void drawFloor(int size){

	glPushMatrix();
	glColor3f(0.65,0.65,0.65);
	float stepSize = 0.1;

	for(int i= -size; i <= size; i++){
		glBegin(GL_LINES);
			glVertex3f(stepSize*(float)i, 0, 0);
			glVertex3f(stepSize*(float)i, (float)size*stepSize, 0);

			glVertex3f(0, stepSize*(float)i, 0);
			glVertex3f((float)size*stepSize, stepSize*(float)i, 0);
			
			glVertex3f(stepSize*(float)i, 0, 0);
			glVertex3f(stepSize*(float)i, -(float)size*stepSize, 0);

			glVertex3f(0, stepSize*(float)i, 0);
			glVertex3f(-(float)size*stepSize, stepSize*(float)i, 0);
		glEnd();

	}
	glPopMatrix();
	return;
}

void initGL(){

	if(glewInit() != GLEW_OK){
		cout << "GLEW init FAIL!" << endl;
		exit(EXIT_FAILURE);
	}
	if(!GLEW_VERSION_2_0){
		cout << "OpenGL 2.0 not supported" << endl;
		exit(EXIT_FAILURE);
	}
	return;
}

