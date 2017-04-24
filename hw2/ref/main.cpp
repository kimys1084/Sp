//#include <Windows.h>
/*#include <GL/freeglut.h>
#include <GL/glut.h>

#include "viewport.h"

#include <algorithm>

#include <stdio.h>
#include <vector>
#include "texture.h"*/
#include <algorithm>
#include <GL/freeglut.h>
#include "texture.h"
#include <windows.h>
#include "viewport.h"
#include "curve.h"
#include <vector>
#include <iostream>
#define M_PI 3.14159265358979323846
#define MAX_PTR 10000
#define SCALE 1500
#define MAX_DISTANCE 100000
#define RES 32

BicubicBezierSurface surface;
GLsizei width = 800, height = 600;
float viewportwidth = 400, viewportheight = 300;
Vector3d norm[MAX_PTR * 10];
int norm_cnt[MAX_PTR*10];
int selectedscene = 0;
int selected = -1;
bool isDrawControlMesh = true;
bool isDottedLine = false;
bool rotation = false;
const int OOCP = 3;	
//const int RES = 128;	
const int DIM = 3;
Vector3d eye;
Vector3d center;
Vector3d upVector;
Vector3d BunnyCenter;
Vector3d bunny[MAX_PTR];
Vector3d bunny_backup[MAX_PTR];
Vector3d mworld[RES + 1][RES + 1][3];
face f[MAX_PTR];

bool isDragging = false;
float radius;
int v_cnt;
int f_cnt;
bool spaceflag = false;
float points[RES + 1][RES + 1][3];
bool initflag = false;
int mouseButton = -1;
int lastX = -1;
int lastY = -1;
double texCoords[MAX_PTR + 1][MAX_PTR + 1][2];
double surfaceNormals[RES + 1][RES + 1][DIM];
GLuint tex;
Point target;

void RayTest(int mouse_x, int mouse_y)
{
	float x = mouse_x;
	float y = height - mouse_y;

	double model[16], proj[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	int viewport[4] = { 0.0f, 0.0f, width, height };
	double ax, ay, az, bx, by, bz;
	gluUnProject(mouse_x, mouse_y, 0.0, model, proj, viewport, &ax, &ay, &az);
	gluUnProject(mouse_x, mouse_y, 1.0, model, proj, viewport, &bx, &by, &bz);

	double cx, cy, cz;
	cx = ax - bx;
	cy = ay - by;
	cz = az - bz;

	target[0] = ax - cx * ay / cy;
	target[1] = az - cz * ay / cy;
}
void getCoG(Point *cog, int idx){
	Point a, b, c;
	int idx1 = f[idx][1];
	int idx2 = f[idx][1];
	int idx3 = f[idx][2];
	SET_PT3(a, bunny[idx1].x, bunny[idx1].y, bunny[idx1].z);
	SET_PT3(b, bunny[idx2].x, bunny[idx2].y, bunny[idx2].z);
	SET_PT3(c, bunny[idx3].x, bunny[idx3].y, bunny[idx3].z);
	(*cog)[0] = (a[0] + b[0] + c[0]) / 3;
	(*cog)[1] = (a[1] + b[1] + c[1]) / 3;
	(*cog)[2] = (a[2] + b[2] + c[2]) / 3;
	return;
}
void NormalV(Vector3d *result, int idx1, int idx2, int idx3){
	Point a, b, c;

	SET_PT3(a, bunny[idx1].x, bunny[idx1].y, bunny[idx1].z);
	SET_PT3(b, bunny[idx2].x, bunny[idx2].y, bunny[idx2].z);
	SET_PT3(c, bunny[idx3].x, bunny[idx3].y, bunny[idx3].z);

	Vector3d x1(b[0] - a[0], b[1] - a[1], b[2] - a[2]);
	Vector3d y1(c[0] - a[0], c[1] - a[1], c[2] - a[2]);
	
	(*result).cross(y1, x1);
	(*result).normalize();
	
	return;
}

int hit_index(int x, int y, int scene)
{
	int xx, yy;
	switch (scene)
	{
	case 1:
		xx = 0, yy = 1;
		break;
	case 3:
		xx = 0, yy = 2;
		break;
	case 4:
		xx = 1, yy = 2;
		break;
	}
	int min = 30;
	int minp = -1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float tx = surface.control_pts[i][j][xx] - x;
			float ty = surface.control_pts[i][j][yy] - y;
			if ((tx * tx + ty * ty) < min)
			{
				min = (tx * tx + ty * ty);
				minp = i * 10 + j;
			}
		}
	}
	return minp;
}
void getNormalV(){
	int i, j;

	for (i = 0; i < RES; i++){
		for (j = 0; j < RES; j++){
			Vector3d _o(points[i][j][0], points[i][j][1], points[i][j][2]);
			Vector3d _l(points[i][j + 1][0], points[i][j + 1][1], points[i][j + 1][2]);
			Vector3d _r(points[i + 1][j][0], points[i + 1][j][1], points[i + 1][j][2]);
			Vector3d lv, rv;
			

			lv.sub(_l, _o);
			mworld[i][j][1].set(lv.x, lv.y, lv.z);
			rv.sub(_r, _o);
			mworld[i][j][0].cross(lv, rv);
			mworld[i][j][2].cross(mworld[i][j][1], mworld[i][j][0]);
		
			mworld[i][j][0].normalize();
			mworld[i][j][1].normalize();
			mworld[i][j][2].normalize();
			
		}
	}
	for (i = 0; i < RES; i++){
		Vector3d _o(points[i][RES][0], points[i][RES][1], points[i][RES][2]);
		Vector3d _l(points[i][RES - 1][0], points[i][RES - 1][1], points[i][RES - 1][2]);
		Vector3d _r(points[i + 1][RES][0], points[i + 1][RES][1], points[i + 1][RES][2]);
		Vector3d lv, rv;
		

		lv.sub(_l, _o);
		rv.sub(_r, _o);
		
		mworld[i][RES][0].cross(rv, lv);
		mworld[i][RES][1].set(lv.x, lv.y, lv.z);
		mworld[i][RES][2].cross(mworld[i][RES][1], mworld[i][RES][0]);
		//mworld[i][RES][0].cross(mworld[i][RES][1], mworld[i][RES][2]);
		mworld[i][RES][0].normalize();
		mworld[i][RES][1].normalize();
		mworld[i][RES][2].normalize();


	}
	for (j = 0; j < RES; j++){
		Vector3d _o(points[RES][j][0], points[RES][j][1], points[RES][j][2]);
		Vector3d _l(points[RES][j + 1][0], points[RES][j + 1][1], points[RES][j + 1][2]);
		Vector3d _r(points[RES-1][j][0], points[RES-1][j][1], points[RES-1][j][2]);
		Vector3d lv, rv;
		
		lv.sub(_l, _o);
		rv.sub(_r, _o);

		mworld[RES][j][1].set(lv.x, lv.y, lv.z);
		mworld[RES][j][0].cross(rv, lv);
		mworld[RES][j][2].cross(mworld[RES][j][1], mworld[RES][j][0]);
		
		mworld[RES][j][0].normalize();
		mworld[RES][j][1].normalize();
		mworld[RES][j][2].normalize();


	}
	Vector3d _o(points[RES][RES][0], points[RES][RES][1], points[RES][RES][2]);
	Vector3d _l(points[RES][RES - 1][0], points[RES][RES - 1][1], points[RES][RES - 1][2]);
	Vector3d _r(points[RES - 1][RES][0], points[RES - 1][RES][1], points[RES - 1][RES][2]);
	Vector3d lv, rv;
	

	lv.sub(_l, _o);
	rv.sub(_r, _o);
	mworld[RES][RES][1].set(lv.x, lv.y, lv.z);
	mworld[RES][RES][0].cross(lv, rv);
	mworld[RES][RES][2].cross(mworld[RES][RES][1], mworld[RES][RES][0]);

	mworld[RES][RES][0].normalize();
	mworld[RES][RES][1].normalize();
	mworld[RES][RES][2].normalize();
	return;
}
void calc_surface()
{
	for (int i = 0; i <= RES; i++)
	for (int j = 0; j <= RES; j++)
	{
		evaluate(&surface, i / (float)RES, j / (float)RES, points[i][j]);
	}

}
void FindShortest(){
	int i, j, k;
	double min = MAX_DISTANCE;
	double dis;
	int minj, mink;
	getNormalV(); // model coordinate

	for (i = 0; i < v_cnt; i++){
		for (j = 0; j <= RES; j++){
			for (k = 0; k <= RES; k++){
				dis = bunny[i].distancef(points[j][k][0], points[j][k][1], points[j][k][2]);
				if (dis < min){
					min = dis;
					minj = j;
					mink = k;
				}
			}
		}
		bunny[i].i = minj;
		bunny[i].j = mink;
		min = MAX_DISTANCE;
		float dot_t = mworld[minj][mink][0].dot(mworld[minj][mink][1]);
	
		Vector3d o,s;
		o.set(points[minj][mink][0], points[minj][mink][1], points[minj][mink][2]);
		s.sub(bunny[i],o); // b - o
		bunny[i].mulM(mworld[minj][mink][0], mworld[minj][mink][1], mworld[minj][mink][2],s);
	}
	return;
}
void rotate(double theta){
	double x, z;
	for (int i = 0; i < v_cnt; i++){
		bunny[i].x -= BunnyCenter.x;
		bunny[i].z -= BunnyCenter.z;

		x = bunny[i].x;
		z = bunny[i].z;

		bunny[i].x = x * cos(theta) + z*sin(theta);
		bunny[i].z = -x * sin(theta) + z*cos(theta);

		bunny[i].x += BunnyCenter.x;
		bunny[i].z += BunnyCenter.z;
	}

	return;
}
void getCenter(){

	double x = 0, y=0, z=0;
	for (int i = 0; i < v_cnt; i++){
		x += bunny[i].x;
		y += bunny[i].y;
		z += bunny[i].z;
	}
	BunnyCenter.x = x / v_cnt;
	BunnyCenter.y = y / v_cnt;
	BunnyCenter.z = z / v_cnt;
	return;
}
void init()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			SET_PT3(surface.control_pts[i][j], 50 * i + 50, 20 * i - 75 * (i == 2) + 200 - j * 50, j * 50 + 50);

	calc_surface();
	getNormalV();
	for (int i = 0; i < v_cnt; i++){
		bunny[i].set(bunny_backup[i].x, bunny_backup[i].y, bunny_backup[i].z);
		bunny[i].setvec(0, 0, 0);
	}
	FindShortest();
	getCenter();
	
	int width, height;
	initPNG(&tex, "parabolic.png", width, height);

	eye = Vector3d(750, 750, 750);
	center = Vector3d(0, 0, 0);
	upVector = Vector3d(0, 1, 0);
	initflag = true;
}

void reshape_callback(GLint nw, GLint nh)
{
	width = nw;
	height = nh;
	viewportwidth = width / 2.0f;
	viewportheight = height / 2.0f;
	radius = std::sqrt(viewportwidth * viewportwidth + viewportheight * viewportheight) / 2;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void DrawSurface1()
{
	for (int i = 0; i < f_cnt; i++)
	for (int j = 0; j <= 2; j++)
	{
		// calculating reflecting vector
		Vector3d mapped;
		mapped.sub(center, eye);
		//mapped.sub(center, eye);
		//Vector3d n = Vector3d(mworld[i][j][0].x, mworld[i][j][0].y, mworld[i][j][0].z);
		Vector3d n = Vector3d(norm[f[i][j]].x, norm[f[i][j]].y, norm[f[i][j]].z);
		
		n.scale(-2 * n.dot(mapped));
		mapped.add(n);
		mapped.normalize();

		// for sphere mapping
		double sum = sqrt(mapped.x*mapped.x + mapped.y*mapped.y + pow(mapped.z + 1, 2));
		texCoords[i][j][0] = (mapped.x / sum + 1) / 2;
		texCoords[i][j][1] = (-mapped.y / sum + 1) / 2;
		
		
	}

	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < f_cnt; i++)
	{
		glBegin(GL_POLYGON);
		//printf("%d %d %d\n", f[i][0], f[i][1], f[i][2]);
			//printf("%f %f\n", texCoords[i][0][0], texCoords[i][0][1]);
			glTexCoord2dv(texCoords[i][0]);
			glVertex3f(bunny[f[i][0]].x, bunny[f[i][0]].y, bunny[f[i][0]].z);
			glTexCoord2dv(texCoords[i][1]);
			glVertex3f(bunny[f[i][1]].x, bunny[f[i][1]].y, bunny[f[i][1]].z);
			glTexCoord2dv(texCoords[i][2]);
			glVertex3f(bunny[f[i][2]].x, bunny[f[i][2]].y, bunny[f[i][2]].z);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	
}
void display_callback()
{
	static double x = 750;
	static double z = 750;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-1, 0, 0); // 가로 경계선
	glVertex3f(1, 0, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0, -1, 0); // 세로 경계선
	glVertex3f(0, 1, 0);
	glEnd();
	
	
	
	glPointSize(8.0f);
	// XY
	glViewport(0, viewportheight, viewportwidth, viewportheight);
	glLoadIdentity();
	gluOrtho2D(0, (double)viewportwidth, 0, (double)viewportheight);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++) // 점
		for (int j = 0; j < 4; j++)
			glVertex2f(surface.control_pts[i][j][0], surface.control_pts[i][j][1]);
	glEnd();
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)  // 점사이 선
		for (int j = 0; j < 3; j++)
		{
			glVertex2f(surface.control_pts[i][j][0], surface.control_pts[i][j][1]); // 세로
			glVertex2f(surface.control_pts[i][j + 1][0], surface.control_pts[i][j + 1][1]);
			glVertex2f(surface.control_pts[j][i][0], surface.control_pts[j][i][1]); // 가로
			glVertex2f(surface.control_pts[j + 1][i][0], surface.control_pts[j + 1][i][1]);
		}
	glEnd();
	
	// XZ
	glViewport(0, 0, viewportwidth, viewportheight);
	glLoadIdentity();
	gluOrtho2D(0, (double)viewportwidth, 0, (double)viewportheight);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			glVertex2f(surface.control_pts[i][j][0], surface.control_pts[i][j][2]);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			glVertex2f(surface.control_pts[i][j][0], surface.control_pts[i][j][2]);
			glVertex2f(surface.control_pts[i][j + 1][0], surface.control_pts[i][j + 1][2]);
			glVertex2f(surface.control_pts[j][i][0], surface.control_pts[j][i][2]);
			glVertex2f(surface.control_pts[j + 1][i][0], surface.control_pts[j + 1][i][2]);
		}
	glEnd();

	// YZ
	glViewport(viewportwidth, 0, viewportwidth, viewportheight);
	glLoadIdentity();
	gluOrtho2D(0, (double)viewportwidth, 0, (double)viewportheight);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			glVertex2f(surface.control_pts[i][j][1], surface.control_pts[i][j][2]);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			glVertex2f(surface.control_pts[i][j][1], surface.control_pts[i][j][2]);
			glVertex2f(surface.control_pts[i][j + 1][1], surface.control_pts[i][j + 1][2]);
			glVertex2f(surface.control_pts[j][i][1], surface.control_pts[j][i][2]);
			glVertex2f(surface.control_pts[j + 1][i][1], surface.control_pts[j + 1][i][2]);
		}
	glEnd();

	// 3D
	getCenter();
	glViewport(viewportwidth, viewportheight, viewportwidth, viewportheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(25, width / (double)height, 0.1, 25000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, upVector.x, upVector.y, upVector.z);

	glEnable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0, 0); //  축 그리기
	glVertex3f(0, 0, 0);
	glVertex3f(500.0f, 0, 0);
	glColor3f(0, 1.0f, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 500.0f, 0);
	glColor3f(0, 0, 1.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 500.0f);
	glEnd();

	glColor3f(0, 0, 0);
	
	for (int i = 0; i <= RES; i += 4)
	{
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= RES; j++)
			glVertex3f(points[i][j][0], points[i][j][1], points[i][j][2]);
		glEnd();
	}
	for (int i = 0; i <= RES; i += 4)
	{
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= RES; j++)
			glVertex3f(points[j][i][0], points[j][i][1], points[j][i][2]);
		glEnd();
	}

	
	if (rotation){
		//rotate(-0.03);
		double tempx = x*cos(-0.03) + z*sin(-0.03);
		double tempz = z*cos(-0.03) - x*sin(-0.03);
		eye = Vector3d(tempx, 750, tempz);
		x = tempx;
		z = tempz;
		//FindShortest();
	}
	/*
	glColor3d(255, 0, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < f_cnt; i++) // 면 그리기
	{
	
		glVertex3f(bunny[f[i][0]].x, bunny[f[i][0]].y, bunny[f[i][0]].z);
		glVertex3f(bunny[f[i][1]].x, bunny[f[i][1]].y, bunny[f[i][1]].z);
		glVertex3f(bunny[f[i][2]].x, bunny[f[i][2]].y, bunny[f[i][2]].z);
		glVertex3f(bunny[f[i][0]].x, bunny[f[i][0]].y, bunny[f[i][0]].z);
	}
	glEnd();
	*/
	if (isDottedLine){
		
		for (int i = 0; i < f_cnt; i++) // 면 그리기
		{
			glColor3d(0, 255, 0);
			int mini = bunny[i].i;
			int minj = bunny[i].j;
			glBegin(GL_LINES);
			glVertex3f(points[mini][minj][0], points[mini][minj][1], points[mini][minj][2]);
			glVertex3f(bunny[i].x, bunny[i].y, bunny[i].z);
			glEnd();
		}
		
	}
	glDisable(GL_DEPTH_TEST);
	DrawSurface1();
	
	if (spaceflag){
		
		getNormalV();
		for (int i = 0; i < v_cnt; i++){
			Vector3d _o;
			int j = bunny[i].i;
			int k = bunny[i].j;
			_o.set(points[j][k][0], points[j][k][1], points[j][k][2]);

			bunny[i].GetNewPoint(mworld[j][k][0], mworld[j][k][1], mworld[j][k][2], _o);
		}
	}
	
	glutSwapBuffers();
}

// void glutMouseFunc(void (*func)(int button, int state, int x, int y));
void mouse_callback(GLint button, GLint action, GLint x, GLint y)
{
	int scene = 0;
	if (x < viewportwidth)
	{
		if (y < viewportheight)
			scene = 1;
		else
		{
			scene = 3;
			y -= (int)viewportheight;
		}
	}
	else
	{
		x -= (int)viewportwidth;
		if (y < viewportheight)
			scene = 2;
		else
		{
			scene = 4;
			y -= (int)viewportheight;
		}
	}

	if (action == GLUT_UP)
	{
		isDragging = false;
		mouseButton = -1;
	}

	if (scene == 2) // scene
	{
		if (action == GLUT_DOWN)
		{
			mouseButton = button;
			isDragging = true;
			lastX = x;
			lastY = y;
		}
	}
	else
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			switch (action)
			{
			case GLUT_DOWN:
				selectedscene = scene;
				selected = hit_index(x, (int)viewportheight - y, scene);
				break;
			case GLUT_UP:
				selected = -1;
				break;
			default: break;
			}
		}
	}
	glutPostRedisplay();
}

// void glutMotionFunc(void (*func)(int x, int y));
void mouse_move_callback(GLint x, GLint y)
{
	Vector3d lastP = getMousePoint(lastX, lastY, viewportwidth, viewportheight, radius);
	Vector3d currentP = getMousePoint(x - viewportwidth, y, viewportwidth, viewportheight, radius);

	if (mouseButton == GLUT_LEFT_BUTTON)
	{
		Vector3d rotateVector;
		rotateVector.cross(currentP, lastP);
		double angle = -currentP.angle(lastP) * 2;
		rotateVector = unProjectToEye(rotateVector, eye, center, upVector);

		Vector3d dEye;
		dEye.sub(center, eye);
		dEye = rotate(dEye, rotateVector, -angle);
		upVector = rotate(upVector, rotateVector, -angle);
		eye.sub(center, dEye);
	}
	else if (mouseButton == GLUT_RIGHT_BUTTON){
		Vector3d dEye;
		dEye.sub(center, eye);
		double offset = 0.025;
		if ((y - lastY) < 0){
			dEye.scale(1 - offset);
		}
		else {
			dEye.scale(1 + offset);
		}
		eye.sub(center, dEye);
	}
	else if (mouseButton == GLUT_MIDDLE_BUTTON){
		double dx = x - viewportwidth - lastX;
		double dy = y - lastY;
		if (dx != 0 || dy != 0)
		{
			Vector3d moveVector(dx, dy, 0);
			moveVector = unProjectToEye(moveVector, eye, center, upVector);
			moveVector.normalize();
			double eyeDistance = Vector3d(eye).distance(Vector3d(center));
			moveVector.scale(std::sqrt(dx*dx + dy*dy) / 1000 * eyeDistance);
			center.add(moveVector);
			eye.add(moveVector);
		}
	}

	lastX = x - viewportwidth;
	lastY = y;

	if (selected != -1)
	{
		int xx = 0;
		int yy = 0;
		switch (selectedscene)
		{
		case 1:
			xx = 0, yy = 1;
			break;
		case 3:
			xx = 0, yy = 2;
			y -= (int)viewportheight;
			break;
		case 4:
			xx = 1, yy = 2;
			x -= (int)viewportwidth;
			y -= (int)viewportheight;
			break;
		}
		x = std::max(x, 0);
		x = std::min(x, (int)viewportwidth);
		y = std::max((int)viewportheight - y, 0);
		y = std::min(y, (int)viewportheight);
		surface.control_pts[selected / 10][selected % 10][xx] = static_cast<float>(x);
		surface.control_pts[selected / 10][selected % 10][yy] = static_cast<float>(y);
		calc_surface();
	}

	glutPostRedisplay();
}

// void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void keyboard_callback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'i': case 'I':
		init();
		break;
	case 'l': case 'L':
		isDottedLine ^= true;
		break;
	case 'c': case 'C':
		isDrawControlMesh ^= true;
		break;
	case 'a': case 'A':
		rotation ^= true;
		break;
	case (27) : exit(0); break;
	case (32):
		if (spaceflag){
			spaceflag = false;
		}
		else{ spaceflag = true; FindShortest(); }

	default: break;
	}
	glutPostRedisplay();
}
void getVertexNormal(){

	int v1, v2, v3;

	float u[3];
	float v[3];

	float len;

	int* tmp;
	tmp = (int*)malloc(sizeof(int)*v_cnt);

	for (int i = 0; i < v_cnt; i++) tmp[i] = 0;

	for (int i = 0; i < f_cnt; i++) {

		v1 = f[i][0];
		v2 = f[i][1];
		v3 = f[i][2];

		double n[3];


		u[0] = bunny[v2].x - bunny[v1].x;
		u[1] = bunny[v2].y - bunny[v1].y;
		u[2] = bunny[v2].z - bunny[v1].z;

		v[0] = bunny[v3].x - bunny[v1].x;
		v[1] = bunny[v3].y - bunny[v1].y;
		v[2] = bunny[v3].z - bunny[v1].z;

		n[0] = u[1] * v[2] - v[1] * u[2];
		n[1] = v[0] * u[2] - u[0] * v[2];
		n[2] = u[0] * v[1] - v[0] * u[1];

		len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= len;
		n[1] /= len;
		n[2] /= len;

		norm[v1].x += n[0];
		norm[v1].y += n[1];
		norm[v1].z += n[2];

		//
		u[0] = bunny[v3].x - bunny[v2].x;
		u[1] = bunny[v3].y - bunny[v2].y;
		u[2] = bunny[v3].z - bunny[v2].z;

		v[0] = bunny[v1].x - bunny[v2].x;
		v[1] = bunny[v1].y - bunny[v2].y;
		v[2] = bunny[v1].z - bunny[v2].z;

		n[0] = u[1] * v[2] - v[1] * u[2];
		n[1] = v[0] * u[2] - u[0] * v[2];
		n[2] = u[0] * v[1] - v[0] * u[1];

		len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= len;
		n[1] /= len;
		n[2] /= len;

		norm[v2].x += n[0];
		norm[v2].y += n[1];
		norm[v2].z += n[2];

		//
		u[0] = bunny[v1].x - bunny[v3].x;
		u[1] = bunny[v1].y - bunny[v3].y;
		u[2] = bunny[v1].z - bunny[v3].z;

		v[0] = bunny[v2].x - bunny[v3].x;
		v[1] = bunny[v2].y - bunny[v3].y;
		v[2] = bunny[v2].z - bunny[v3].z;

		n[0] = u[1] * v[2] - v[1] * u[2];
		n[1] = v[0] * u[2] - u[0] * v[2];
		n[2] = u[0] * v[1] - v[0] * u[1];

		len = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= len;
		n[1] /= len;
		n[2] /= len;

		norm[v3].x += n[0];
		norm[v3].y += n[1];
		norm[v3].z += n[2];
		tmp[v1] += 1;
		tmp[v2] += 1;
		tmp[v3] += 1;
	}

	for (int i = 0; i < v_cnt; i++) {
		norm[i].x /= tmp[i];
		norm[i].y /= tmp[i];
		norm[i].z /= tmp[i];
	}

	return ;
}

void get_normalV(){
	for (int i = 0; i < v_cnt; i++){
		norm_cnt[i] = 0;
		norm[i].set(0, 0, 0);
	}
	///*for (int i = 0; i < f_cnt; i++){
	//	Vector3d result;
	//	NormalV(&result, f[i][0],f[i][1],f[i][2]);
	//	norm[f[i][0]].setvec(result.x, result.y, result.z);
	//	NormalV(&result, f[i][1], f[i][2], f[i][0]);
	//	norm[f[i][1]].setvec(result.x, result.y, result.z);
	//	NormalV(&result, f[i][2], f[i][1], f[i][0]);
	//	norm[f[i][2]].setvec(result.x, result.y, result.z);
	//	norm_cnt[f[i][0]]++;
	//	norm_cnt[f[i][1]]++;
	//	norm_cnt[f[i][2]]++;
	//}
	//for (int i = 0; i < f_cnt; i++){
	//	for (int j = 0; j < 3; j++){
	//		norm[f[i][j]].scale(1.0 / norm_cnt[f[i][j]]);
	//		norm[f[i][j]].normalize();
	//	}
	//	
	//	/*
	//	printf("%f %f %f\n", norm[f[i][0]].x, norm[f[i][0]].y, norm[f[i][0]].z);
	//	printf("%f %f %f\n", norm[f[i][1]].x, norm[f[i][1]].y, norm[f[i][1]].z);
	//	printf("%f %f %f\n", norm[f[i][2]].x, norm[f[i][2]].y, norm[f[i][2]].z);*/
	//}


}
int ReadInput(char* filename){
	FILE* fp;
	char data[200];
	char ch;
	char a[40], b[40],c[40];
	
	v_cnt = f_cnt = 0;
	if (!(fp = fopen(filename, "r"))){ return -1; }

	while (fgets(data, 200, fp)){
		if (data[0] == '#'){ continue; }
		else if (data[0] == 'v'){
			
			sscanf(data, "%c %s %s %s", &ch, a, b, c);
			bunny[v_cnt].x = atof(a)*SCALE +150;
			bunny[v_cnt].y = atof(b)*SCALE;
			bunny[v_cnt].z = atof(c)*SCALE + 120;
			bunny_backup[v_cnt].x = bunny[v_cnt].x;
			bunny_backup[v_cnt].y = bunny[v_cnt].y;
			bunny_backup[v_cnt].z = bunny[v_cnt].z;
			v_cnt++; 
			
		}
		else if (data[0] == 'f'){

			sscanf(data, "%c %s %s %s", &ch, a, b, c);
			f[f_cnt][0] = atoi(a)-1;
			f[f_cnt][1] = atoi(b)-1;
			f[f_cnt][2] = atoi(c)-1;
			
			//printf("%d %d %d\n", f[f_cnt][0], f[f_cnt][1], f[f_cnt][2]);
			f_cnt++;
		}
		
	}
	
	fclose(fp);
	get_normalV();
	getVertexNormal();
	return 1;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Beizer Surface Editor");

	ReadInput("bunny.obj");
	getCenter();
	init();
	FindShortest();
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(mouse_move_callback);
	//glutDisplayFunc(display_callback);
	glutKeyboardFunc(keyboard_callback);
	glutIdleFunc(display_callback);
	
	glutMainLoop();
	
	return 0;
}
