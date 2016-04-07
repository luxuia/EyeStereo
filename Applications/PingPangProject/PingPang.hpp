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

/**
	@file 
	@class PingPang

*/

class PingPang {
	/**
		@class Ball
		球类
	*/
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
	ID3D10Device*	pd3dDevice = NULL;
	ID3D10Effect*	pEffect = NULL;
	ID3D10EffectTechnique*	pTech = NULL;
	ID3D10InputLayout*		pInputLayout = NULL;


	IDXGISwapChain*			pSwapChain = NULL;
	CModelViewerCamera*		pCamera = NULL;

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


	Ball*							pBall = NULL;
	std::vector<DiffSpecularBox*>   pBoxVector;			

	long int						windowsWidth, windowsHeight;
	StereoSetting*					pStereoSetting = NULL;
	bool							bactive;
	bool							bstereo;

	Light							lights[3];
	int								lightType;
	D3DXVECTOR3						eyePos;
// 
// 	ID3D10RasterizerState*			pSolidState;

	KeyBoard*						pKeyBoard;
	
	PingPang():pShaderRV(NULL), pd3dDevice(NULL), pEffect(NULL), pTech(NULL), pInputLayout(NULL), pSwapChain(NULL),pBall(NULL) {
		lengthStack = 4;
		heightStack = 3;
	}

	bool init(ID3D10Device* pdevice, StereoSetting* stereo, CModelViewerCamera* pcamera, KeyBoard* key);


	/**
		用4个长方体构建一个围墙，让球不会跑出去
	*/
	void CreateBounds();

	void initLight();

	/**
		将设置为dead的砖块恢复，并将球放在原来的位置
	*/
	void Restart();


	/**
		更新球和砖块的状态，如果球运动方向在精度范围内将会碰撞，就反射并设置砖块的状态为DIE
	*/
	void CaculateBallV(float fTime);

	/**
		@param[in] v 球的速度向量
		@param[in] normal 碰撞面的法向量
		@return 反射后的速度向量
	*/
	D3DXVECTOR3 RefectBall(D3DXVECTOR3 v, D3DXVECTOR3 normal);

	void stereoRender(float fTime);

	void renderScene(int index);

	~PingPang();



	void ClearScreen(ID3D10Device *pd3dDevice);

};


#endif //__PINGPANG_H__