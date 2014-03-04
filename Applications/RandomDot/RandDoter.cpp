
#include "RandDoter.hpp"

#include "DXUT.h"
#include "Geometry/vertex.h"


using namespace EyeStereo;

RandomDot::RandomDot():pd3dDevice(0), iVertexNum(-1), dotDist(0.005){}

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
}


void RandomDot::makeRandomDot(Vertex** vertices, float x, float y) {

	srand(1);

	for (int i = 0; i < iVertexNum; ++i) {
			vertices[0][i] = Vertex(RandF(0, x), RandF(0, y), 1, WHITE);
		
		//	printf("Build:(%f, %f, %f)\n", vertices[0][i0].pos.x, vertices[0][i0].pos.y, vertices[0][i0].pos.z);

			vertices[1][i] = vertices[0][i];
			vertices[1][i].color = WHITE;
			binGeometry[i] = false;


			if (inGeometry(vertices[0][i], pGeo)) {// && vertices[0][i].pos.x >= dotDist && vertices[0][i].pos.x <= x+dotDist) {
				vertices[0][i].pos.x -= dotDist; //init as 0.2
				binGeometry[i] = true;
				//printf("new Point (%0.2f, %0.2f)\n", vertices[0][i].pos.x, vertices[0][i].pos.y);
			} else if (inGeometry(vertices[0][i].pos.x+dotDist, vertices[0][i].pos.y, 0, pGeo)) {
				do {
					vertices[0][i] = Vertex(RandF(0, x), RandF(0, y), 1, WHITE);
				//	printf("new Point (%0.2f, %0.2f)\n", vertices[0][i].pos.x, vertices[0][i].pos.y);
					if (inGeometry(vertices[0][i], pGeo) && (!inGeometry(vertices[0][i].pos.x+dotDist, vertices[0][i].pos.y, 0, pGeo))) {
						break;
					} 
				}while (1);
			}

			
	}
}




bool RandomDot::init(ID3D10Device *pDevice, float x, float y, const int cnt, Geometry*	gout) {
	//UnknownType vbd;
	pd3dDevice = pDevice;
	pd3dDevice->AddRef();

	//Vertex* vertices[2];

	vertices[0] = new Vertex[cnt];
	vertices[1] = new Vertex[cnt];
	binGeometry = new bool[cnt];
	velocity = new Vector3[cnt];
	ZeroMemory(velocity, sizeof(Vector3)*cnt);
	velocityL = Vector3(0, 0, 0);
	iVertexNum = cnt;
	
	pGeo = gout;

	vBuffer[0] = vBuffer[1] = NULL;


	boundX = x, boundY = y;
	makeRandomDot(vertices, x, y);
	updateBuffer();

	pointStat = new Vector3[iVertexNum];
	ZeroMemory(pointStat, sizeof(Vector3)*iVertexNum);
	//updateVertex();
	//updateBuffer();
	//delete vertices[0];
	//delete vertices[1];

	return true;

}


bool RandomDot::outBound(Vector3 p, Vector3 &v, int flg) {
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
	using std::vector;

	vector< Vertex > vtemp;
	vector< int > itemp;

	vtemp.clear();
	itemp.clear();

	for (int i = 0; i < iVertexNum; ++i) {
		Vector3 b = Vector3(RandF(-1,1), RandF(-1,1), 0);
		
		velocity[i] += b*ftime;

		if (outBound(vertices[1][i].pos, velocity[i], 0)) {
			//velocity[i] = -velocity[i];
		}


		vertices[0][i].pos += velocity[i]*(ftime/2);
		vertices[1][i].pos = vertices[0][i].pos;

		


		if (inGeometry(vertices[1][i], pGeo)) {
			//printf("inGeometry");
			if (!inGeometry(vertices[1][i].pos.x-dotDist, vertices[1][i].pos.y, vertices[1][i].pos.z, pGeo)) {
				vtemp.push_back(vertices[1][i]);
			}

			vertices[1][i].pos.x += dotDist;

		} else if (inGeometry(vertices[1][i].pos.x - dotDist, vertices[1][i].pos.y, vertices[1][i].pos.z, pGeo)) {
			itemp.push_back(i);
		}

		
		/*
		if (inGeometry(vertices[0][i], pGeo)) {
			vertices[0][i].pos.x -= dotDist; //init as 0.2
		} else if (inGeometry(vertices[0][i].pos.x+dotDist, vertices[0][i].pos.y, 0, pGeo)) {
			do {
				if (inGeometry(vertices[0][i], pGeo) && (!inGeometry(vertices[0][i].pos.x+dotDist, vertices[0][i].pos.y, 0, pGeo))) {
					break;
				} 
			}while (1);
		}*/

		/*
		if (binGeometry[i] == true) {

			if (outBound(vertices[0][i].pos, velocity[i], 1)) {
				//velocityL = -velocityL;
			}
			vertices[0][i].pos += velocity[i]*(ftime/2);
			vertices[1][i].pos = vertices[0][i].pos;
			vertices[1][i].pos.x += dotDist;
		} else {
			vertices[0][i].pos = vertices[1][i].pos;
		}
		*/
	}

	//printf("Then:(%f, %f, %f)\n", t.x, t.y, t.z);


	for (UINT i = 0; i < itemp.size() && i < vtemp.size(); i++) {
		vertices[1][itemp[i]] = vtemp[i];
	}

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