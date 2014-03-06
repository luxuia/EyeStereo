
#include "TetrisCubes.h"

using namespace EyeStereo;

bool TetrisCubes::CollisionOne(UINT i, ID3DX10Mesh* othermesh, D3DXVECTOR3 &otherpos, Status dir = DOWN, float space = 0.01) {
	if (Vec3Len(otherpos, WordPos[i]) < 1+Elapse) {
		//break;

	}
	D3DXVECTOR3 dir3;
	D3DXVECTOR3 origin = WordPos[i];
	if (dir & DOWN) {
		dir3 = D3DXVECTOR3(0, -1, 0);
		/*origin.y -= 0.5;*/
	} else if (dir == LEFT) {
		dir3 = D3DXVECTOR3(-1, 0, 0);
		/*origin.x -= 0.5;*/
	} else if (dir == RIGHT) {
		dir3 = D3DXVECTOR3(+1, 0, 0);
		/*origin.x += 0.5;*/
	}
	float dist;
	D3DXMATRIX msword;
	D3DXMATRIX inverseW;

	D3DXMatrixTranslation(&msword, otherpos.x, otherpos.y, otherpos.z);
	D3DXMatrixInverse(&inverseW, 0, &msword);
	D3DXVECTOR3 torigin, tdir;

	D3DXVec3TransformCoord(&torigin, &origin, &inverseW);
	D3DXVec3TransformNormal(&tdir, &dir3, &inverseW);

	UINT hitCount;

	othermesh->Intersect(&torigin, &tdir, &hitCount, NULL, NULL, NULL, &dist, NULL);

	if (abs(dist-0.5) < space + 0.001) {

		return true;
	};
	return false;
}

void TetrisCubes::Move(float step, Status dir = NONE) {
	if (dir == RIGHT)
		for (UINT i = 0; i < WordPos.size(); ++i ) {
			WordPos[i].x += step;
		}
	else if (dir == LEFT) {
		for (UINT i = 0; i < WordPos.size(); ++i ) {
			WordPos[i].x -= step;
		}
	} else if (dir == DOWN) {
		for (UINT i = 0; i < WordPos.size(); ++i ) {
			WordPos[i].y -= step;
		}
	} 
}

void TetrisCubes::Move(float step) {
	if (step > Elapse) 
		Move(step, RIGHT);
	else if (step < -Elapse ){
		Move(-step, LEFT);
	}
}


int POINTCMP(const D3DXVECTOR3& pa, const D3DXVECTOR3& pb) {
	if (pa.x != pb.x)
		return pa.x < pb.x;
	return pa.y < pb.y;
}

D3DXVECTOR3*	TetrisCubes::getCentrePos() {
		std::sort(WordPos.begin(), WordPos.end(), POINTCMP);
		CentrePos = new D3DXVECTOR3((WordPos[0].x + WordPos[WordPos.size()-1].x)/2.0, (WordPos[0].y + WordPos[WordPos.size()-1].y)/2.0,0);
		return CentrePos;
}


void TetrisCubes::Rotate90() {
	
	D3DXMATRIX mat;
	D3DXVECTOR4 tvec4;
	D3DXVECTOR3 centre = *getCentrePos();
	D3DXMatrixRotationZ(&mat, EYE_PI/2);
	for (UINT i = 0; i < WordPos.size(); ++i) {
		WordPos[i] -= centre;
		D3DXVec3Transform(&tvec4, &(WordPos[i]), &mat);
		WordPos[i].x = tvec4.x;
		WordPos[i].y = tvec4.y;
		WordPos[i].z = tvec4.z;
		WordPos[i] += centre;
	}
	if (type == LINE) {
		for (UINT i = 0; i < WordPos.size(); ++i) {
			WordPos[i].x -= 0.5;
		}
	}
}