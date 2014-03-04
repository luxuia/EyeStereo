#ifndef __GEOMETRY__
#define __GEOMETRY__

#include "Utility/PCH.h"

struct Vector3 {

	float x, y, z;

	Vector3(float xx, float yy, float zz)
		:x(xx), y(yy), z(zz){}
	Vector3(){}
	Vector3(D3DXVECTOR3 &p):x(p.x), y(p.y), z(p.z) {

	}

	Vector3 operator*(float a) {
		return Vector3(x*a, y*a, z*a);
	}

	

	Vector3 operator-(const Vector3& v) const {
		return Vector3(x-v.x, y-v.y, z-v.z);
	}

	void operator+=(const Vector3& v) {
		x+=v.x;
		y+=v.y;
		z+=v.z;
	}

	void operator*=(float a) {
		x*=a;
		y*=a;
		z*=a;
	}

	Vector3 operator-(void) {
		return Vector3(-x, -y, -z);
	}
	void operator=(const Vector3& a) {
		x = a.x;
		y = a.y;
		z = a.z;
	}

	float Dot(const Vector3& v) const {
		return x*v.x + y*v.y + z*v.z;
	}

	Vector3 Cross(const Vector3& v) const {
		return Vector3(
			y*v.z - z*v.y,
			z*v.x - x*v.z,
			x*v.y - y*v.x);
	}


	
};


struct Geometry {

	Vector3 *point;
	int		 cnt;

	Geometry(int n) {
		cnt = n;
		point = new Vector3[3*n];
	}

	Geometry(Geometry *p) {
		cnt = p->cnt;
		point = p->point;
	}
	~Geometry() {
		delete point;
	}

};

#endif //__GEOMERTY__