#pragma once

#ifndef __VERTEX_H__
#define __VERTEX_H__

//#include "DXUT.h"
#include "Geometry.h"

struct Vertex{

	Vertex(){}
	Vertex(float x, float y, float z, D3DXCOLOR c)
		:pos(x, y, z), color(c) {}

	Vector3 pos;
	D3DXCOLOR	color;


	void operator=(const Vertex& a) {
		pos = a.pos;
		color = a.color;
	}
};

struct Vertex8 {

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;

	Vertex8(){}
	Vertex8(float x, float y, float z, 
		float nx, float ny, float nz, 
		float u, float v)
		: pos(x,y,z), normal(nx,ny,nz), texC(u,v){}

};


struct Vertex14 {

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXCOLOR	diffuse;
	D3DXCOLOR	spec;

	Vertex14(){}

	Vertex14(float x, float y, float z, float nx, float ny, float nz):pos(D3DXVECTOR3(x, y, z)),normal(D3DXVECTOR3(nx, ny, nz))  {};
};


#endif // __VERTEX_H__