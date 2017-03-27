#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

int width = 1260, height = 760;

void init(){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}

void resize(int w, int h){
	glViewport(0,0, w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluLookAt(0,10,-10,0,0,0,0,1,0);
	gluPerspective(50.0, ((float)w)/h, 0.1, 100.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return;
}

void drawSphere(){

	//sphere 1
	glPushMatrix();
		glColor3f(0.7, 0.0, 0.0);
		glTranslatef(10,0,10);
		glutSolidSphere(0.5f, 50, 50);
	//sphere 2
		glColor3f(0.0, 0.7, 0.0);
		glTranslatef(0,0,4);
		glutSolidSphere(0.5f, 50, 50);
	//sphere 3
		glColor3f(0.0, 0.0, 0.7);
		glTranslatef(0,0,2);
		glutSolidSphere(0.5f, 50, 50);
	glPopMatrix();
	glFlush();
	return;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	drawSphere();


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
	
	}

	return;
}

int main(int argc, char** argv){


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Homework 1 : Picking, 2016-21191");
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse); 
	glutReshapeFunc(resize);
	glutMainLoop();
	return 0;
}
