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
float rotation = 0;
float depths[WIDTH*HEIGHT];
GLuint fbo=0;
GLuint depthBuffer;
GLuint img;
GLuint fbo1=0;
GLuint depthBuffer1;
GLuint img1;


void drawCube(int flag){

	glBegin(GL_QUADS);
		if(flag == 1){
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,-0.5, 0.5);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f,-0.5, 0.5);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f,0.5, 0.5);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5,0.5, 0.5);
		
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,-0.5, -0.5);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,0.5, -0.5);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f,0.5, -0.5);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f,-0.5, -0.5);
		
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,0.5, -0.5);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,0.5, 0.5);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f,0.5, 0.5);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f,0.5, -0.5);
		}
		else if( flag ==2){
			glNormal3f(0.0f, -1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,-0.5, -0.5);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f,-0.5, -0.5);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f,-0.5, 0.5);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,-0.5, 0.5);

			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f,-0.5, -0.5);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f,0.5, -0.5);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f,0.5, 0.5);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f,-0.5, 0.5);

			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,-0.5, -0.5);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,-0.5, 0.5);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,0.5, 0.5);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,0.5, -0.5);
		}
	glEnd();
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(-0.5f,-0.5, -0.5);
		glVertex3f(-0.5f,-0.5, 0.5);
		
		glVertex3f(-0.5f,-0.5, -0.5);
		glVertex3f(-0.5f,0.5, -0.5);
		
		glVertex3f(-0.5f,-0.5, -0.5);
		glVertex3f(0.5f,-0.5, -0.5);

		glVertex3f(-0.5f,-0.5, 0.5);
		glVertex3f(0.5f,-0.5, 0.5);

		glVertex3f(-0.5f,-0.5, 0.5);
		glVertex3f(-0.5f,0.5, 0.5);
		
		glVertex3f(0.5f,-0.5, 0.5);
		glVertex3f(0.5f,0.5, 0.5);

		glVertex3f(0.5f,-0.5, 0.5);
		glVertex3f(0.5f,-0.5, -0.5);


		glVertex3f(0.5f,0.5, 0.5);
		glVertex3f(0.5f,-0.5, 0.5);

		glVertex3f(0.5f,0.5, 0.5);
		glVertex3f(0.5f,0.5, -0.5);


		glVertex3f(0.5f,-0.5, 0.5);
		glVertex3f(0.5f,-0.5, -0.5);

		glVertex3f(0.5f,-0.5, -0.5);
		glVertex3f(0.5f,0.5, -0.5);

		glVertex3f(0.5f,0.5, -0.5);
		glVertex3f(-0.5f,0.5, -0.5);
	glEnd();

	return;
}
void drawSmallCube(){

	glBegin(GL_QUADS);
		glColor4f(0.0f,1.0f,0.0f,1.0f);
		glVertex3f(-0.5f,-0.5, 0.5);
		glVertex3f(0.5f,-0.5, 0.5);
		glVertex3f(0.5f,0.5, 0.5);
		glVertex3f(-0.5f,-0.5, 0.5);
		
		glColor4f(1.0f,1.0f,0.0f,1.0f);
		glVertex3f(-0.5f,-0.5, -0.5);
		glVertex3f(-0.5f,0.5, -0.5);
		glVertex3f(0.5f,0.5, -0.5);
		glVertex3f(0.5f,-0.5, -0.5);
		
		glColor4f(0.0f,0.0f,1.0f,1.0f);
		glVertex3f(-0.5f,0.5, -0.5);
		glVertex3f(-0.5f,0.5, 0.5);
		glVertex3f(0.5f,0.5, 0.5);
		glVertex3f(0.5f,0.5, -0.5);
		
		glColor4f(0.0f,0.0f,1.0f,1.0f);
		glVertex3f(-0.5f,-0.5, -0.5);
		glVertex3f(0.5f,-0.5, -0.5);
		glVertex3f(0.5f,-0.5, 0.5);
		glVertex3f(-0.5f,-0.5, 0.5);

		glColor4f(0.0f,1.0f,1.0f,1.0f);
		glVertex3f(0.5f,-0.5, -0.5);
		glVertex3f(0.5f,0.5, -0.5);
		glVertex3f(0.5f,0.5, 0.5);
		glVertex3f(0.5f,-0.5, 0.5);

		glColor4f(1.0f,0.0f,1.0f,1.0f);
		glVertex3f(-0.5f,-0.5, -0.5);
		glVertex3f(-0.5f,-0.5, 0.5);
		glVertex3f(-0.5f,0.5, 0.5);
		glVertex3f(-0.5f,0.5, -0.5);
	glEnd();

	return;
}
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
			bunny->V[vertexNum_][1] = atof(b)*SCALE-0.4;
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
void initFBO(){
	
	GLenum err = glewInit();
	if(GLEW_OK != err){
		cout << glewGetErrorString(err) << endl;
	}


	
	glGenFramebuffersEXT(1,&fbo);
	glGenFramebuffersEXT(1,&fbo1);

	
	glGenRenderbuffersEXT(1,&depthBuffer);
	glGenRenderbuffersEXT(1,&depthBuffer1);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo1);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer1);

	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, WIDTH, HEIGHT);

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer1);



	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
		cout << "not working!"<< endl;
	}


	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo1);

	glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glGenTextures(1, &img1);
	//glBindTexture(GL_TEXTURE_2D, img1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, img, 0);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, img1, 0);

	return;
}
void deleteFBO(){

	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &fbo1);
	return;
}
void display(){


	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, img, 0);

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,WIDTH,HEIGHT);


	glClearColor(0.7f,0.7f,0.f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	//drawSmallCube();
	glPushMatrix();
	glRotatef((GLfloat)rotation*2, 1.0, 1.0, 1.0);
	//glColor3f(0.5f, 0.5f,0.0);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glColor4f(0.3f,0.3f,0.3f,0.3f);
	for(int i=0; i < bunny->faceNum; i++){
		glBegin(GL_POLYGON);
			glNormal3f(norm[bunny->F[i][0]].x, norm[bunny->F[i][0]].y , norm[bunny->F[i][0]].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(bunny->V[bunny->F[i][0]][0], bunny->V[bunny->F[i][0]][1], bunny->V[bunny->F[i][0]][2]);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(bunny->V[bunny->F[i][1]][0], bunny->V[bunny->F[i][1]][1], bunny->V[bunny->F[i][1]][2]);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(bunny->V[bunny->F[i][2]][0], bunny->V[bunny->F[i][2]][1], bunny->V[bunny->F[i][2]][2]);
		glEnd();
	}
	glDisable(GL_BLEND);
	glPopMatrix();
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

//--------------------------
	if ( rotation < 360.0) { rotation += 0.1;}
	else{ rotation =0;}
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0,1.0,0.0);
	glPushMatrix();
	glRotatef((GLfloat)rotation, 1.0, 1.0, 1.0);	
	drawCube(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


//second
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo1);
	//glBindTexture(GL_TEXTURE_2D, img1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, img1, 0);

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,WIDTH,HEIGHT);


	glClearColor(0.3f,0.7f,0.0f,0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	//drawSmallCube();
	glPushMatrix();
	glRotatef((GLfloat)rotation*2, 1.0, 1.0, 1.0);
	//glColor3f(0.5f, 0.5f,0.0);
	glColor4f(0.7f,0.0f,0.7f,1.0f);
	for(int i=0; i < bunny->faceNum; i++){
		glBegin(GL_POLYGON);
			glNormal3f(norm[bunny->F[i][0]].x, norm[bunny->F[i][0]].y , norm[bunny->F[i][0]].z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(bunny->V[bunny->F[i][0]][0], bunny->V[bunny->F[i][0]][1], bunny->V[bunny->F[i][0]][2]);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(bunny->V[bunny->F[i][1]][0], bunny->V[bunny->F[i][1]][1], bunny->V[bunny->F[i][1]][2]);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(bunny->V[bunny->F[i][2]][0], bunny->V[bunny->F[i][2]][1], bunny->V[bunny->F[i][2]][2]);
		glEnd();
	}
	glPopMatrix();
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glBindTexture();




	

// texture


	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0,1.0,0.0);
	glPushMatrix();
	glRotatef((GLfloat)rotation, 1.0, 1.0, 1.0);
	drawCube(2);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	
	glutSwapBuffers();
	//deleteFBO();
	return;
}


int main(int argc, char *argv[]){


	ReadFile("bunny.obj");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("systemProgramming hw2 201621191 kys");
	glewInit();
	initFBO();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMainLoop();
	deleteFBO();
	return 0;
}

