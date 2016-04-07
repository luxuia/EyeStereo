#ifndef __SELECT_STEREO_H__
#define __SELECT_STEREO_H__

/** @file
	@brief 四选一项目，让用户在四个球中选出真立体的求
*/

#include "Utility/PCH.h"
#include "Geometry/DiffSpecularSphere.hpp"   //< 带有漫反射和高光的球体
#include "Geometry/Background.h"			//< 带有颜色的长方体
#include "Geometry/Light.h"					//< 光渲染辅助模块


using namespace EyeStereo;

/** 
	@class ChooseItRender

*/

class ChooseItRender {

public:

	ID3D10Device*					pd3dDevice = NULL;
	ID3D10Effect*					pEffect = NULL;
	ID3D10EffectTechnique*			pTech = NULL;
	ID3D10InputLayout*				pInputLayout = NULL;
	
	
			
	ID3D10EffectMatrixVariable*		mfxWVP;
	ID3D10EffectMatrixVariable*		mfxWorld;
	ID3D10EffectVariable*			mfxEyePosVal;
	ID3D10EffectVariable*			mfxLightVal[3];  ///< 光的属性，平行光，点光，锥形光
	ID3D10EffectScalarVariable*		mfxLightType;    ///< 光的类型

	ID3D10EffectShaderResourceVariable* pShaderRV = NULL;
	
	D3DXMATRIX						matWorld;
	D3DXMATRIX						matWorldViewProjection;

	IDXGISwapChain*					pSwapChain = NULL;

	CModelViewerCamera*				pCamera;
	//Box*							pBox;
	DiffSpecularSphere*				pSphere = NULL;
	Background*						pBackground = NULL;

	long int						windowsWidth, windowsHeight;
	StereoSetting*					pStereoSetting;
	bool							bactive;
	Vector3f*						pSpherePos = NULL;

	Light							lights[3];
	int								lightType;
	D3DXVECTOR3						eyePos;

	bool							bStereo;
	float							initDepth, initRadius;
	

	ID3D10RasterizerState*			pSolidState = NULL;

	ChooseItRender() {
		initRadius = 2;
		initDepth = 4;
	}

	bool init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera) {
		pd3dDevice = pd3d;

		HRESULT hr;
		V_RETURN( D3DX10CreateEffectFromFile( L"../Data/Shader/DiffSpecular.fx", NULL, NULL, "fx_4_0", 
			D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
			pd3d, NULL, NULL, &pEffect, NULL, NULL ) );

		pTech = pEffect->GetTechniqueByName("RenderScene");
		
		mfxWVP		= pEffect->GetVariableByName("gWVP")->AsMatrix();
		mfxWorld	= pEffect->GetVariableByName("gWorld")->AsMatrix();
		mfxEyePosVal= pEffect->GetVariableByName("gEyePosW");
		mfxLightVal[0] = pEffect->GetVariableByName("gParalledLight");
		mfxLightVal[1] = pEffect->GetVariableByName("gPointLight");
		mfxLightVal[2] = pEffect->GetVariableByName("gSpotLight");
		mfxLightType= pEffect->GetVariableByName("gLightType")->AsScalar();

		D3D10_INPUT_ELEMENT_DESC layout[] = {

			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,  0,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
			{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	12, D3D10_INPUT_PER_VERTEX_DATA, 0},
			{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	24, D3D10_INPUT_PER_VERTEX_DATA, 0},
			{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	40, D3D10_INPUT_PER_VERTEX_DATA, 0},
		};


		UINT numElements = sizeof(layout) / sizeof(layout[0]);

		D3D10_PASS_DESC PassDesc;
		pTech->GetPassByIndex(0)->GetDesc( &PassDesc );
		pd3d->CreateInputLayout( layout, numElements, 
			PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout);

		pSphere = new DiffSpecularSphere();
		pSphere->init(pd3dDevice, 1, 40, 40);

		pBackground = new Background();
		pBackground->init(pd3dDevice, 20, 10, 10);


		pSwapChain = DXUTGetDXGISwapChain();
		pSwapChain->AddRef();

		windowsHeight = DXUTGetWindowHeight();
		windowsWidth = DXUTGetWindowWidth();

		pStereoSetting = pstereo;
		pCamera = pcamera;

		bactive = false;
		bStereo = false;
		initLight();
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
		
	*/
	void initLight() {
		lightType = 2;

		// Parallel light.
		lights[0].dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
		lights[0].ambient  = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		lights[0].diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		lights[0].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// Pointlight--position is changed every frame to animate.
		lights[1].ambient  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		lights[1].diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		lights[1].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		lights[1].att.x    = 0.0f;
		lights[1].att.y    = 0.1f;
		lights[1].att.z    = 0.0f;
		lights[1].range    = 50.0f;
		lights[1].pos	   = D3DXVECTOR3(0, 2, 6);
		lights[1].dir	   = D3DXVECTOR3(1, -2, 6);

		// Spotlight--position and direction changed every frame to animate.
		lights[2].ambient  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		lights[2].diffuse  = D3DXCOLOR(.3f, .3f, .3f, 1.0f);
		lights[2].specular = D3DXCOLOR(0.3f, .3f, .3f, 1.0f);
		lights[2].att.x    = 1.0f;
		lights[2].att.y    = 0.0f;
		lights[2].att.z    = 0.0f;
		lights[2].spotPow  = 64.0f;
		lights[2].range    = 10000.0f;
		lights[2].pos	   = D3DXVECTOR3(4, 40, -40);
		lights[2].dir	   = D3DXVECTOR3(-4, -4, 4);

	}


	/**
		变化投影矩阵的参数，改变camera。具体见附件3D_Stereo_Rendering_Using_OpenGL_(and_GLUT)
	*/
	void stereoRender(float fTime) {

		pd3dDevice->IASetInputLayout(pInputLayout);
		pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pd3dDevice->RSSetState(pSolidState);
		
		mfxLightType->SetInt(lightType);

		for (UINT i = 0; i < 3; i++) {
			mfxLightVal[i]->SetRawValue(&lights[i], 0, sizeof(Light));
		}
		mfxEyePosVal->SetRawValue(&eyePos, 0, sizeof(D3DXVECTOR3));

		pCamera->SetViewParams(&D3DXVECTOR3(0, 0, -5), &D3DXVECTOR3(0, 0, 0));

		D3DXMATRIX mView;
		D3DXMATRIX mPorject;

		// ----Draw a Stereo Sphere
		
		D3DXMatrixTranslation(&matWorld, pSpherePos[0].x, pSpherePos[0].y, pSpherePos[0].z);

		mView = *pCamera->GetViewMatrix();
		mPorject = *pCamera->GetProjMatrix();


		mfxWorld->SetMatrix( (float*)&matWorld );

		float currentSeparaion = pStereoSetting->g_Separation * pStereoSetting->g_EyeSeparation;

		mPorject._31 = mPorject._31 + currentSeparaion;
		mPorject._41 = mPorject._41 - currentSeparaion* pStereoSetting->g_Convergence;
		//printf("Stereo:\n");
		//printMatrix((float*)&(mPorject.m));
		matWorldViewProjection = matWorld * mView * mPorject;

		mfxWVP->SetMatrix( (float*)&matWorldViewProjection );

		ClearScreen(pd3dDevice);
		renderLeft(fTime);
		// ----End Draw a Stereo Sphere 
		

		// ----Draw background
		D3DXMatrixTranslation(&matWorld, 0, -3, 8);
		mfxWorld->SetMatrix(matWorld);
		matWorldViewProjection = matWorld*mView* mPorject;
		mfxWVP->SetMatrix(matWorldViewProjection);
		drawBacground();
		// ----End Draw background


		//  ----Draw 3 unStereo Sphere
		mPorject = *pCamera->GetProjMatrix();
		//printMatrix((float*)&(mPorject.m));
		for (int i = 1; i < 4; i++) {

			D3DXMatrixTranslation(&matWorld, pSpherePos[i].x, pSpherePos[i].y, pSpherePos[i].z);
			mfxWorld->SetMatrix((float*)&matWorld);
			matWorldViewProjection = matWorld*mView*mPorject;
			mfxWVP->SetMatrix((float*)&matWorldViewProjection);
			renderLeft(fTime);

		}
		// ----End Draw 3...

		pStereoSetting->draw(pd3dDevice, pStereoSetting->LEFT_EYE);


		//Begin Render Right Eye


		D3DXMatrixTranslation(&matWorld, pSpherePos[0].x, pSpherePos[0].y, pSpherePos[0].z);
		
		mPorject = *pCamera->GetProjMatrix();
		mPorject._31 = mPorject._31 - currentSeparaion;
		mPorject._41 = mPorject._41 + currentSeparaion* pStereoSetting->g_Convergence;

		matWorldViewProjection = matWorld * mView * mPorject;

		mfxWVP->SetMatrix( (float*)&matWorldViewProjection );
		ClearScreen(pd3dDevice);
		renderRight(fTime);


		// Draw BackGround
		D3DXMatrixTranslation(&matWorld, 0, -3, 8);
		mfxWorld->SetMatrix(matWorld);
		matWorldViewProjection = matWorld*mView* mPorject;
		mfxWVP->SetMatrix(matWorldViewProjection);
		drawBacground();

		mPorject = *pCamera->GetProjMatrix();
		for (int i = 1; i < 4; i++) {

			D3DXMatrixTranslation(&matWorld, pSpherePos[i].x, pSpherePos[i].y, pSpherePos[i].z);
			mfxWorld->SetMatrix((float*)&matWorld);
			matWorldViewProjection = matWorld*mView*mPorject;
			mfxWVP->SetMatrix((float*)&matWorldViewProjection);
			renderLeft(fTime);
		}

		pStereoSetting->draw(pd3dDevice, pStereoSetting->RIGHT_EYE);



		//非Stereo 时把下面的注释掉 
		
		if (bStereo) {
			ClearScreen(pd3dDevice);
			pStereoSetting->draw(pd3dDevice);
		}
		
	}

	void drawBacground() {
		D3D10_TECHNIQUE_DESC techDesc;
		pTech->GetDesc( &techDesc );
		for (UINT p = 0; p < techDesc.Passes; p++) {
			pTech->GetPassByIndex( p )->Apply( 0 );
			pBackground->draw();
			//	pBox->draw(pd3dDevice);
		}
	}

	void renderLeft(float fTime) {
		
		D3D10_TECHNIQUE_DESC techDesc;
		pTech->GetDesc( &techDesc );
		for (UINT p = 0; p < techDesc.Passes; p++) {
			pTech->GetPassByIndex( p )->Apply( 0 );
			pSphere->draw();
			//pBox->draw(pd3dDevice);
		}

	}

	void renderRight(float fTime) {
		D3D10_TECHNIQUE_DESC techDesc;
		pTech->GetDesc( &techDesc );
		for (UINT p = 0; p < techDesc.Passes; p++) {
			pTech->GetPassByIndex( p )->Apply( 0 );
			pSphere->draw();
		//	pBox->draw(pd3dDevice);
		}
	}


	~ChooseItRender() {
		SAFE_RELEASE(pEffect);
		//pTech;
		SAFE_RELEASE(pInputLayout);
		SAFE_RELEASE(pSwapChain);
		SAFE_RELEASE(pSolidState);
		//SAFE_RELEASE(pShaderRV);
		//delete pBox;
		SAFE_DELETE( pSphere);
		SAFE_DELETE( pBackground);
		
	}



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