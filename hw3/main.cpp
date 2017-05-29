#include "header.h"

using namespace std;


OBJECT teapot;
/*--------------------------------------------
advanced graphics hw3
glsl shading & teapot objloader
applyed trackball

yongsung kim
2017-05-29
---------------------------------------------*/

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
void reshape(GLint w, GLint h){	camera.resize(w,h); }

void mouse(int button, int state, int x, int y){

	if(state == GLUT_UP){
	mouseMovePressed = false;
	mouseRotatePressed = false;
	mouseZoomPressed = false;
	}
	else{
		if( button == GLUT_LEFT && GLUT_ACTIVE_SHIFT == glutGetModifiers()){
			lastX = x;
			lastY = y;
			mouseMovePressed = true;
			mouseRotatePressed = false;
			mouseZoomPressed = false;
		}
		else if( button ==GLUT_LEFT_BUTTON && GLUT_ACTIVE_CTRL == glutGetModifiers()){
			lastZoom = y;
			mouseMovePressed = false;
			mouseRotatePressed = false;
			mouseZoomPressed = true;
		}
		else if( button == GLUT_LEFT_BUTTON){
			camera.beginRotate(x,y);
			mouseMovePressed = false;
			mouseRotatePressed = true;
			mouseZoomPressed = false;
		}

	}
	glutPostRedisplay();
	return;
}
void motion(int x, int y){
	if(mouseRotatePressed == true) camera.rotate(x,y);
	else if( mouseMovePressed == true) {
		camera.move((x - lastX) / static_cast<float>(width),
					(lastY - y) / static_cast<float>(height), 0.0);
		lastX = x;
		lastY = y;
	}
	else if( mouseZoomPressed ==true){
		camera.zoom(float(y - lastZoom) / height);
		lastZoom = y;
	}

	glutPostRedisplay();
	return;
}

	
void display(){

	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.apply();

	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glRotatef(180,0,1,0);
	glRotatef(-90,1,0,0);
	glRotatef(90,0,0,1);
	drawXYZ(100);
	drawFloor(10);
	glPopMatrix();
	drawOBJ(teapot);
	glFlush();	
	glutSwapBuffers();
	return;
}
void idle(){ glutPostRedisplay(); }

int main(int argc, char *argv[]){



	teapot.loadOBJFile("teapot.obj");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(WINX,WINY);
	glutCreateWindow("systemProgramming hw2 201621191 kys");

	camera.resize(width, height);
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}

