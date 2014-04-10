#include "RandDoter.hpp"

#include "digit_numbers.h"
#include "DXUT.h"
#include "Geometry/vertex.h"

#define __DEBUG__ 1

using namespace EyeStereo;

RandomDot::RandomDot(int maxbound):pd3dDevice(0), iVertexNum(-1), dotDist(0.005), maxPointNum(maxbound) {}

RandomDot::~RandomDot()	{

	SAFE_DELETE( vertices[0] );
	SAFE_DELETE( vertices[1] );
	SAFE_DELETE( pointStat );
	SAFE_DELETE( velocity );
	SAFE_DELETE( binGeometry );
	SAFE_RELEASE( vBuffer[0]  );
	SAFE_RELEASE( vBuffer[1] );
	SAFE_RELEASE( pd3dDevice );
}



bool RandomDot::inGeometry(Vertex &p, Geometry *g) {
	return inGeometry(p.pos.x, p.pos.y, 0, g);
}

bool RandomDot::inGeometry(float x, float y, float z, Geometry *g) {
	/*
	Vector3	P(x, y, z);
	//printf("check\n");
	for (int i = 0; i < g->cnt; i++) {

		Vector3 A(g->point[i*3].x,	 g->point[i*3].y,   g->point[i*3].z);
		Vector3 B(g->point[i*3+1].x, g->point[i*3+1].y, g->point[i*3+2].z);
		Vector3 C(g->point[i*3+2].x, g->point[i*3+2].y, g->point[i*3+2].z);

		Vector3 v0 = C - A ;
		Vector3 v1 = B - A ;
		Vector3 v2 = P - A ;

		float dot00 = v0.Dot(v0) ;
		float dot01 = v0.Dot(v1) ;
		float dot02 = v0.Dot(v2) ;
		float dot11 = v1.Dot(v1) ;
		float dot12 = v1.Dot(v2) ;

		float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

		float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
		if (u < 0 || u > 1) // if u out of range, return directly
		{
			continue ;
		}

		float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
		if (v < 0 || v > 1) // if v out of range, return directly
		{
			continue;
		}

		if ( u + v <= 1 ) {
			return true;
		}
	}
	return false;
	*/
	return false;
}

// do create Dot then shift these.
void RandomDot::makeRandomDot(Vertex** vertices, float x, float y) {

	srand(1);
	for (int i = 0; i < maxPointNum; ++i) {
			vertices[0][i] = Vertex(RandF(0, x), RandF(0, y), 0, WHITE);
			vertices[1][i] = vertices[0][i];
			vertices[1][i].color = WHITE;
			binGeometry[i] = false;
	}
}


void RandomDot::makeRandomDotTest(Vertex** vertices, float x, float y) {
	srand(1);

	for (int i = 0; i < maxPointNum; ++i) {
		vertices[0][i] = Vertex(RandF(0, x), RandF(0, y), 0, WHITE);

		while (!inGeometry(vertices[0][i], pGeo)) {
			vertices[0][i] = Vertex(RandF(0, x), RandF(0, y), 0, WHITE);
		}

		vertices[1][i] = vertices[0][i];
		vertices[1][i].color = WHITE;
	}
}

void RandomDot::dotShiftNumber(Vertex** vertices)
{
	for (UINT i = 0; i < maxPointNum; ++i) {
		if (pGeo->distanceShiftDot(vertices[1][i].pos.x, vertices[1][i].pos.y) > Elapse) {
#ifdef __DEBUG__
			vertices[1][i].color = YELLOW;
#else
			vertices[1][i].pos.x += dotDist;
#endif
		}
	}
}

bool RandomDot::init(ID3D10Device *pDevice, float x, float y, const int cnt, Geometry*	gout) {
	//UnknownType vbd;
	pd3dDevice = pDevice;
	pd3dDevice->AddRef();

	//Vertex* vertices[2];

	vertices[0] = new Vertex[maxPointNum];
	vertices[1] = new Vertex[maxPointNum];
	binGeometry = new bool[maxPointNum];
	velocity = new Vector3f[maxPointNum];
	ZeroMemory(velocity, sizeof(Vector3f)*maxPointNum);
	velocityL = new Vector3f(0, 0, 0);
	iVertexNum = cnt;
	
	pGeo = gout;

	vBuffer[0] = vBuffer[1] = NULL;

	boundX = x, boundY = y;
	makeRandomDot(vertices, x, y);
	dotShiftNumber(vertices);
	updateBuffer();

	pointStat = new Vector3f[maxPointNum];
	ZeroMemory(pointStat, sizeof(Vector3f)*maxPointNum);

	return true;
}


bool RandomDot::outBound(Vector3f p, Vector3f &v, int flg) {
	if (p.x > boundX && v.x > 0) {
		v.x = -v.x;
		return true;
	}
	if (p.x < 0 && v.x < 0) {
		v.x = -v.x;
		return true;
	}
	if (p.y <0 && v.y < 0) {
		v.y = -v.y;
		return true;
	}
	if (p.y > boundY && v.y > 0) {
		v.y = -v.y;
		return true;
	}
	return false;
}

bool RandomDot::updateVertex(float ftime) {

	for (int i = 0; i < maxPointNum; ++i) {
		Vector3f b = Vector3f(RandF(-1,1), RandF(-1,1), 0);
		
		velocity[i] += b*ftime;


		/// Data changed in the procedure
		if (outBound(vertices[1][i].pos, velocity[i], 0)) {
			//velocity[i] = -velocity[i];
		}

		vertices[0][i].pos += velocity[i]*(ftime/2);
		vertices[1][i].pos = vertices[0][i].pos;
		vertices[1][i].color = WHITE;
	
	}

	dotShiftNumber(vertices);
	updateBuffer();

	return true;
}

bool RandomDot::updateBuffer() {
	

	if (vBuffer[0] != NULL) {
		vBuffer[0]->Release();
		vBuffer[1]->Release();
	}

	HRESULT hr;
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Vertex ) * iVertexNum;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices[0];
	hr = pd3dDevice->CreateBuffer( &bd, &InitData, &vBuffer[0] );
	if( FAILED( hr ) )
		return hr;

	InitData.pSysMem = vertices[1];
	hr = pd3dDevice->CreateBuffer( &bd, &InitData, &vBuffer[1] );

	return hr;
}