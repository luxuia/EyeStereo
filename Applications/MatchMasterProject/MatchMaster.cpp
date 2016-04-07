#include "MatchMaster.hpp"

using namespace EyeStereo;

MatchMaster::MatchMaster() {
	initRadius = 2;
	initDepth = 4;
	pd3dDevice = NULL;
	bactive = false;
	bStereo = false;
}

bool MatchMaster::init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera) {
	printf("%s init start... \n", __FILE__);

	pd3dDevice = pd3d;
	pd3d->AddRef();

	HRESULT hr;
	V_RETURN(D3DX10CreateEffectFromFile(L"../Data/Shader/NormalTexture.fx", NULL, NULL, "fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
		pd3d, NULL, NULL, &pEffect, NULL, NULL));

	pTech = pEffect->GetTechniqueByName("NoSpecular");

	g_ptxDiffuseVariable = pEffect->GetVariableByName("g_MeshTexture")->AsShaderResource();

	g_pAmbient = pEffect->GetVariableByName("g_vMaterialAmbient")->AsVector();
	g_pDiffuse = pEffect->GetVariableByName("g_vMaterialDiffuse")->AsVector();
	g_pSpecular = pEffect->GetVariableByName("g_vMaterialSpecular")->AsVector();
	g_pOpacity = pEffect->GetVariableByName("g_fMaterialAlpha")->AsScalar();
	g_pSpecularPower = pEffect->GetVariableByName("g_nMaterialShininess")->AsScalar();
	g_pLightColor = pEffect->GetVariableByName("g_vLightColor")->AsVector();
	g_pLightPosition = pEffect->GetVariableByName("g_vLightPosition")->AsVector();
	g_pBrightness_left = pEffect->GetVariableByName("g_brightness_left")->AsScalar();
	g_pBrightness_right = pEffect->GetVariableByName("g_brightness_right")->AsScalar();
	//		g_pContrast   = pEffect->GetVariableByName("g_contrast")->AsScalar();
	g_pCameraPosition = pEffect->GetVariableByName("g_vCameraPosition")->AsVector();
	g_pTime = pEffect->GetVariableByName("g_fTime")->AsScalar();
	g_pWorld = pEffect->GetVariableByName("g_mWorld")->AsMatrix();
	g_pWorldViewProjection = pEffect->GetVariableByName("g_mWorldViewProjection")->AsMatrix();


	/*
	pShaderRV = pEffect->GetVariableByName("g_MeshTexture")->AsShaderResource();
	ID3D10ShaderResourceView*	shaderResourceView;
	D3DX10CreateShaderResourceViewFromFile( pd3dDevice,
	L"texture.png", 0, 0, &shaderResourceView, 0 );
	//g_pfxBoxMapVar->SetResource( NULL );
	pShaderRV->SetResource( shaderResourceView );
	shaderResourceView->Release();
	*/
	const D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//glutSolidSphere()

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	D3D10_PASS_DESC PassDesc;
	pTech->GetPassByIndex(0)->GetDesc(&PassDesc);
	pd3d->CreateInputLayout(layout, numElements,
		PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout);

	pd3dDevice->IASetInputLayout(pInputLayout);


	//Init Objects

	MatchMasterObj = new MatchMasterObject();
	MatchMasterObj->pd3dDevice = pd3d;
	MatchMasterObj->pEffect = pEffect;

	//MatchMasterObj->AddObject(L"泳圈", L"../Data/Object/yongquan.obj", D3DXVECTOR3(200, -2, 10), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	//MatchMasterObj->AddObject(L"泳圈", L"../Data/Object/yongquan.obj", D3DXVECTOR3(0, 200, 100), D3DXVECTOR3(0.05, 0.05, 0.05), false);

	MatchMasterObj->AddObject(L"圆锥", L"../Data/Object/yuanzhui.obj", D3DXVECTOR3(-100, -2, 20), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	MatchMasterObj->AddObject(L"圆锥", L"../Data/Object/yuanzhui.obj", D3DXVECTOR3(-10, 150, 20), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	MatchMasterObj->AddObject(L"圆锥", L"../Data/Object/yuanzhui.obj", D3DXVECTOR3(-100, -2, 200), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	MatchMasterObj->AddObject(L"圆锥", L"../Data/Object/yuanzhui.obj", D3DXVECTOR3(-10, 150, 200), D3DXVECTOR3(0.05, 0.05, 0.05), false);

	MatchMasterObj->AddObject(L"圆柱", L"../Data/Object/yuanzhu.obj", D3DXVECTOR3(250, -20, 7), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	MatchMasterObj->AddObject(L"圆柱", L"../Data/Object/yuanzhu.obj", D3DXVECTOR3(350, 105, 7), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	MatchMasterObj->AddObject(L"圆柱", L"../Data/Object/yuanzhu.obj", D3DXVECTOR3(20, -200, 150), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	MatchMasterObj->AddObject(L"圆柱", L"../Data/Object/yuanzhu.obj", D3DXVECTOR3(30, 15, 150), D3DXVECTOR3(0.05, 0.05, 0.05), false);

	//MatchMasterObj->AddObject(L"圆环", L"../Data/Object/yuanhuan.obj", D3DXVECTOR3(-150, -150, 130), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	//MatchMasterObj->AddObject(L"圆环", L"../Data/Object/yuanhuan.obj", D3DXVECTOR3(-46, 100, -10), D3DXVECTOR3(0.05, 0.05, 0.05), false);

	MatchMasterObj->AddObject(L"四棱锥", L"../Data/Object/silinzhui.obj", D3DXVECTOR3(100, -165, 200), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	MatchMasterObj->AddObject(L"四棱锥", L"../Data/Object/silinzhui.obj", D3DXVECTOR3(150, 21, 200), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	MatchMasterObj->AddObject(L"四棱锥", L"../Data/Object/silinzhui.obj", D3DXVECTOR3(20, 49, -89), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	MatchMasterObj->AddObject(L"四棱锥", L"../Data/Object/silinzhui.obj", D3DXVECTOR3(90, -9, -89), D3DXVECTOR3(0.05, 0.05, 0.05), false);

	//MatchMasterObj->AddObject(L"球", L"../Data/Object/qiu.obj", D3DXVECTOR3(231, -30, 200), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	//MatchMasterObj->AddObject(L"球", L"../Data/Object/qiu.obj", D3DXVECTOR3(25, -342, 200), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	//MatchMasterObj->AddObject(L"球", L"../Data/Object/qiu.obj", D3DXVECTOR3(125, 95, 14), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	//MatchMasterObj->AddObject(L"球", L"../Data/Object/qiu.obj", D3DXVECTOR3(41, 19, 14), D3DXVECTOR3(0.05, 0.05, 0.05), false);


	//MatchMasterObj->AddObject(L"茶壶", L"../Data/Object/chahu.obj", D3DXVECTOR3(159, -10, 93), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	//MatchMasterObj->AddObject(L"茶壶", L"../Data/Object/chahu.obj", D3DXVECTOR3(23, -199, 93), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	//MatchMasterObj->AddObject(L"茶壶", L"../Data/Object/chahu.obj", D3DXVECTOR3(-45, -192, 170), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	//MatchMasterObj->AddObject(L"茶壶", L"../Data/Object/chahu.obj", D3DXVECTOR3(-62, 25, 170), D3DXVECTOR3(0.05, 0.05, 0.05), false);

	//MatchMasterObj->AddObject(L"长方形", L"../Data/Object/changfangxin.obj", D3DXVECTOR3(-178, -10, 300), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	//MatchMasterObj->AddObject(L"长方形", L"../Data/Object/changfangxin.obj", D3DXVECTOR3(27, -31, 300), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	//MatchMasterObj->AddObject(L"长方形", L"../Data/Object/changfangxin.obj", D3DXVECTOR3(-16, -15, -300), D3DXVECTOR3(0.05, 0.05, 0.05), false);
	//MatchMasterObj->AddObject(L"长方形", L"../Data/Object/changfangxin.obj", D3DXVECTOR3(120, -18, -300), D3DXVECTOR3(0.05, 0.05, 0.05), false);

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


	pCamera->SetViewParams(&D3DXVECTOR3(0, 2, -5), &D3DXVECTOR3(0, 0, 0));

	return true;

}


void MatchMaster::stereoRender(float fTime) {

	pd3dDevice->IASetInputLayout(pInputLayout);
	pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDevice->RSSetState(pSolidState);



	HRESULT hr;

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mPorject;
	D3DXMATRIX mWVP;

	// ----Draw  Stereo Sphere

	mView = *pCamera->GetViewMatrix();
	mPorject = *pCamera->GetProjMatrix();
	V(g_pBrightness_left->SetFloat((float)g_brightness_left));
	V(g_pBrightness_right->SetFloat((float)g_brightness_right));
	//		V( g_pContrast->SetFloat( (float) g_contrast) );

	float currentSeparaion = pStereoSetting->g_Separation * pStereoSetting->g_EyeSeparation;
	mPorject._31 = mPorject._31 + currentSeparaion;
	mPorject._41 = mPorject._41 - currentSeparaion* pStereoSetting->g_Convergence;
	ClearScreen(pd3dDevice);

	// ----Draw background
	D3DXMATRIX mat;
	for (UINT id = 0; id < MatchMasterObj->num; ++id) {
		D3DXMatrixTranslation(&mWorld, MatchMasterObj->VecObject[id].g_ObjectsPosition.x, MatchMasterObj->VecObject[id].g_ObjectsPosition.y, MatchMasterObj->VecObject[id].g_ObjectsPosition.z);
		mWorld *= *D3DXMatrixScaling(&mat, MatchMasterObj->VecObject[id].g_ObjectResize.x, MatchMasterObj->VecObject[id].g_ObjectResize.y, MatchMasterObj->VecObject[id].g_ObjectResize.z);
		//mWorld*= mat;
		mWVP = mWorld*mView*mPorject;

		// Update the effect's variables. 
		V(g_pWorldViewProjection->SetMatrix((float*)&mWVP));
		V(g_pWorld->SetMatrix((float*)&mWorld));
		V(g_pTime->SetFloat((float)fTime));
		V(g_pCameraPosition->SetFloatVector((float*)pCamera->GetEyePt()));

		for (UINT i = 0; i <MatchMasterObj->VecObject[id].g_ObjectsMesh->GetNumSubsets(); ++i) {
			RenderSubset(id, i);
		}
	}
	// ----End Draw 3...

	pStereoSetting->draw(pd3dDevice, pStereoSetting->LEFT_EYE);


	//Begin Render Right Eye
	ClearScreen(pd3dDevice);

	mPorject = *pCamera->GetProjMatrix();
	mPorject._31 = mPorject._31 - currentSeparaion;
	mPorject._41 = mPorject._41 + currentSeparaion* pStereoSetting->g_Convergence;

	// Draw BackGround
	for (UINT id = 0; id < MatchMasterObj->VecObject.size(); ++id) {
		D3DXMatrixTranslation(&mWorld, MatchMasterObj->VecObject[id].g_ObjectsPosition.x, MatchMasterObj->VecObject[id].g_ObjectsPosition.y, MatchMasterObj->VecObject[id].g_ObjectsPosition.z);
		mWorld *= *D3DXMatrixScaling(&mat, MatchMasterObj->VecObject[id].g_ObjectResize.x, MatchMasterObj->VecObject[id].g_ObjectResize.y, MatchMasterObj->VecObject[id].g_ObjectResize.z);
		/*printf("%f %f %f\n", g_ObjectResize[id].x, g_ObjectResize[id].y, g_ObjectResize[id].z );*/
		mWVP = mWorld*mView*mPorject;
		// Update the effect's variables. 
		V(g_pWorldViewProjection->SetMatrix((float*)&mWVP));
		V(g_pWorld->SetMatrix((float*)&mWorld));
		V(g_pTime->SetFloat((float)fTime));
		V(g_pCameraPosition->SetFloatVector((float*)pCamera->GetEyePt()));

		for (UINT i = 0; i < MatchMasterObj->VecObject[id].g_ObjectsMesh->GetNumSubsets(); ++i) {
			RenderSubset(id, i);
		}
	}

	pStereoSetting->draw(pd3dDevice, pStereoSetting->RIGHT_EYE);



	//非Stereo 时把下面的注释掉 

	if (bStereo) {
		ClearScreen(pd3dDevice);
		pStereoSetting->draw(pd3dDevice);
	}

}

void MatchMaster::RenderSubset(UINT id, UINT iSubSet) {
	HRESULT hr;

	Material *pMaterial = MatchMasterObj->VecObject[id].g_ObjectsMesh->GetSubsetMaterial(iSubSet);
	V(g_pAmbient->SetFloatVector(pMaterial->vAmbient));
	V(g_pDiffuse->SetFloatVector(pMaterial->vDiffuse));
	V(g_pSpecular->SetFloatVector(pMaterial->vSpecular));
	V(g_pOpacity->SetFloat(pMaterial->fAlpha));
	V(g_pSpecularPower->SetInt(pMaterial->nShininess));

	if (!IsErrorResource(pMaterial->pTextureRV10))
		g_ptxDiffuseVariable->SetResource(pMaterial->pTextureRV10);

	D3D10_TECHNIQUE_DESC techDesc;
	pMaterial->pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p) {
		pMaterial->pTechnique->GetPassByIndex(p)->Apply(0);
		MatchMasterObj->VecObject[id].g_ObjectsMesh->GetMesh()->DrawSubset(iSubSet);
	}

}


void MatchMaster::pick(int sx, int sy) {
	//	printf("call pick function\n");
	D3DXMATRIX P = *pCamera->GetProjMatrix();

	// Compute picking ray in view space.
	float vx = (+2.0f*sx / DXUTGetWindowWidth() - 1.0f) / P(0, 0);
	float vy = (-2.0f*sy / DXUTGetWindowHeight() + 1.0f) / P(1, 1);
	float vz = 1.0f;

	D3DXVECTOR3 rayOrigin(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rayDir(vx, vy, vz);

	// Tranform to world space.
	//D3DXMATRIX V = *pCamera->GetViewMatrix();
	const D3DXMATRIX matView = *pCamera->GetViewMatrix();
	const D3DXMATRIX matWorld = *pCamera->GetWorldMatrix();
	D3DXMATRIX V = matWorld * matView;

	D3DXMATRIX inverseV;
	D3DXMatrixInverse(&inverseV, 0, &V);

	D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &inverseV);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &inverseV);



	UINT hitCount;
	float dist, maxDist = INT_MAX;
	int maxIndex = -1;
	ID3D10Blob* allHits;
	// Transform to the mesh's local space.
	D3DXMATRIX inverseW;
	D3DXMATRIX msword;


	for (unsigned i = 0; i < MatchMasterObj->num; i++) {
		ID3DX10Mesh* d3dxmesh = MatchMasterObj->VecObject[i].g_ObjectsMesh->GetMesh();
		D3DXMatrixTranslation(&msword, MatchMasterObj->VecObject[i].g_ObjectsPosition.x, MatchMasterObj->VecObject[i].g_ObjectsPosition.y, MatchMasterObj->VecObject[i].g_ObjectsPosition.z);
		msword *= *D3DXMatrixScaling(&inverseW, MatchMasterObj->VecObject[i].g_ObjectResize.x, MatchMasterObj->VecObject[i].g_ObjectResize.y, MatchMasterObj->VecObject[i].g_ObjectResize.z);/// for "inverseW" tmp usage ;
		D3DXMatrixInverse(&inverseW, 0, &msword);
		D3DXVECTOR3 torigin(rayOrigin), tdir(rayDir);

		D3DXVec3TransformCoord(&torigin, &rayOrigin, &inverseW);
		D3DXVec3TransformNormal(&tdir, &rayDir, &inverseW);

		/*printf("origin(%f, %f, %f)\n dir(%f, %f, %f)\n",
		rayOrigin.x, rayOrigin.y, rayOrigin.z,
		rayDir.x, rayDir.y, rayDir.z);*/
		d3dxmesh->Intersect(&torigin, &tdir, &hitCount,
			NULL, NULL, NULL, &dist, NULL);
		if (hitCount > 0) {
			if (maxDist > dist) {
				maxDist = dist;
				maxIndex = i;
			}
		}
	}
	if (maxIndex != -1)
	{
		pick_cnt++;
		Choose_ID[pick_cnt % 2] = MatchMasterObj->VecObject[maxIndex].g_ObjectNum;
//		MatchMasterObj->VecObject[maxIndex].g_ObjectFlag = false;
		if (pick_cnt & 1) {
//			if ((!MatchMasterObj->VecObject[Choose_ID[0]].g_ObjectFlag) && (!MatchMasterObj->VecObject[Choose_ID[1]].g_ObjectFlag))
			if (MatchMasterObj->VecObject[Choose_ID[0]].g_ObjectNum != MatchMasterObj->VecObject[Choose_ID[1]].g_ObjectNum) {
				if (MatchMasterObj->VecObject[Choose_ID[0]].g_ObjectID == MatchMasterObj->VecObject[Choose_ID[1]].g_ObjectID) {
					if (MatchMasterObj->VecObject[Choose_ID[0]].g_ObjectsPosition.z == MatchMasterObj->VecObject[Choose_ID[1]].g_ObjectsPosition.z) {
						swprintf(mShareMessage, L"恭喜您,成功消掉了一对%s,得到3分!", MatchMasterObj->VecObject[maxIndex].g_ObjectsName);
						Score += 3;
						MatchMasterObj->DeletObject(Choose_ID[0], Choose_ID[1]);
					}
					else {
						swprintf(mShareMessage, L"很遗憾,这是不同深度的一对%s", MatchMasterObj->VecObject[maxIndex].g_ObjectsName);
					}
				
				}
				else {
					swprintf(mShareMessage, L"抱歉,这不是同样的物体");
				}
			}	
		}
		else
		{
			swprintf(mShareMessage, L"你选中了%s", MatchMasterObj->VecObject[maxIndex].g_ObjectsName);
		}
	}
	else
	{
		swprintf(mShareMessage, L"");
	}
	*mShareChanged = true;
}

void MatchMaster::HandMessages(UINT uMsg, LPARAM lParam) {

	switch (uMsg) {
	case WM_LBUTTONDOWN:
		pick(LOWORD(lParam), HIWORD(lParam));
		return;
	};
}

void MatchMaster::ShareMessage(WCHAR* sharestr, bool *sharechanged) {
	mShareChanged = sharechanged;
	mShareMessage = sharestr;
}


MatchMaster::~MatchMaster() {
	SAFE_RELEASE(pd3dDevice);
	SAFE_RELEASE(pEffect);
	//pTech;
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pSolidState);
	//SAFE_RELEASE(pShaderRV);
	//delete pBox;
	// 		delete pSphere;
	// 		delete pBackground;
	MatchMasterObj->VecObject.clear();

}



void MatchMaster::ClearScreen(ID3D10Device *pd3dDevice)
{
	float ClearColor[4] = { 0.176f, 0.196f, 0.667f, 0.0f };
	ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
	pd3dDevice->ClearRenderTargetView(pRTV, ClearColor);

	// Clear the depth stencil
	ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
	pd3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	return;
}