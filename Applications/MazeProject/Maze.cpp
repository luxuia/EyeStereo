#include "Maze.hpp"

using namespace EyeStereo;

Maze::Maze() {
	initRadius = 2;
	initDepth = 4;
	pd3dDevice = NULL;
	bactive = false;
	bStereo = false;
}

bool Maze::init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera) {
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
	MazeObj = new MazeObject();
	MazeObj->pd3dDevice = pd3d;
	MazeObj->pEffect = pEffect;
	MazeObj->AddObject(L"泳圈", L"../Data/Object/yongquan.obj", D3DXVECTOR3(200, -2, 10), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	MazeObj->AddObject(L"圆锥", L"../Data/Object/yuanzhui.obj", D3DXVECTOR3(-100, -2, 20), D3DXVECTOR3(0.05, 0.05, 0.05), true);
	MazeObj->AddObject(L"圆柱", L"../Data/Object/yuanzhu.obj", D3DXVECTOR3(250, -20, 7), D3DXVECTOR3(0.05, 0.05, 0.05), true);


	MazeObj->AddObject(L"圆环", L"../Data/Object/yuanhuan.obj", D3DXVECTOR3(-150, -150, 130), D3DXVECTOR3(0.05, 0.05, 0.05), true);


	MazeObj->AddObject(L"四棱锥", L"../Data/Object/silinzhui.obj", D3DXVECTOR3(100, -17, 200), D3DXVECTOR3(0.05, 0.05, 0.05), true);


	MazeObj->AddObject(L"球", L"../Data/Object/qiu.obj", D3DXVECTOR3(25, -30, 200), D3DXVECTOR3(0.05, 0.05, 0.05), true);


	MazeObj->AddObject(L"茶壶", L"../Data/Object/chahu.obj", D3DXVECTOR3(-120, -10, 93), D3DXVECTOR3(0.05, 0.05, 0.05), true);

	MazeObj->AddObject(L"长方形", L"../Data/Object/changfangxin.obj", D3DXVECTOR3(-150, -10, 300), D3DXVECTOR3(0.05, 0.05, 0.05), true);




/*	for (UINT id = 0; id < MazeObj->num; ++id) {
		CMeshLoader10* mesh = new CMeshLoader10();
		V_RETURN(mesh->Create(pd3dDevice, vec_Obj[id].g_ObjectsPath));
		wprintf(L"Loading Mesh: %s\n", vec_Obj[id].g_ObjectsPath);
		for (UINT i = 0; i < mesh->GetNumMaterials(); ++i) {
			Material* pMaterial = mesh->GetMaterial(i);

			const char* strTechnique = "";

			if (pMaterial->pTextureRV10 && pMaterial->bSpecular)
				strTechnique = "TexturedSpecular";
			else if (pMaterial->pTextureRV10 && !pMaterial->bSpecular)
				strTechnique = "TexturedNoSpecular";
			else if (!pMaterial->pTextureRV10 && pMaterial->bSpecular)
				strTechnique = "Specular";
			else if (!pMaterial->pTextureRV10 && !pMaterial->bSpecular)
				strTechnique = "NoSpecular";

			pMaterial->pTechnique = pEffect->GetTechniqueByName(strTechnique);
		}
		vec_Obj[id].g_ObjectsMesh = mesh;
	}
	*/


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


void Maze::stereoRender(float fTime) {

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
	for (UINT id = 0; id < MazeObj->num; ++id) {
		D3DXMatrixTranslation(&mWorld, MazeObj->VecObject[id].g_ObjectsPosition.x, MazeObj->VecObject[id].g_ObjectsPosition.y, MazeObj->VecObject[id].g_ObjectsPosition.z);
		mWorld *= *D3DXMatrixScaling(&mat,MazeObj->VecObject[id].g_ObjectResize.x,MazeObj->VecObject[id].g_ObjectResize.y,MazeObj->VecObject[id].g_ObjectResize.z);
		//mWorld*= mat;
		mWVP = mWorld*mView*mPorject;

		// Update the effect's variables. 
		V(g_pWorldViewProjection->SetMatrix((float*)&mWVP));
		V(g_pWorld->SetMatrix((float*)&mWorld));
		V(g_pTime->SetFloat((float)fTime));
		V(g_pCameraPosition->SetFloatVector((float*)pCamera->GetEyePt()));

		for (UINT i = 0; i <MazeObj->VecObject[id].g_ObjectsMesh->GetNumSubsets(); ++i) {
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
	for (UINT id = 0; id < MazeObj->num; ++id) {
		D3DXMatrixTranslation(&mWorld, MazeObj->VecObject[id].g_ObjectsPosition.x, MazeObj->VecObject[id].g_ObjectsPosition.y, MazeObj->VecObject[id].g_ObjectsPosition.z);
		mWorld *= *D3DXMatrixScaling(&mat,MazeObj->VecObject[id].g_ObjectResize.x,MazeObj->VecObject[id].g_ObjectResize.y,MazeObj->VecObject[id].g_ObjectResize.z);
		/*printf("%f %f %f\n", g_ObjectResize[id].x, g_ObjectResize[id].y, g_ObjectResize[id].z );*/
		mWVP = mWorld*mView*mPorject;
		// Update the effect's variables. 
		V(g_pWorldViewProjection->SetMatrix((float*)&mWVP));
		V(g_pWorld->SetMatrix((float*)&mWorld));
		V(g_pTime->SetFloat((float)fTime));
		V(g_pCameraPosition->SetFloatVector((float*)pCamera->GetEyePt()));

		for (UINT i = 0; i < MazeObj->VecObject[i].g_ObjectsMesh->GetNumSubsets(); ++i) {
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

void Maze::RenderSubset(UINT id, UINT iSubSet) {
	HRESULT hr;

	Material *pMaterial = MazeObj->VecObject[id].g_ObjectsMesh->GetSubsetMaterial(iSubSet);
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
		MazeObj->VecObject[id].g_ObjectsMesh->GetMesh()->DrawSubset(iSubSet);
	}

}


void Maze::pick(int sx, int sy) {
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


	for (unsigned i = 0; i < MazeObj->num; i++) {
		ID3DX10Mesh* d3dxmesh = MazeObj->VecObject[i].g_ObjectsMesh->GetMesh();
		D3DXMatrixTranslation(&msword, MazeObj->VecObject[i].g_ObjectsPosition.x, MazeObj->VecObject[i].g_ObjectsPosition.y, MazeObj->VecObject[i].g_ObjectsPosition.z);
		msword *= *D3DXMatrixScaling(&inverseW, MazeObj->VecObject[i].g_ObjectResize.x, MazeObj->VecObject[i].g_ObjectResize.y, MazeObj->VecObject[i].g_ObjectResize.z);/// for "inverseW" tmp usage ;
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
	if(maxIndex != -1)
		swprintf(mShareMessage, L"你选中了%s\n", MazeObj->VecObject[maxIndex].g_ObjectsName); 
	else
		swprintf(mShareMessage, L"");
	*mShareChanged = true;
	
	
}

void Maze::HandMessages(UINT uMsg, LPARAM lParam) {

	switch (uMsg) {
	case WM_LBUTTONDOWN:
		pick(LOWORD(lParam), HIWORD(lParam));
		return;
	};
}

void Maze::ShareMessage(WCHAR* sharestr, bool *sharechanged) {
	mShareChanged = sharechanged;
	mShareMessage = sharestr;
}


Maze::~Maze() {
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

}



void Maze::ClearScreen(ID3D10Device *pd3dDevice)
{
	float ClearColor[4] = { 0.176f, 0.196f, 0.667f, 0.0f };
	ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
	pd3dDevice->ClearRenderTargetView(pRTV, ClearColor);

	// Clear the depth stencil
	ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
	pd3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
	return;
}

