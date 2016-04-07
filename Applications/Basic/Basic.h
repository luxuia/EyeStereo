#ifndef __SELECT_STEREO_H__
#define __SELECT_STEREO_H__

/** @file
@brief 简单扩展框架
@author luxuia@foxmail.com
*/


#include "Utility/PCH.h"
#include "Geometry/DiffSpecularSphere.hpp"
#include "Geometry/Background.h"
#include "Geometry/Light.h"


using namespace EyeStereo;

/**
@class BasicRender
@brief 简单render类
*/
class BasicRender {

public:

	ID3D10Device*					pd3dDevice = NULL;
	ID3D10Effect*					pEffect = NULL;
	ID3D10EffectTechnique*			pTech = NULL;
	ID3D10InputLayout*				pInputLayout = NULL;

	ID3D10EffectShaderResourceVariable* pShaderRV = NULL;
	
	D3DXMATRIX						matWorld;
	D3DXMATRIX						matWorldViewProjection;

	IDXGISwapChain*					pSwapChain = NULL;

	CModelViewerCamera*				pCamera;

	long int						windowsWidth, windowsHeight; ///< 窗口大小
	StereoSetting*					pStereoSetting;
	bool							bactive;
	Vector3f*						pSpherePos = NULL;

	D3DXVECTOR3						eyePos;

	bool							bStereo;
	float							initDepth, initRadius;
	

	ID3D10RasterizerState*			pSolidState = NULL;

	BasicRender() {
		initRadius = 2;
		initDepth = 4;
	}


/**
	@brief 模块初始化
	@param[in] pd3d ID3D10Device设备指针 
	@param[in] pstereo 立体视设置指针
	@param[in] pcamera 视点
*/
	bool init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera) {
		pd3dDevice = pd3d;

		HRESULT hr;
		V_RETURN(D3DX10CreateEffectFromFile(L"../Data/Shader/Pos4Color4.fx", NULL, NULL, "fx_4_0",
			D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
			pd3d, NULL, NULL, &pEffect, NULL, NULL) == 1);

		pTech = pEffect->GetTechniqueByName("RenderScene");

		D3D10_INPUT_ELEMENT_DESC layout[] = {

			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numElements = sizeof(layout) / sizeof(layout[0]);

		D3D10_PASS_DESC PassDesc;
		pTech->GetPassByIndex(0)->GetDesc( &PassDesc );
		pd3d->CreateInputLayout( layout, numElements, 
			PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout);


		pSwapChain = DXUTGetDXGISwapChain();
		pSwapChain->AddRef();

		windowsHeight = DXUTGetWindowHeight();
		windowsWidth = DXUTGetWindowWidth();

		pStereoSetting = pstereo;
		pCamera = pcamera;

		bactive = false;
		bStereo = false;
		eyePos = D3DXVECTOR3(0, 0, -5);
		
		D3D10_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));

		
		rsDesc.FillMode = D3D10_FILL_SOLID;
		rsDesc.CullMode = D3D10_CULL_BACK;
		rsDesc.FrontCounterClockwise = false;

		pd3dDevice->CreateRasterizerState(&rsDesc, &pSolidState);
		
		return true;

	}


/**
	@brief 渲染过程入口
	@param[in] fTime 上一帧到这一帧所经过的时间
	调用renderLeft和renderRight来渲染，或者用其他方法。具体实现方法可以参考其他具体sample
*/
	void stereoRender(float fTime) {

		pd3dDevice->IASetInputLayout(pInputLayout);
		pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pd3dDevice->RSSetState(pSolidState);

		renderLeft(fTime);
		renderRight(fTime);
		
	}

	void renderLeft(float fTime) {
		

	}

	void renderRight(float fTime) {
	}


/**
	类析构函数
*/
	~BasicRender() {
		SAFE_RELEASE(pEffect);
		SAFE_RELEASE(pInputLayout);
		SAFE_RELEASE(pSwapChain);
		SAFE_RELEASE(pSolidState);
	}


/**
	填充窗口，考虑放到Utility里
*/
	void ClearScreen(ID3D10Device *pd3dDevice)
	{
		float ClearColor[4] = { 0.176f, 0.196f, 0.667f, 0.0f };
		ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
		pd3dDevice->ClearRenderTargetView( pRTV, ClearColor );

		// Clear the depth stencil
		ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
		pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH, 1.0, 0 );
		return;
	}

};



#endif //__SELECT_STEREO_H__