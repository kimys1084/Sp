#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "libeigen/Eigen/Dense"
#include "picking.h"

using namespace std;
using namespace Eigen;



static bool mouseMovePressed = false;

int width = 1260, height = 760;
int lastX, lastY;
int Move = 0;
float cameraX = 0, cameraY = 10, cameraZ = -10;

float near = 0.1, far = 100.0;
float ratio = (float)WIDTH / HEIGHT;
float Fovy = 50.0;


float nearW = 0, nearH = 0;
float farW = 0, farH = 0;

Sphere O[3];

void initSphere(){
	O[0].id = 0;
	O[0].radius = 0.5;
	O[0].RGB[0] = 0.7, O[0].RGB[1] = 0.0, O[0].RGB[2] = 0.0;
	O[0].curPos[0] = 0, O[0].curPos[1] = 0, O[0].curPos[2] = 2;
	O[0].ratio = 0.9945;

	O[1].id = 1;
	O[1].radius = 0.5;
	O[1].RGB[0] = 0.0, O[1].RGB[1] = 0.7, O[1].RGB[2] = 0.0;
	O[1].curPos[0] = 0, O[1].curPos[1] = 0, O[1].curPos[2] = 4;
	O[1].ratio = 0.995;
	O[2].id = 2;
	O[2].radius = 0.5;
	O[2].RGB[0] = 0.0, O[2].RGB[1] = 0.0, O[2].RGB[2] = 0.7;
	O[2].curPos[0] = 0, O[2].curPos[1] = 0, O[2].curPos[2] = 6;
	O[2].ratio = 0.9953;
	return;
}
int intersectionCheck(float farX, float farY, float farZ){

	float a = (farX - cameraX) * (farX - cameraX) + (farY - cameraY) * (farY - cameraY) + (farZ - cameraZ) * (farZ - cameraZ); 

	for( int id = 0; id < 3; id++){
		float b = 2*((farX - cameraX) * (cameraX - O[id].curPos[0]) + (farY - cameraY) * (cameraY - O[id].curPos[1]) + (farZ - cameraZ) * (cameraZ - O[id].curPos[2]));

		float c = (cameraX - O[id].curPos[0]) * (cameraX - O[id].curPos[0]) + (cameraY - O[id].curPos[1]) * (cameraY - O[id].curPos[1]) + (cameraZ - O[id].curPos[2]) * (cameraZ - O[id].curPos[2]) - (O[id].radius * O[id].radius);

		float delta = b*b - 4*a*c;
		if( delta >= 0){
			return id;
		}
	}

	return -1; // true or false
}

void Touched(int id){


	float R = (rand() % 10)/ 10.0;
	float G = (rand() % 10)/ 10.0;
	float B = (rand() % 10)/ 10.0;

	O[id].RGB[0] = R;
	O[id].RGB[1] = G;
	O[id].RGB[2] = B;	

	return;
}

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
Vector3d unProject(int mouseX, int mouseY, float z){

	GLdouble projection[16];
	GLdouble modelView[16];
	GLint viewPort[4];

	
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetDoublev(GL_MODELVIEW_MATRIX,modelView);
	glGetIntegerv(GL_VIEWPORT,viewPort);
	
	GLfloat zCursor, winX, winY;
	winX = (float)mouseX;
	winY = HEIGHT - (float)mouseY;
	zCursor = (float)z;

	GLdouble wx, wy, wz;
	gluUnProject(winX,winY,zCursor, modelView, projection, viewPort, &wx, &wy, &wz);
	
	Vector3d ret(wx,wy,wz);
	return ret;
}

int get_unprojected_point(float mouseX, float mouseY){

	Vector3d near_coord, far_coord;
	near_coord = unProject(mouseX, mouseY, 0);
	far_coord = unProject(mouseX, mouseY, 1);

	cout << "near -> x : "  <<  near_coord(0) << " y : "<< near_coord(1) << " z : "<< near_coord(2)<<endl;
	cout << "far -> x : "  <<  far_coord(0) << " y : "<< far_coord(1) << " z : "<< far_coord(2)<<endl<<endl;
	
	return intersectionCheck(far_coord(0), far_coord(1), far_coord(2));	
}

void init(){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	initSphere();
}


void resize(int w, int h){
	glViewport(0,0, w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(Fovy, ratio, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ,0,0,0,0,1,0);

	return;
}

void drawSphere(){

	//sphere 1
	glPushMatrix();
		glColor3f(O[0].RGB[0],O[0].RGB[1], O[0].RGB[2]);
		glTranslatef(O[0].curPos[0],O[0].curPos[1],O[0].curPos[2]);
		glutSolidSphere(O[0].radius, 50, 50);
	glPopMatrix();
	glPushMatrix();
	//sphere 2
		glColor3f(O[1].RGB[0],O[1].RGB[1], O[1].RGB[2]);
		glTranslatef(O[1].curPos[0],O[1].curPos[1],O[1].curPos[2]);
		glutSolidSphere(O[1].radius, 50, 50);
	glPopMatrix();
	glPushMatrix();
	//sphere 3
		glColor3f(O[2].RGB[0],O[2].RGB[1], O[2].RGB[2]);
		glTranslatef(O[2].curPos[0],O[2].curPos[1],O[2].curPos[2]);
		glutSolidSphere(O[2].radius, 50, 50);
	glPopMatrix();
	glFlush();
	return;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0, 1.0, 1.0, 0.0);
	drawSphere();
	glEnd();

	glutSwapBuffers();
	return;
}


void keyboard(unsigned char keyPressed, int x, int y){

	switch(keyPressed){
		case 'q':
			exit(0);
			break;
		case 's':
			Touched(0);
			break;
	}
	
	glutPostRedisplay();		

	return;
}

void mouse(int button, int state, int x, int y){

	
	if( button == GLUT_UP){
		mouseMovePressed = false;

	}
	else if( button == GLUT_LEFT_BUTTON){
		lastX = x;
		lastY = y;
		mouseMovePressed = true;
		cout << "pixel : x :"<< x <<"	y: " << y << endl;
		
		int sphereID = get_unprojected_point(x, y);
		if( sphereID >= 0){
			Touched(sphereID);
			Move = sphereID;
			cout << "SphereID : " << sphereID  << " Touched! "<< endl;
		}
		else{
			Move = -1;
		}
	}
	
	glutPostRedisplay();		

	return;
}
void moveSphere(Vector3d nearP, Vector3d farP, int id, int mouseX, int mouseY){

	float ratioX, ratioY, ratioZ;
	Vector3d currentPos(O[id].curPos[0], O[id].curPos[1], O[id].curPos[2]);
	
	Vector3d shortV, longV;

	shortV = currentPos - nearP;
	longV = farP - nearP;

	float Ratio = (longV(2) - shortV(2)) / longV(2);
	Ratio = O[id].ratio;
	Vector3d ret = unProject(mouseX, mouseY, Ratio);

	O[id].curPos[0] = ret(0);
	O[id].curPos[1] = ret(1);
	O[id].curPos[2] = ret(2);

	return;

}
void motion(int x, int y){

	if( mouseMovePressed == true){
		if(Move != -1){ // move sphere
			Vector3d nearP = unProject(lastX, lastY, 0);
			Vector3d farP= unProject(lastX, lastY, 1);
			moveSphere(nearP, farP, Move, x,y);
		}
		lastX = x;
		lastY = y;
	}


	glutPostRedisplay();
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
	glutMotionFunc(motion);
	glutReshapeFunc(resize);
	glutMainLoop();
	return 0;
}
