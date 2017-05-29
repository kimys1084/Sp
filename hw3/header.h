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
			glColor3f(1,0,0);
			glVertex3f(0,0,0);
			glVertex3f(size,0,0);

			glColor3f(0,1,0);
			glVertex3f(0,0,0);
			glVertex3f(0,size,0);
		
			glColor3f(0,0,1);
			glVertex3f(0,0,0);
			glVertex3f(0,0,size);
		glEnd();

	glPopMatrix();
	return;
}
