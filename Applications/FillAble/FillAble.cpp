#include "FillAble.h"

using namespace EyeStereo;

static const float ObjectResizeScale = 0.1f;


static const std::wstring screw[] = {
	L"../Data/Object/Circle_out.obj",
};

static const std::wstring nut[] = {
	L"../Data/Object/Circle_in.obj"
};

bool FillAble::init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera, KeyBoard* pkeyboard) {
	printf("%s init start... \n", __FILE__);
	pd3dDevice = pd3d;
	pd3d->AddRef();


	g_pMeshHelper = new MeshHelper(pd3dDevice, pEffect, L"../Data/Shader/NormalTexture.fx");

	HRESULT hr;

	const D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	//glutSolidSphere()

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	D3D10_PASS_DESC PassDesc;
	g_pMeshHelper->pTech->GetPassByIndex(0)->GetDesc( &PassDesc );
	pd3d->CreateInputLayout( layout, numElements, 
		PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout);

	pd3dDevice->IASetInputLayout(pInputLayout);

	g_BackGroundMesh = new CMeshLoader10();
	g_pMeshHelper->CreateMesh(g_BackGroundMesh, L"../Data/Object/earth.obj");
	g_BackGroundPosition = D3DXVECTOR3(0, 0, 0);

	g_pFillAbleCreator = new ModelObjectCreator(*g_pMeshHelper, screw, nut, sizeof(screw)/sizeof(screw[0]));

	g_pFillAbleCreator->CreateRandomCube( ModelObject::ACTIVE); //// depth position in screen
	g_pKeyBoard = pkeyboard;

	pSwapChain = DXUTGetDXGISwapChain();
	pSwapChain->AddRef();

	windowsHeight = DXUTGetWindowHeight();
	windowsWidth = DXUTGetWindowWidth();

	pStereoSetting = pstereo;
	pCamera = pcamera;

	bactive = false;
	bStereo = false;
	g_fLastMoveTime = 0;
	g_fObjectMoveSpace = 1;  //1 second

	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));


	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;

	pd3dDevice->CreateRasterizerState(&rsDesc, &pSolidState);

	g_ActiveCube = -1;
	return true;

}

void FillAble::Restart() {
	g_pFillAbleCreator->CreateRandomCube(ModelObject::ACTIVE);
}


bool FillAble::Collision(ModelObject::Status status) {
	int isCollision = FALSE;

	D3DXVECTOR3 pos(g_pFillAbleCreator->ScrewObject->TestMove(g_fObjectMoveSpace / 10, status));

	/////// TODO Change the collision judge to auto, common
	if (pos.y < 25 + Elapse) {
		if (pos.x*pos.x + pos.z*pos.z <= 54*54 && pos.x*pos.x + pos.z*pos.z > 1) {
			return true;
		}
	}
	if (status == ModelObject::DOWN && !isCollision) {
		isCollision = g_pFillAbleCreator->ScrewObject->Collision(g_BackGroundMesh->GetMesh(), g_BackGroundPosition, ModelObject::DOWN, g_fObjectMoveSpace / 10);
	}
	return isCollision;
}

void FillAble::UpdateObjectPosition(float fTime) {
	static float waitToMove = 0;
	if (fTime - g_fLastMoveTime < g_fObjectMoveSpace/100) {
		return;
	}
	ModelObject::Status status = ModelObject::NONE;
	
	if ( (*g_pKeyBoard)._upDownRightLeft[1]) {
		status = ModelObject::DOWN;
	} else if ( (*g_pKeyBoard)._upDownRightLeft[2]) {
		status = ModelObject::RIGHT;
	} else if ( (*g_pKeyBoard)._upDownRightLeft[3]) {
		status = ModelObject::LEFT;
	} 
	if (status != ModelObject::NONE) {
			if (g_pFillAbleCreator->ScrewObject->status == ModelObject::ACTIVE) {
				if (!Collision(status)) {
					if (status == ModelObject::DOWN) {
						g_pFillAbleCreator->ScrewObject->Move(g_fObjectMoveSpace / 10, ModelObject::DOWN);
					}
					else if (status == ModelObject::LEFT){
						g_pFillAbleCreator->ScrewObject->Move(g_fObjectMoveSpace / 10, ModelObject::LEFT);
					}
					else if (status == ModelObject::RIGHT) {
						g_pFillAbleCreator->ScrewObject->Move(g_fObjectMoveSpace / 10, ModelObject::RIGHT);
					}
				}
			}
	}
	g_fLastMoveTime = fTime;

}

void FillAble::stereoRender(float fTime) {

	pd3dDevice->IASetInputLayout(pInputLayout);
	pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDevice->RSSetState(pSolidState);

	UpdateObjectPosition(fTime);

	HRESULT hr;

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mPorject;
	D3DXMATRIX mWVP;

	// ----Draw  Stereo Sphere

	mView = *pCamera->GetViewMatrix();
	mPorject = *pCamera->GetProjMatrix();

	float currentSeparaion = pStereoSetting->g_Separation * pStereoSetting->g_EyeSeparation;
	mPorject._31 = mPorject._31 + currentSeparaion;
	mPorject._41 = mPorject._41 - currentSeparaion* pStereoSetting->g_Convergence;
	ClearScreen(pd3dDevice);
	D3DXMATRIXA16 mat;

	//Begin Render Right Eye
	// ----Draw background
	/// Render Screw
	D3DXMatrixTranslation(&mWorld, g_pFillAbleCreator->ScrewObject->WordPos.x, g_pFillAbleCreator->ScrewObject->WordPos.y, g_pFillAbleCreator->ScrewObject->WordPos.z);
	mWorld *= *D3DXMatrixScaling(&mat, ObjectResizeScale, ObjectResizeScale, ObjectResizeScale);
	mWVP = mWorld*mView*mPorject;
	g_pMeshHelper->RenderMesh(g_pFillAbleCreator->ScrewObject->mesh, mWorld, mWVP, fTime);

	/// Render Nut
	D3DXMatrixTranslation(&mWorld, g_pFillAbleCreator->NutObject->WordPos.x, g_pFillAbleCreator->NutObject->WordPos.y, g_pFillAbleCreator->NutObject->WordPos.z);
	mWorld *= *D3DXMatrixScaling(&mat, ObjectResizeScale, ObjectResizeScale, ObjectResizeScale);
	mWVP = mWorld*mView*mPorject;
	g_pMeshHelper->RenderMesh(g_pFillAbleCreator->NutObject->mesh, mWorld, mWVP, fTime);


	//// Render Background
	D3DXMatrixTranslation(&mWorld, g_BackGroundPosition.x, g_BackGroundPosition.y, g_BackGroundPosition.z);
	mWVP = mWorld*mView*mPorject;
	g_pMeshHelper->RenderMesh(g_BackGroundMesh, mWorld, mWVP, fTime);

	pStereoSetting->draw(pd3dDevice, pStereoSetting->LEFT_EYE);


	//Begin Render Right Eye
	ClearScreen(pd3dDevice);

	mPorject = *pCamera->GetProjMatrix();
	mPorject._31 = mPorject._31 - currentSeparaion;
	mPorject._41 = mPorject._41 + currentSeparaion* pStereoSetting->g_Convergence;
	
	D3DXMatrixTranslation(&mWorld, g_pFillAbleCreator->ScrewObject->WordPos.x, g_pFillAbleCreator->ScrewObject->WordPos.y, g_pFillAbleCreator->ScrewObject->WordPos.z);
	mWorld *= *D3DXMatrixScaling(&mat, ObjectResizeScale, ObjectResizeScale, ObjectResizeScale);
	mWVP = mWorld*mView*mPorject;
	g_pMeshHelper->RenderMesh(g_pFillAbleCreator->ScrewObject->mesh, mWorld, mWVP, fTime);

	/// Render Nut
	D3DXMatrixTranslation(&mWorld, g_pFillAbleCreator->NutObject->WordPos.x, g_pFillAbleCreator->NutObject->WordPos.y, g_pFillAbleCreator->NutObject->WordPos.z);
	mWorld *= *D3DXMatrixScaling(&mat, ObjectResizeScale, ObjectResizeScale, ObjectResizeScale);
	mWVP = mWorld*mView*mPorject;
	g_pMeshHelper->RenderMesh(g_pFillAbleCreator->NutObject->mesh, mWorld, mWVP, fTime);


	//// Render Background
	D3DXMatrixTranslation(&mWorld, g_BackGroundPosition.x, g_BackGroundPosition.y, g_BackGroundPosition.z);
	mWVP = mWorld*mView*mPorject;
	g_pMeshHelper->RenderMesh(g_BackGroundMesh, mWorld, mWVP, fTime);

	pStereoSetting->draw(pd3dDevice, pStereoSetting->RIGHT_EYE);



	//非Stereo 时把下面的注释掉 

	if (bStereo) {
		ClearScreen(pd3dDevice);
		pStereoSetting->draw(pd3dDevice);
	}

}

void renderScrewNutBackGround() {

}
