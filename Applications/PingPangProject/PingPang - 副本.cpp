//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------

#include "PingPang.hpp"

using namespace EyeStereo;


bool PingPang::init(ID3D10Device* pdevice, StereoSetting* stereo, CModelViewerCamera* pcamera, KeyBoard* key) {
	pd3dDevice = pdevice;
	pCamera = pcamera;
	pCamera->SetViewParams(&D3DXVECTOR3(-1, 2, -2), &D3DXVECTOR3(-1, 0, 0));
	pStereoSetting = stereo;
	pKeyBoard = key;

	D3DX10CreateEffectFromFile(L"../Data/Shader/DiffSpecular.fx", NULL, NULL, "fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
		pd3dDevice, NULL, NULL, &pEffect, NULL, NULL);

	pTech = pEffect->GetTechniqueByName("RenderScene");


	mfxWVP = pEffect->GetVariableByName("gWVP")->AsMatrix();
	mfxWorld = pEffect->GetVariableByName("gWorld")->AsMatrix();
	mfxEyePosVal = pEffect->GetVariableByName("gEyePosW");
	mfxLightVal[0] = pEffect->GetVariableByName("gParalledLight");
	mfxLightVal[1] = pEffect->GetVariableByName("gPointLight");
	mfxLightVal[2] = pEffect->GetVariableByName("gSpotLight");
	mfxLightType = pEffect->GetVariableByName("gLightType")->AsScalar();

	D3D10_INPUT_ELEMENT_DESC layout[] = {

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//glutSolidSphere()

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	D3D10_PASS_DESC PassDesc;
	pTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	pd3dDevice->CreateInputLayout(layout, numElements,
		PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout);


	pBall = new Ball();
	pBall->init(pd3dDevice, 0.25, 40, 40);
	pBall->pos.y = -2;
	pBall->pos.z = 1;
	pBall->r = 0.25;
	pBall->v = D3DXVECTOR3(RandF() / 5, 0, 2);


	blockBox* box = new blockBox(0.5, 0.2, 0.1, WHITE);
	box->init(pd3dDevice);
	box->live = blockBox::FOREVER;
	pBoxVector.push_back(box);

	for (int i = 0; i < heightStack*lengthStack / 2; i++) {
		blockBox *box1 = new blockBox(0.5, 0.2, 0.1, RED);
		box1->init(pd3dDevice);
		pBoxVector.push_back(box1);
		blockBox *box2 = new blockBox(0.5, 0.2, 0.1, YELLOW);
		box2->init(pd3dDevice);
		pBoxVector.push_back(box2);
	}

	//D3DXMatrixTranslation(matBlockWorld, 0, -2, 2);
	pBoxVector[0]->x = 0;
	pBoxVector[0]->y = -2;
	pBoxVector[0]->z = 0;
	for (int i = 0; i < heightStack; i++) {
		for (int j = 0; j < lengthStack; j++) {
			D3DXMatrixTranslation(&(matBlockWorld[i*lengthStack + j + 1]),
				-2 + j, -2, 0.2f*i + 4);

			pBoxVector[i*lengthStack + j + 1]->x = -2 + j;
			pBoxVector[i*lengthStack + j + 1]->y = -2;
			pBoxVector[i*lengthStack + j + 1]->z = 0.2*i + 4;
			pBoxVector[i*lengthStack + j + 1]->live = blockBox::LIVE;
			pBoxVector[i*lengthStack + j + 1]->CaculateWorldAxis(matBlockWorld[i*lengthStack + j + 1]);
			pBoxVector[i*lengthStack + j + 1]->updatePlane(matBlockWorld[i*lengthStack + j + 1]);

		}
	}


	CreateBounds();


	pSwapChain = DXUTGetDXGISwapChain();
	pSwapChain->AddRef();

	windowsHeight = DXUTGetWindowHeight();
	windowsWidth = DXUTGetWindowWidth();

	pCamera = pcamera;

	bactive = false;
	bstereo = false;
	//printf("InitLight");
	initLight();
	eyePos = D3DXVECTOR3(0, 0, 0);



	// 		D3D10_RASTERIZER_DESC rsDesc;
	// 		ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));
	// 
	// 		
	// 		rsDesc.FillMode = D3D10_FILL_SOLID;
	// 		rsDesc.CullMode = D3D10_CULL_NONE;
	// 		rsDesc.FrontCounterClockwise = false;
	// 
	// 		pd3dDevice->CreateRasterizerState(&rsDesc, &pSolidState);

	return true;

}


void PingPang::CreateBounds() {
	blockBox *left = new blockBox(0.1, 0.1, 10, GREEN);
	blockBox *right = new blockBox(0.1, 0.1, 10, GREEN);
	blockBox *top = new blockBox(10, 0.1, 0.1, GREEN);
	blockBox *down = new blockBox(10, 0.1, 0.1, GREEN);


	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat,
		-4, -2, 0);
	left->x = -4;
	left->y = -2;
	left->z = 0;
	left->init(pd3dDevice);
	left->CaculateWorldAxis(mat);
	left->updatePlane(mat);
	left->live = blockBox::FOREVER;
	pBoxVector.push_back(left);


	D3DXMatrixTranslation(&mat,
		4, -2, 0);
	right->x = 2;
	right->y = -2;
	right->z = 0;
	right->init(pd3dDevice);
	right->CaculateWorldAxis(mat);
	right->updatePlane(mat);
	right->live = blockBox::FOREVER;
	pBoxVector.push_back(right);

	D3DXMatrixTranslation(&mat,
		0, -2, 6);
	top->x = 0;
	top->y = -2;
	top->z = 6;
	top->init(pd3dDevice);
	top->CaculateWorldAxis(mat);
	top->updatePlane(mat);
	top->live = blockBox::FOREVER;
	pBoxVector.push_back(top);

	D3DXMatrixTranslation(&mat,
		0, -2, -1);
	down->x = 0;
	down->y = -2;
	down->z = -1;
	down->init(pd3dDevice);
	down->CaculateWorldAxis(mat);
	down->updatePlane(mat);
	down->live = blockBox::FOREVER;
	pBoxVector.push_back(down);



}

void PingPang::initLight() {
	lightType = 2;
	//printf("initLight\n");
	// Parallel light.

	lights[0].dir = D3DXVECTOR3(0.5, 0.5, 0.5);
	lights[0].ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	lights[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	lights[0].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// Pointlight--position is changed every frame to animate.
	lights[1].ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	lights[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	lights[1].specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	lights[1].att.x = 0.0f;
	lights[1].att.y = 0.1f;
	lights[1].att.z = 0.0f;
	lights[1].range = 50.0f;
	lights[1].pos = D3DXVECTOR3(0, 2, -6);
	lights[1].dir = D3DXVECTOR3(1, -2, 6);

	// Spotlight--position and direction changed every frame to animate.
	lights[2].ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	lights[2].diffuse = D3DXCOLOR(.3f, .3f, .3f, 1.0f);
	lights[2].specular = D3DXCOLOR(0.3f, .3f, .3f, 1.0f);
	lights[2].att.x = 1.0f;
	lights[2].att.y = 0.0f;
	lights[2].att.z = 0.0f;
	lights[2].spotPow = 64.0f;
	lights[2].range = 10000.0f;
	lights[2].pos = D3DXVECTOR3(4, 40, -40);
	lights[2].dir = D3DXVECTOR3(-4, -4, 4);

}



void PingPang::Restart() {


	for (UINT i = 0; i < pBoxVector.size(); i++) {
		if (pBoxVector[i]->live == blockBox::DIE)
			pBoxVector[i]->live = blockBox::LIVE;
	}

	pBall->pos = D3DXVECTOR3(0, -2, 1);
	pBall->v = D3DXVECTOR3(RandF() / 10, 0, 2);
	pBoxVector[0]->x = 0;
	pCamera->SetViewParams(&D3DXVECTOR3(-1, 2, -2), &D3DXVECTOR3(-1, 0, 0));

}



D3DXVECTOR3 PingPang::RefectBall(D3DXVECTOR3 v, D3DXVECTOR3 normal) {

	//D3DXVec3Normalize(&v, &v);
	//D3DXVec3Normalize(&normal, &normal);


	D3DXVECTOR3 outv(v - D3DXVec3Dot(&v, &normal)*normal * 2);



	if (D3DXVec3Length(&outv) > 3) {
		printf("this is to quick");
	}

	return outv;
}


void PingPang::CaculateBallV(float fTime) {
	//printf("dist from left to %0.2f\n", boxVector[boxVector.size()-1].getDist(5, pBall->pos));

	for (int i = 0; i < pBoxVector.size(); i++) {
		if (pBoxVector[i]->live == blockBox::DIE)
			continue;
		for (int j = 0; j < 6; j++) {
			float dis = pBoxVector[i]->getDist(j, pBall->pos);

			if (dis <= Elapse || dis > pBall->r - Elapse) {
				continue;
			}
			else {
				D3DXVECTOR3 intersect;

				D3DXPlaneIntersectLine(&intersect, &(pBoxVector[i]->pPlane[j]), &(pBall->pos), &(pBall->pos - pBoxVector[i]->v[j * 4].normal));
				if (Vector3(intersect - pBall->pos).Dot(Vector3(pBall->v)) > 0) {
					if (pBoxVector[i]->contain(j, intersect)) {


						pBall->v = RefectBall(pBall->v, pBoxVector[i]->v[j * 4].normal);


						if (i == 0) {
							if ((*pKeyBoard)[2]) {
								pBall->v.x += 0.2;
							}
							else if ((*pKeyBoard)[3]) {
								pBall->v.x -= 0.2;
							}
						}

						//pBall->v.z = -pBall->v.z ; ///////////////////TODO fanshe
						//pBall->v.x += RandF()/10-0.2;
						//	printf("v %0.2f\n", pBall->v.x);
						if (pBoxVector[i]->live != blockBox::FOREVER) {
							pBoxVector[i]->live = blockBox::DIE;
						}
						return;
					}
				}
			}
		}
	}

}



void PingPang::stereoRender(float fTime) {
	static bool first = true;
	if (first) {
		pCamera->SetViewParams(&D3DXVECTOR3(-1, 2, -2), &D3DXVECTOR3(-1, 0, 0));
		first = false;
	}
	ClearScreen(pd3dDevice);
	pd3dDevice->IASetInputLayout(pInputLayout);
	pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	mfxLightType->SetInt(lightType);

	for (UINT i = 0; i < 3; i++) {
		mfxLightVal[i]->SetRawValue(&lights[i], 0, sizeof(Light));
	}
	mfxEyePosVal->SetRawValue((void *)pCamera->GetEyePt(), 0, sizeof(D3DXVECTOR3));

	D3DXMATRIX mView;
	D3DXMATRIX mPorject;

	if ((*pKeyBoard)[2])
		pBoxVector[0]->x += 2 * fTime;
	if ((*pKeyBoard)[3])
		pBoxVector[0]->x -= 2 * fTime;

	D3DXMatrixTranslation(&matBlockWorld[0], pBoxVector[0]->x, pBoxVector[0]->y, pBoxVector[0]->z);
	pBoxVector[0]->CaculateWorldAxis(matBlockWorld[0]);
	pBoxVector[0]->updatePlane(matBlockWorld[0]);



	CaculateBallV(fTime);
	pBall->pos += fTime*pBall->v;

	D3DXMatrixTranslation(&matWorld, pBall->pos.x, pBall->pos.y, pBall->pos.z);

	mView = *pCamera->GetViewMatrix();
	mPorject = *pCamera->GetProjMatrix();

	mfxWorld->SetMatrix((float*)&matWorld);

	float currentSeparaion = pStereoSetting->g_Separation * pStereoSetting->g_EyeSeparation;

	mPorject._31 = mPorject._31 + currentSeparaion;
	mPorject._41 = mPorject._41 - currentSeparaion* pStereoSetting->g_Convergence;


	matWorldViewProjection = matWorld * mView * mPorject;

	mfxWVP->SetMatrix((float*)&matWorldViewProjection);
	renderScene(0);

	//////////////////////////////////////////////////
	//D3DXMatrixTranslation(&matWorld, 2, 2, 2);

	mView = *pCamera->GetViewMatrix();

	for (int i = 0; i < pBoxVector.size(); i++) {
		if (pBoxVector[i]->live == blockBox::DIE)
			continue;
		D3DXMatrixTranslation(&matBlockWorld[i], pBoxVector[i]->x, pBoxVector[i]->y, pBoxVector[i]->z);

		mfxWorld->SetMatrix((float*)&matBlockWorld[i]);
		matWorldViewProjection = matBlockWorld[i] * mView * mPorject;

		mfxWVP->SetMatrix((float*)&matWorldViewProjection);
		D3D10_TECHNIQUE_DESC techDesc;
		pTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; p++) {
			pTech->GetPassByIndex(p)->Apply(0);
			pBoxVector[i]->draw();

		}
	}

	pStereoSetting->draw(pd3dDevice, pStereoSetting->LEFT_EYE);
	ClearScreen(pd3dDevice);
	mPorject = *(pCamera->GetProjMatrix());
	mPorject._31 = mPorject._31 - currentSeparaion;
	mPorject._41 = mPorject._41 + currentSeparaion* pStereoSetting->g_Convergence;


	matWorldViewProjection = matWorld * mView * mPorject;

	mfxWVP->SetMatrix((float*)&matWorldViewProjection);
	renderScene(0);

	//////////////////////////////////////////////////
	//D3DXMatrixTranslation(&matWorld, 2, 2, 2);

	mView = *pCamera->GetViewMatrix();

	for (int i = 0; i < pBoxVector.size(); i++) {
		if (pBoxVector[i]->live == blockBox::DIE)
			continue;
		D3DXMatrixTranslation(&matBlockWorld[i], pBoxVector[i]->x, pBoxVector[i]->y, pBoxVector[i]->z);

		mfxWorld->SetMatrix((float*)&matBlockWorld[i]);
		matWorldViewProjection = matBlockWorld[i] * mView * mPorject;

		mfxWVP->SetMatrix((float*)&matWorldViewProjection);
		D3D10_TECHNIQUE_DESC techDesc;
		pTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; p++) {
			pTech->GetPassByIndex(p)->Apply(0);
			pBoxVector[i]->draw();

		}
	}

	pStereoSetting->draw(pd3dDevice, pStereoSetting->RIGHT_EYE);
	//ClearScreen(pd3dDevice);

	if (bstereo) {
		ClearScreen(pd3dDevice);
		pStereoSetting->draw(pd3dDevice);
	}


}

void PingPang::renderScene(int index) {
	D3D10_TECHNIQUE_DESC techDesc;
	pTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; p++) {
		pTech->GetPassByIndex(p)->Apply(0);
		pBall->draw();
	}
}


void PingPang::ClearScreen(ID3D10Device *pd3dDevice)
{
	float ClearColor[4] = { 0.176f, 0.196f, 0.667f, 0.0f };
	ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
	pd3dDevice->ClearRenderTargetView(pRTV, ClearColor);

	// Clear the depth stencil
	ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
	pd3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	return;
}

PingPang::~PingPang() {
	SAFE_RELEASE(pEffect);
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pSwapChain);
	/*		SAFE_DELETE(pSolidState);*/
	if (pBall != NULL)
		delete pBall;

	pBoxVector.clear();

}