#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/Dense>

#define WIDTH 1260
#define HEIGHT 760
#define PI 3.14159265

using namespace std;
using namespace Eigen;

int width = 1260, height = 760;
float cameraX = 0, cameraY = 10, cameraZ = -10;

float near = 0.1, far = 100.0;
float ratio = (float)WIDTH / HEIGHT;
float Fovy = 50.0;


float nearW = 0, nearH = 0;
float farW = 0, farH = 0;


void get_ratio(){
	
	Vector3d v(cameraX, cameraY, cameraZ);
	v = v.normalized();
	// get point on near plane

	Vector3d V_near, V_far;
	
	V_near = v * near;
	V_far = v* far;

	float nearCenterPoint[3];
	float farCenterPoint[3];
	
	nearCenterPoint[0] = V_near[0] + cameraX;
	nearCenterPoint[1] = V_near[1] + cameraY;
	nearCenterPoint[2] = V_near[2] + cameraZ;
	
	farCenterPoint[0] = V_far[0] + cameraX;
	farCenterPoint[1] = V_far[1] + cameraY;
	farCenterPoint[2] = V_far[2] + cameraZ;

	//get h/2, w/2
	
	nearH = 2.0 * tan((Fovy/2) * PI / 180.0) * near;
	nearW = nearH * ratio;

	farH = 2.0 * tan((Fovy/2) * PI / 180.0) * far;
	farW = farH *ratio;

	
	return;
}
void get_unprojected_point(float mouseX, float mouseY){

	float nearX = mouseX * nearW / WIDTH;
	float nearY = mouseY * nearH / HEIGHT;

	float farX = mouseX * farW / WIDTH;
	float farY = mouseY * farH / HEIGHT;

	cout << nearX << " " << nearY << endl;
	cout << farX << " " << farY << endl;
	return;
}

void init(){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}


void resize(int w, int h){
	glViewport(0,0, w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(Fovy, ratio, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ,0,0,0,1,0,0);

	return;
}

void drawSphere(){

	//sphere 1
	glPushMatrix();
		glColor3f(0.7, 0.0, 0.0);
		glTranslatef(0,0,2);
		glutSolidSphere(0.5f, 50, 50);
	glPopMatrix();
	glPushMatrix();
	//sphere 2
		glColor3f(0.0, 0.7, 0.0);
		glTranslatef(0,0,4);
		glutSolidSphere(0.5f, 50, 50);
	glPopMatrix();
	glPushMatrix();
	//sphere 3
		glColor3f(0.0, 0.0, 0.7);
		glTranslatef(0,0,6);
		glutSolidSphere(0.5f, 50, 50);
	glPopMatrix();
	glFlush();
	return;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0, 1.0, 1.0, 0.0);
	drawSphere();
	glColor3f(1.0,0.0,1.0);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,2);
	glVertex3f(0.0,0.0,4);
	glEnd();

	glutSwapBuffers();
	return;
}


void keyboard(unsigned char keyPressed, int x, int y){

	switch(keyPressed){
		case 'q':
			exit(0);
			break;

	}
	
	glutPostRedisplay();		

	return;
}

void mouse(int button, int state, int x, int y){

	if( button == GLUT_LEFT_BUTTON){
		cout << "mouse clicked!: x :"<< x <<"	y: " << y << endl;
		get_unprojected_point(x,y);	
	}

	return;
}

int main(int argc, char** argv){


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Homework 1 : Picking, 2016-21191");
	init();
	get_ratio();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(resize);
	glutMainLoop();
	return 0;
}
