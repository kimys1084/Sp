#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "header.h"
#include "OBJloader.h"

#define WIDTH 1200
#define HEIGHT 800


using namespace std;

OBJECT bunny;
int SCALE =5;
float rotation = 0;
float depths[WIDTH*HEIGHT];

void keyboard(unsigned char key, int x, int y){

	switch(key){
		case 'q':
			exit(0);
			break;

		default:
			break;
	}

	glutPostRedisplay();

	return;
}
void reshape(GLint w, GLint h){

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return;
}
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
void drawOBJ(){
	glPushMatrix();
		for(int i=0; i < bunny.getFaceNumber(); i++){
			int idx1 = bunny.face(i,0);
			int idx2 = bunny.face(i,1);
			int idx3 = bunny.face(i,2);

			glBegin(GL_LINES);
			glVertex3f(bunny.vertex(idx1,0), bunny.vertex(idx1,1), bunny.vertex(idx1,2));
			glVertex3f(bunny.vertex(idx2,0), bunny.vertex(idx2,1), bunny.vertex(idx2,2));
			glVertex3f(bunny.vertex(idx3,0), bunny.vertex(idx3,1), bunny.vertex(idx3,2));
			glVertex3f(bunny.vertex(idx1,0), bunny.vertex(idx1,1), bunny.vertex(idx1,2));
			glEnd();
		}
	glPopmatrix();
}
	
void display(){


	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	drawXYZ(100);

	
	glutSwapBuffers();
	return;
}


int main(int argc, char *argv[]){

	bunny.loadOBJFile("bunny.obj");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("systemProgramming hw2 201621191 kys");
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	//glutIdleFunc(display);
	glutMainLoop();
	return 0;
}

