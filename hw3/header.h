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

//------ GLSL------

static GLuint program = 0, vertShader = 0, fragShader = 0;
static bool enableProgram = false;
static bool enableStripe = false;
static float freq = 10.f;


static GLfloat lightPos[4] = {-1.f, 1.f, 0.f, 0.f};
static GLfloat lightPos1[4] = {3.f, 3.f, 3.f, 1.f};
static GLfloat lightPos2[4] = {2.f, 1.f, 2.f, 1.f};

static GLfloat lightKa[4] = {0.5f, 0.f, 0.f, 1.f};
static GLfloat lightKd[4] = {0.5f, 0.5f, 0.f, 1.f};
static GLfloat lightKs[4] = {1.f, 1.f, 1.f, 1.f};
static GLfloat matKs[4] = {1.f, 1.f, 1.f, 1.f};
static GLfloat matShininess = 50.0f;

static bool light0 = false, light1 = false, light2 = false;

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
		for(int i=0; i < obj.getFaceSize(); i++){
			int idx1 = obj.face(i,0);
			int idx2 = obj.face(i,1);
			int idx3 = obj.face(i,2);
			int idx4 = obj.face(i,3);
			int idx5 = obj.face(i,4);
			int idx6 = obj.face(i,5);
			
			glBegin(GL_POLYGON);
			glColor3f(0.f,0.f,0.5);
				glNormal3f(obj.vertexNorm(idx4,0), obj.vertexNorm(idx4,1), obj.vertexNorm(idx4,2));
				glVertex3f(obj.vertex(idx1,0),obj.vertex(idx1,1), obj.vertex(idx1,2));
				
				glNormal3f(obj.vertexNorm(idx5,0), obj.vertexNorm(idx5,1), obj.vertexNorm(idx5,2));
				glVertex3f(obj.vertex(idx2,0),obj.vertex(idx2,1), obj.vertex(idx2,2));
				
				glNormal3f(obj.vertexNorm(idx5,0), obj.vertexNorm(idx5,1), obj.vertexNorm(idx5,2));
				glVertex3f(obj.vertex(idx3,0),obj.vertex(idx3,1), obj.vertex(idx3,2));
				
				//glVertex3f(obj.vertex(idx1,0),obj.vertex(idx1,1), obj.vertex(idx1,2));
			glEnd();
		}
	glPopMatrix();
	return;
}

void drawFloor(int size){

	glPushMatrix();
	glColor3f(0.f,0.65,0.65);
	float stepSize = 0.01;

	for(int i= -size; i <= size; i++){
		glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(stepSize*(float)i, 0, 0);
			glVertex3f(stepSize*(float)i, (float)size*stepSize, 0);

			glVertex3f(0, stepSize*(float)i, 0);
			glVertex3f((float)size*stepSize, stepSize*(float)i, 0);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(stepSize*(float)i, 0, 0);
			glVertex3f(stepSize*(float)i, -(float)size*stepSize, 0);

			glVertex3f(0, stepSize*(float)i, 0);
			glVertex3f(-(float)size*stepSize, stepSize*(float)i, 0);
		glEnd();

	}
	glPopMatrix();
	return;
}
void drawLightPos(){
	glPushMatrix();
		glPointSize(10);
		glBegin(GL_POINTS);
		glColor3f(1.0,0,0);
			glVertex3f(lightPos[0], lightPos[1], lightPos[2]);
			glVertex3f(lightPos1[0], lightPos1[1], lightPos1[2]);
			glVertex3f(lightPos2[0], lightPos2[1], lightPos2[2]);
		glEnd();
	glPopMatrix();	

	return;
}




// ----------__GLSL_____----------------------
void printShaderInfoLog(GLuint obj){

	int len = 0, charsWritten = 0;
	char* infoLog;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &len);
	if(len > 0){
		infoLog = (char*)malloc(len);
		glGetShaderInfoLog(obj, len, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}

	return;
}
void printProgramInfoLog(GLuint obj){

	int len = 0, charsWritten = 0;
	char* infoLog;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &len);
	if(len > 0){
		infoLog = (char*)malloc(len);
		glGetProgramInfoLog(obj, len, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}

	return;
}
char* readFromFile(const char* filename){
	
	FILE* fp = fopen(filename, "rt");
	if(fp != NULL){
		fseek(fp, 0, SEEK_END);
		size_t count = ftell(fp);
		rewind(fp);
		if( count > 0){
			char* text = (char*)malloc(count+1);
			count  = (int)fread(text, 1, count, fp);
			text[count] = '\0';
			fclose(fp);
			return text;			
		}else fclose(fp);
	}
	return NULL;
}
GLuint createShader(const char* src, GLenum type){

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	printShaderInfoLog(shader);
	return shader;
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
void createProgram(){
	char* vert = readFromFile("phong.vert");
	char* frag = readFromFile("phnog.frag");
	
	vertShader = createShader(vert,GL_VERTEX_SHADER);
	fragShader = createShader(frag, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	printProgramInfoLog(program);
	free(vert);
	free(frag);
	return;
}
void cleanUp(){
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(program);
	program = vertShader = fragShader = 0;
	return;
}

void setLight(){


	if(light0){
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	}
	else glDisable(GL_LIGHT0);
	if(light1){ // set point light
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightKs);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	} else glDisable(GL_LIGHT1);
	if(light2){
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightKs);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
	} else glDisable(GL_LIGHT2);

	return;
}
void lightOnOffMsg(bool flag, unsigned char idx){
	
	if(flag) cout << "light" <<idx << " ON" << endl;
	else cout << "light" << idx << " OFF" << endl;
	return;
}
