#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "DXUT.h"

struct Light{

	Light() {
		ZeroMemory(this, sizeof(Light));
	}

	D3DXVECTOR3 pos;
	float pad1;
	D3DXVECTOR3 dir;
	float pad2;
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXVECTOR3 att;
	float spotPow;
	float range;

};



#endif//__LIGHT_H__