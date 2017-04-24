#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "header.h"


#define WIDTH 1200
#define HEIGHT 800



using namespace std;

OBJECT* bunny;
VECTOR3D norm[4968*10];
int SCALE =5;
int rotation = 0;
GLuint fbo;
GLuint depthBuffer;
GLuint img;

void getNormal(){
	int v1, v2, v3;
	float u[3];
	float v[3];

	float len;
	int* tmp;

	tmp = (int*)malloc(sizeof(int)*bunny->vertexNum);

	for(int i=0; i < bunny->vertexNum; i++) tmp[i] =0;

	for(int i=0; i < bunny->faceNum; i++){
		v1 = bunny->F[i][0];
		v2 = bunny->F[i][1];
		v3 = bunny->F[i][2];

		float n[3];

		u[0] = bunny->V[v2][0] - bunny->V[v1][0];
		u[1] = bunny->V[v2][1] - bunny->V[v1][1];
		u[2] = bunny->V[v2][2] - bunny->V[v1][2];


		v[0] = bunny->V[v3][0] - bunny->V[v1][0];
		v[1] = bunny->V[v3][1] - bunny->V[v1][1];
		v[2] = bunny->V[v3][2] - bunny->V[v1][2];

		n[0] = u[1] * v[2] - v[1] * u[2];
		n[1] = u[0] * v[2] - v[0] * u[2];
		n[2] = u[0] * v[1] - v[0] * u[1];
	
		len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= len;
		n[1] /= len;
		n[2] /= len;

		norm[v1].x +=n[0];
		norm[v1].y +=n[1];
		norm[v1].z +=n[2];
//
		u[0] = bunny->V[v3][0] - bunny->V[v2][0];
		u[1] = bunny->V[v3][1] - bunny->V[v2][1];
		u[2] = bunny->V[v3][2] - bunny->V[v2][2];


		v[0] = bunny->V[v1][0] - bunny->V[v2][0];
		v[1] = bunny->V[v1][1] - bunny->V[v2][1];
		v[2] = bunny->V[v1][2] - bunny->V[v2][2];

		n[0] = u[1] * v[2] - v[1] * u[2];
		n[1] = u[0] * v[2] - v[0] * u[2];
		n[2] = u[0] * v[1] - v[0] * u[1];
	
		len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= len;
		n[1] /= len;
		n[2] /= len;
	
		norm[v2].x +=n[0];
		norm[v2].y +=n[1];
		norm[v2].z +=n[2];
//

		u[0] = bunny->V[v1][0] - bunny->V[v3][0];
		u[1] = bunny->V[v1][1] - bunny->V[v3][1];
		u[2] = bunny->V[v1][2] - bunny->V[v3][2];


		v[0] = bunny->V[v2][0] - bunny->V[v3][0];
		v[1] = bunny->V[v2][1] - bunny->V[v3][1];
		v[2] = bunny->V[v2][2] - bunny->V[v3][2];

		n[0] = u[1] * v[2] - v[1] * u[2];
		n[1] = u[0] * v[2] - v[0] * u[2];
		n[2] = u[0] * v[1] - v[0] * u[1];
	
		len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= len;
		n[1] /= len;
		n[2] /= len;
	
		norm[v3].x +=n[0];
		norm[v3].y +=n[1];
		norm[v3].z +=n[2];

		tmp[v1] +=1;
		tmp[v2] +=1;
		tmp[v3] +=1;
	
	}
	for (int i=0; i < bunny->vertexNum; i++){
		norm[i].x /= tmp[i];
		norm[i].y /= tmp[i];
		norm[i].z /= tmp[i];
	}

	return;

}
int ReadFile(const char* filename){
	FILE *fp;
	char data[200];
	char ch;
	char a[40], b[40], c[40];
	int vertexNum_, faceNum_;
	vertexNum_ = faceNum_ = 0;

//--INIT-------------------
	bunny = (OBJECT*)malloc(sizeof(OBJECT));
	
	bunny->V = (float**)malloc(sizeof(float*)*2503);
	for(int i =0; i < 2503; i++){
		bunny->V[i] = (float*)malloc(sizeof(float)*3);
	}

	bunny->F = (int**)malloc(sizeof(int*)*4968);
	for(int i =0; i < 4968; i++){
		bunny->F[i] = (int*)malloc(sizeof(int)*3);
	}
//-------------------------


	if(!(fp = fopen(filename, "r"))){ return -1;}

	while(fgets(data, 200, fp)){
	
		if(data[0] == '#'){continue;}
		else if( data[0] == 'v'){
			sscanf(data, "%c %s %s %s", &ch, a,b,c);
			bunny->V[vertexNum_][0] = atof(a)*SCALE;
			bunny->V[vertexNum_][1] = atof(b)*SCALE;
			bunny->V[vertexNum_][2] = atof(c)*SCALE;
			vertexNum_++;
		}
		else if( data[0]=='f'){
			
			sscanf(data, "%c %s %s %s", &ch, a,b,c);
			bunny->F[faceNum_][0] = atof(a) -1;
			bunny->F[faceNum_][1] = atof(b) -1;
			bunny->F[faceNum_][2] = atof(c) -1;
			faceNum_++;
		}
	}
	bunny->vertexNum = vertexNum_;
	bunny->faceNum = faceNum_;

	getNormal();	
	fclose(fp);
	return 1;
}
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
void display(){

/*
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,WIDTH,HEIGHT);

	glClearColor(0.0f,0.0f,0.0f,0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBegin(GL_QUADS);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();

	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
*/

	rotation = (rotation+1)%360;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

// texture
//	glGenTextures(1, &img);
//	glBindTexture(GL_TEXTURE_2D, img);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, img, 0);

	//GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	//glEnable(GL_TEXTURE_2D);
	//glViewport(0,0,WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glRotatef((GLfloat)rotation, 0.0, 1.0, 0.0);	
	glColor3f(1.0,0.0,0.0);
	for(int i=0; i < bunny->faceNum; i++){
		glBegin(GL_POLYGON);
			glVertex3f(bunny->V[bunny->F[i][0]][0], bunny->V[bunny->F[i][0]][1], bunny->V[bunny->F[i][0]][2]);
			glVertex3f(bunny->V[bunny->F[i][1]][0], bunny->V[bunny->F[i][1]][1], bunny->V[bunny->F[i][1]][2]);
			glVertex3f(bunny->V[bunny->F[i][2]][0], bunny->V[bunny->F[i][2]][1], bunny->V[bunny->F[i][2]][2]);
			//glVertex3f(bunny->V[bunny->F[i][0]][0], bunny->V[bunny->F[i][0]][1], bunny->V[bunny->F[i][0]][2]);
		glEnd();
	}

	glPopMatrix();
	glutSwapBuffers();
	return;
}


int main(int argc, char *argv[]){

	glGenFramebuffersEXT(1,&fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	ReadFile("bunny.obj");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("systemProgramming hw2 201621191 kys");
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}

