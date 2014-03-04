//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------


#ifndef __PINGPANG_H__
#define __PINGPANG_H__


#include "Utility\PCH.h"
#include "Geometry\DiffSpecularSphere.hpp"
#include "Geometry\DiffSpecularBox.h"
#include "Geometry\Light.h"

#include "Stereo\StereoSetting.h"
#include "Event\KeyBoardState.h"

#include "Geometry\Vertex.h"


using namespace EyeStereo;

class PingPang {
	class Ball: public DiffSpecularSphere {
	public:
		D3DXVECTOR3 pos;
		float r;

		D3DXVECTOR3 v;

		Ball() {
			pos.x = pos.y = pos.z = 0;
		}
	};



public:
	ID3D10Device*	pd3dDevice;
	ID3D10Effect*	pEffect;
	ID3D10EffectTechnique*	pTech;
	ID3D10InputLayout*		pInputLayout;


	IDXGISwapChain*			pSwapChain;
	CModelViewerCamera*		pCamera;

	int lengthStack, heightStack;


	ID3D10EffectMatrixVariable*		mfxWVP;
	ID3D10EffectMatrixVariable*		mfxWorld;
	ID3D10EffectVariable*			mfxEyePosVal;
	ID3D10EffectVariable*			mfxLightVal[3];
	ID3D10EffectScalarVariable*		mfxLightType;

	ID3D10EffectShaderResourceVariable* pShaderRV;

	D3DXMATRIX						matWorld;
	D3DXMATRIX						matBlockWorld[20];
	D3DXMATRIX						matWorldViewProjection;


	Ball*							pBall;
	std::vector<DiffSpecularBox*>			pBoxVector;			

	long int						windowsWidth, windowsHeight;
	StereoSetting*					pStereoSetting;
	bool							bactive;
	bool							bstereo;

	Light							lights[3];
	int								lightType;
	D3DXVECTOR3						eyePos;
// 
// 	ID3D10RasterizerState*			pSolidState;

	KeyBoard*						pKeyBoard;
	
	PingPang():pShaderRV(NULL),pd3dDevice(NULL), pEffect(NULL), pTech(NULL), pInputLayout(NULL), pSwapChain(NULL),pBall(NULL) {
		lengthStack = 4;
		heightStack = 3;
	}

	bool init(ID3D10Device* pdevice, StereoSetting* stereo, CModelViewerCamera* pcamera, KeyBoard* key);

	void CreateBounds();

	void initLight();

	void Restart();

	void CaculateBallV(float fTime);

	D3DXVECTOR3 RefectBall(D3DXVECTOR3 v, D3DXVECTOR3 normal);

	void stereoRender(float fTime);

	void renderScene(int index);

	~PingPang();



	void ClearScreen(ID3D10Device *pd3dDevice);

};


#endif //__PINGPANG_H__