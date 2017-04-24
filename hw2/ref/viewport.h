#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <vector>
#include <iostream>

#ifndef EPS
#define EPS         1.110223024E-16        /* data type is float */
#endif
#define LARGE    FLT_MAX

enum XY { X = 0, Y };
enum TYPE { AABB = 0, OBB };

typedef int face[3];

struct Vector3d{
	double x;
	double y;
	double z;
	double len;
	int i, j;
	double v[3];
	Vector3d(){
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->v[0] = 0;
		this->v[1] = 0;
		this->v[2] = 0;
	}
	Vector3d(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void set(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void setvec(float x, float y, float z){
		this->v[0] = x;
		this->v[1] = y;
		this->v[2] = z;
	}
	void sub(Vector3d t1, Vector3d t2)
    {
		this->x = t1.x - t2.x;
		this->y = t1.y - t2.y;
		this->z = t1.z - t2.z;
	} 
	void mulM(Vector3d m1, Vector3d m2, Vector3d m3, Vector3d t){
	
		this->v[0] = (m1.x)*(t.x) + (m1.y)*(t.y) + (m1.z)*(t.z);
		this->v[1] = (m2.x)*(t.x) + (m2.y)*(t.y) + (m2.z)*(t.z);
		this->v[2] = (m3.x)*(t.x) + (m3.y)*(t.y) + (m3.z)*(t.z);
	}
	void GetNewPoint(Vector3d m1, Vector3d m2, Vector3d m3, Vector3d t){
		this->x = m1.x*v[0] + m2.x*v[1] + m3.x*v[2];// +t.x;
		this->y = m1.y*v[0] + m2.y*v[1] + m3.y*v[2];// +t.y;
		this->z = m1.z*v[0] + m2.z*v[1] + m3.z*v[2];// +t.z;

		this->add(t);
	}


	void cross(Vector3d v1, Vector3d v2)
    { 
        float x,y;

        x = v1.y*v2.z - v1.z*v2.y;
        y = v2.x*v1.z - v2.z*v1.x;
        this->z = v1.x*v2.y - v1.y*v2.x;
        this->x = x;
        this->y = y;
    }
	void scale(float s)
    {
        this->x *= s;
        this->y *= s;
        this->z *= s;
    }
	void add(Vector3d t1)
    { 
        this->x = this->x + t1.x;
        this->y = this->y + t1.y;
        this->z = this->z + t1.z;
    }
	void normalize()
    {
        float norm;

        norm = 1.0f/std::sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
        this->x *= norm;
        this->y *= norm;
        this->z *= norm;
    }
	float length()
    {
        return std::sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
    }
	float dot(Vector3d v1)
    {
      return (this->x*v1.x + this->y*v1.y + this->z*v1.z);
    }
	float angle(Vector3d v1) 
    { 
      float vDot = this->dot(v1) / ( this->length()*v1.length() );
      if( vDot < -1.0) vDot = -1.0;
      if( vDot >  1.0) vDot =  1.0;
      return((float) (std::acos( vDot )));
    }
	float distance(Vector3d p1)
    {
      float dx, dy, dz;

      dx = this->x-p1.x;
      dy = this->y-p1.y;
      dz = this->z-p1.z;
      return std::sqrt(dx*dx+dy*dy+dz*dz);
    }
	float distancef(float x1, float y1, float z1)
	{
		float dx, dy, dz;

		dx = this->x - x1;
		dy = this->y - y1;
		dz = this->z - z1;
		return std::sqrt(dx*dx + dy*dy + dz*dz);
	}
	float* genArray(){
		float* v = new float[3];
		v[0] = x;
		v[1] = y;
		v[2] = z;
		return v;
	}
};

struct Matrix3d{
	float m00, m01, m02, m10, m11,m12, m20, m21, m22;

	Matrix3d(){
	}
	void setV(Vector3d v1, Vector3d v2, Vector3d v3){
		m00 = v1.x;
		m01 = v1.y;
		m02 = v1.z;
		m10 = v2.x;
		m11 = v2.y;
		m12 = v2.z;
		m20 = v3.x;
		m21 = v3.y;
		m22 = v3.z;
	}
	void set(float angle, Vector3d v){
		set(angle, v.x, v.y, v.z);
	}
	Matrix3d inverse(Matrix3d m) {

		Matrix3d n;

		float det = m.m00*(m.m11*m.m22 - m.m21*m.m12)
			- m.m01*(m.m10*m.m22 - m.m12*m.m20)
			+ m.m02*(m.m10*m.m21 - m.m11*m.m20);

		float invdet = 1 / det;

		n.m00 = (m.m11 * m.m22 - m.m21*m.m12) * invdet;
		n.m10 = -1 * (m.m01 * m.m22 - m.m02*m.m21) * invdet;
		n.m20 = (m.m01 * m.m12 - m.m02*m.m11) * invdet;

		n.m01 = -1 * (m.m10 * m.m22 - m.m12*m.m20) * invdet;
		n.m11 = (m.m00 * m.m22 - m.m02*m.m20) * invdet;
		n.m21 = -1 * (m.m00 * m.m12 - m.m10*m.m02) * invdet;

		n.m02 = (m.m10 * m.m21 - m.m20*m.m11) * invdet;
		n.m12 = -1 * (m.m00 * m.m21 - m.m20*m.m01) * invdet;
		n.m22 = (m.m00 * m.m11 - m.m10*m.m01) * invdet;

		return n;
	}
	void set(float angle, float x, float y, float z){
		float mag = std::sqrt( x*x + y*y + z*z);

      if( mag < EPS ) {
		m00 = 1.0;
		m01 = 0.0;
		m02 = 0.0;

		m10 = 0.0;
		m11 = 1.0;
		m12 = 0.0;

		m20 = 0.0;
		m21 = 0.0;
		m22 = 1.0;
      } else {
		mag = 1.0f/mag;
        float ax = x*mag;
        float ay = y*mag;
        float az = z*mag;

        float sinTheta = std::sin(angle);
        float cosTheta = std::cos(angle);
        float t = 1.0f - cosTheta;

        float xz = ax * az;
        float xy = ax * ay;
        float yz = ay * az;

        m00 = t * ax * ax + cosTheta;
        m01 = t * xy - sinTheta * az;
        m02 = t * xz + sinTheta * ay;

        m10 = t * xy + sinTheta * az;
        m11 = t * ay * ay + cosTheta;
        m12 = t * yz - sinTheta * ax;

        m20 = t * xz - sinTheta * ay;
        m21 = t * yz + sinTheta * ax;
        m22 = t * az * az + cosTheta;
      }
	}

	void transform(Vector3d& t) {
		 float x,y,z;
		 x = m00* t.x + m01*t.y + m02*t.z;
		 y = m10* t.x + m11*t.y + m12*t.z;
		 z = m20* t.x + m21*t.y + m22*t.z;
		 t.set(x,y,z);
    }
};

struct Triangle{
	Vector3d vertices[3];
	Vector3d normal;
	Triangle(Vector3d& v1, Vector3d& v2, Vector3d& v3){
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		Vector3d vector1, vector2;
		vector1.sub(v2, v1);
		vector2.sub(v3, v2);
		normal.cross(vector1, vector2);
		normal.normalize();
	}
};

Vector3d rotate(Vector3d input, Vector3d rotateVector, float angle);
Vector3d unProjectToEye(Vector3d vector, Vector3d& eye, Vector3d& center, Vector3d& upVector);
Vector3d getMousePoint(int mouseX, int mouseY, int width, int height, float radius);


#endif /* _VIEWPORT_H_ */

