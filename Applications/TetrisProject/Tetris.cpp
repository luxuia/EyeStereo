#include "Tetris.h"

using namespace EyeStereo;

bool Tetris::init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera, KeyBoard* pkeyboard) {
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

	g_CubeMesh = new CMeshLoader10();
	g_pMeshHelper->CreateMesh(g_CubeMesh, L"../Data/Object/cube.obj");
	g_BackGroundMesh = new CMeshLoader10();
	g_pMeshHelper->CreateMesh(g_BackGroundMesh, L"../Data/Object/earth.obj");
	g_BackGroundPosition = D3DXVECTOR3(0, -1, CONST_Z_);

	g_pTetrisCreator = new TetrisCubesCreator(g_CubeMesh);

	g_pTetrisCreator->CreateRandomCube(TetrisCubes::ACTIVE);
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
	rsDesc.CullMode = D3D10_CULL_BACK;
	rsDesc.FrontCounterClockwise = false;

	pd3dDevice->CreateRasterizerState(&rsDesc, &pSolidState);
	pCamera->SetViewParams(&D3DXVECTOR3(0, 3, -20), &D3DXVECTOR3(0, 0, 0));
	g_ActiveCube = -1;
	return true;

}

void Tetris::Restart() {
	g_pTetrisCreator->Cubes.clear();
	g_pTetrisCreator->CreateRandomCube(TetrisCubes::ACTIVE);
}

UINT Tetris::FindConnectedCubes(std::vector< uupair > &stack, TetrisCubes::Status dir = TetrisCubes::NONE) {

	if (dir == TetrisCubes::NONE) {
		return FindConnectedCubes(stack, TetrisCubes::LEFT) + FindConnectedCubes(stack, TetrisCubes::RIGHT);
	}
	
	UINT st, nd;
	for (UINT i(0); i < g_pTetrisCreator->Cubes.size(); ++i) {
		for (UINT p(0); p < g_pTetrisCreator->Cubes[i].WordPos.size(); ++p ) {
			if ( std::find( stack.begin(), stack.end(), std::make_pair(i, p) ) == stack.end() ) {
				st = stack.at(0).first;
				nd = stack.at(0).second;
				if (g_pTetrisCreator->Cubes[st].CollisionOne(nd, g_pTetrisCreator->CubeMesh->GetMesh(), g_pTetrisCreator->Cubes[i].WordPos[p], dir, 100)) {
					stack.push_back(std::make_pair(i, p));
					return FindConnectedCubes(stack, dir) + 1;
				}
			}
		}
	}
	return 1;
}

bool _pairCMP(const Tetris::uupair& a, const Tetris::uupair& b) {
		if (a.first != b.first)
			return a.first > b.first;
		else
			return b.second < a.second;
	}


void Tetris::DestoryCubes(std::vector<uupair > &stack) {

	
	std::sort(stack.begin(), stack.end(), _pairCMP);
	for (UINT i = 0; i < stack.size(); ++i) {
		std::vector< D3DXVECTOR3 > &pos = g_pTetrisCreator->Cubes[stack[i].first].WordPos;
		pos.erase(pos.begin()+stack[i].second, pos.begin()+stack[i].second+1);

		//release empty Cubes;
		if (pos.empty()/*g_pTetrisCreator->Cubes[stack[i].first].WordPos.empty()*/) {
			g_pTetrisCreator->Cubes.erase(g_pTetrisCreator->Cubes.begin()+stack[i].first, g_pTetrisCreator->Cubes.begin()+stack[i].first+1);
		}
	}
}

void Tetris::DeleteIfFullLine() {
	std::vector< uupair > _stack;
	int isDestoryed = FALSE;
	for (UINT i = 0; i < g_pTetrisCreator->Cubes.size(); ++i) {
		for (UINT p = 0; p < g_pTetrisCreator->Cubes[i].WordPos.size(); ++p) {
			_stack.clear();
			_stack.push_back(std::pair<UINT, UINT>(i, p));
			if (FindConnectedCubes(_stack, TetrisCubes::NONE) > 10) {
				
				DestoryCubes(_stack); 
				isDestoryed = TRUE;
				if (g_pTetrisCreator->Cubes.empty())
					break;
			}
		}
	}
		if (isDestoryed){
			for (UINT i = 0; i < g_pTetrisCreator->Cubes.size(); ++i) {
				if (g_pTetrisCreator->Cubes[i].status == TetrisCubes::NONE)
						g_pTetrisCreator->Cubes[i].status = TetrisCubes::DOWN;
			}
		}
	
	
}


bool Tetris::CollisionALL(UINT i, TetrisCubes::Status status) {
	int isCollision = FALSE;
	for (UINT p(0); p < g_pTetrisCreator->Cubes.size(); ++p) {
		if (i != p)
			if ( g_pTetrisCreator->Cubes[i].Collision(&(g_pTetrisCreator->Cubes[p]), status, g_fObjectMoveSpace/10)/*Collision(i, p, status, g_fObjectMoveSpace/10*/ ) {
				isCollision = TRUE;
				break;
			}
	}
	if (status == TetrisCubes::DOWN && !isCollision) {
		isCollision = g_pTetrisCreator->Cubes[i].Collision(g_BackGroundMesh->GetMesh(), g_BackGroundPosition, TetrisCubes::DOWN, g_fObjectMoveSpace / 10);
	}
	return isCollision;
}

void Tetris::Rotate90() {
	if (g_pTetrisCreator->Cubes.size() >= g_ActiveCube ) {
		g_pTetrisCreator->Cubes[g_ActiveCube].Rotate90();
	}
}

void Tetris::UpdateTetrisPosition(float fTime) {
	int flgActive = 0;
	static float waitToMove = 0;
	for (UINT i = 0; i < g_pTetrisCreator->Cubes.size(); ++i) {
		if (g_pTetrisCreator->Cubes[i].status == TetrisCubes::ACTIVE) {
			flgActive = i+1;
			g_ActiveCube = i;
		}
	}

	if (flgActive ==  0) {
		puts("Create Cubes");
		g_pTetrisCreator->CreateRandomCube(TetrisCubes::ACTIVE);
		waitToMove = 0;
	} 


	if (fTime - g_fLastMoveTime < g_fObjectMoveSpace/100) {
		return;
	}

	TetrisCubes::Status status = TetrisCubes::NONE;
	
	if ( (*g_pKeyBoard)._upDownRightLeft[1]) {
		status = TetrisCubes::DOWN;
	} else if ( (*g_pKeyBoard)._upDownRightLeft[2]) {
		status = TetrisCubes::RIGHT;
	} else if ( (*g_pKeyBoard)._upDownRightLeft[3]) {
		status = TetrisCubes::LEFT;
	} 
	if (status != TetrisCubes::NONE) {
		for (UINT i(0); i < g_pTetrisCreator->Cubes.size(); ++i) {
			if (g_pTetrisCreator->Cubes[i].status == TetrisCubes::ACTIVE) {
				
				if (!CollisionALL(i, status)) {
					if (status == TetrisCubes::DOWN) {
						g_pTetrisCreator->Cubes[i].Move(g_fObjectMoveSpace / 20, TetrisCubes::DOWN);
					}
					else if (status == TetrisCubes::LEFT){
						while (waitToMove > 0-Elapse) waitToMove -= 1;
					}
					else if (status == TetrisCubes::RIGHT) {
						while (waitToMove < 0+Elapse) waitToMove += 1;
					}

				}
				break;
			}
		}
	}
	for (UINT i(0); i < g_pTetrisCreator->Cubes.size(); ++i) {
		if (g_pTetrisCreator->Cubes[i].status == TetrisCubes::ACTIVE) {
			if (waitToMove > Elapse) {
				g_pTetrisCreator->Cubes[i].Move(g_fObjectMoveSpace / 10, TetrisCubes::RIGHT);
				waitToMove -= g_fObjectMoveSpace/10;
			} else if (waitToMove < -Elapse) {
				g_pTetrisCreator->Cubes[i].Move(g_fObjectMoveSpace / 10, TetrisCubes::LEFT);
				waitToMove += g_fObjectMoveSpace/10;
			}
		}
	}
	for (UINT i(0); i < g_pTetrisCreator->Cubes.size(); ++i) {
		int isCollision = FALSE;
		if (g_pTetrisCreator->Cubes[i].status & TetrisCubes::DOWN) {
			for (UINT p(0); p <g_pTetrisCreator->Cubes.size(); ++p) {
				if (p != i )
				if (g_pTetrisCreator->Cubes[i].Collision(&(g_pTetrisCreator->Cubes[p]), TetrisCubes::DOWN)) {
						puts("Collision Cubes");
						isCollision = TRUE;
						break;
					}
			}
			if (!isCollision && g_pTetrisCreator->Cubes[i].Collision(g_BackGroundMesh->GetMesh(), g_BackGroundPosition) ) {
				puts("Collision BackGround");
				isCollision = TRUE;
			}
			if (isCollision ) {
				puts("Collision");
				if (abs(waitToMove) > Elapse ) {
					g_pTetrisCreator->Cubes[i].Move(waitToMove);
				}
				g_pTetrisCreator->Cubes[i].status = TetrisCubes::NONE;
				DeleteIfFullLine();
				
			}
			
			
			if (fTime - g_fLastMoveTime > g_fObjectMoveSpace/100 && !isCollision) {
				//printf("Time ON current: %0.2f   LastTime: %0.2f\n", fTime, g_fLastMoveTime);
				
				g_pTetrisCreator->Cubes[i].Move(g_fObjectMoveSpace / 100, TetrisCubes::DOWN);
			}
		}
	}
	g_fLastMoveTime = fTime;

}

void Tetris::stereoRender(float fTime) {

	pd3dDevice->IASetInputLayout(pInputLayout);
	pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDevice->RSSetState(pSolidState);

	UpdateTetrisPosition(fTime);

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

	// ----Draw background
	TetrisCubes tmpCubes;
	for (UINT i = 0; i < g_pTetrisCreator->Cubes.size(); ++i) {
		if (g_pTetrisCreator->Cubes[i].status != TetrisCubes::INVISIABLE) {
			tmpCubes = g_pTetrisCreator->Cubes[i];
			for (UINT p(0); p < tmpCubes.WordPos.size(); ++p) {
				D3DXMatrixTranslation(&mWorld, tmpCubes.WordPos[p].x, tmpCubes.WordPos[p].y, tmpCubes.WordPos[p].z);
				mWVP = mWorld*mView*mPorject;
				g_pMeshHelper->RenderMesh(g_pTetrisCreator->CubeMesh, mWorld, mWVP, fTime);

			}
		}
	}

	D3DXMatrixTranslation(&mWorld, g_BackGroundPosition.x, g_BackGroundPosition.y, g_BackGroundPosition.z);
	mWVP = mWorld*mView*mPorject;
	g_pMeshHelper->RenderMesh(g_BackGroundMesh, mWorld, mWVP, fTime);

	pStereoSetting->draw(pd3dDevice, pStereoSetting->LEFT_EYE);


	//Begin Render Right Eye
	ClearScreen(pd3dDevice);

	mPorject = *pCamera->GetProjMatrix();
	mPorject._31 = mPorject._31 - currentSeparaion;
	mPorject._41 = mPorject._41 + currentSeparaion* pStereoSetting->g_Convergence;

	for (UINT i = 0; i < g_pTetrisCreator->Cubes.size(); ++i) {
		if (g_pTetrisCreator->Cubes[i].status != TetrisCubes::INVISIABLE) {
			tmpCubes = g_pTetrisCreator->Cubes[i];
			for (UINT p(0); p < tmpCubes.WordPos.size(); ++p) {
				D3DXMatrixTranslation(&mWorld, tmpCubes.WordPos[p].x, tmpCubes.WordPos[p].y, tmpCubes.WordPos[p].z);
				mWVP = mWorld*mView*mPorject;
				g_pMeshHelper->RenderMesh(g_pTetrisCreator->CubeMesh, mWorld, mWVP, fTime);

			}
		}
	}

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


void Tetris::pick(int sx, int sy) {
	printf("call pick function\n");
	D3DXMATRIX P = *pCamera->GetProjMatrix();

	// Compute picking ray in view space.
	float vx = (+2.0f*sx/DXUTGetWindowWidth()  - 1.0f)/P(0,0);
	float vy = (-2.0f*sy/DXUTGetWindowHeight() + 1.0f)/P(1,1);

	D3DXVECTOR3 rayOrigin(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rayDir(vx, vy, 1.0f);

	// Tranform to world space.
	D3DXMATRIX V = *pCamera->GetViewMatrix();

	D3DXMATRIX inverseV;
	D3DXMatrixInverse(&inverseV, 0, &V);

	D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &inverseV);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &inverseV);




	UINT hitCount;
	float u,v,t;
	// Transform to the mesh's local space.
	D3DXMATRIX inverseW;

	D3DXMATRIX msword;

// 
// 	for(unsigned i = 0; i < g_ObjectsPosition.size(); i++) {
// 		ID3DX10Mesh* d3dxmesh = g_ObjectsMesh[i]->GetMesh();
// 		D3DXMatrixTranslation(&msword, g_ObjectsPosition[i].x, g_ObjectsPosition[i].y, g_ObjectsPosition[i].z);
// 		D3DXMatrixInverse(&inverseW, 0, &msword);
// 		D3DXVECTOR3 torigin(rayOrigin), tdir(rayDir);
// 
// 		D3DXVec3TransformCoord(&torigin, &rayOrigin, &inverseW);
// 		D3DXVec3TransformNormal(&tdir, &rayDir, &inverseW);
// 
// 		printf("origin(%f, %f, %f)\n dir(%f, %f, %f)\n", 
// 			rayOrigin.x, rayOrigin.y, rayOrigin.z, 
// 			rayDir.x, rayDir.y, rayDir.z);
// 		d3dxmesh->Intersect(&torigin, &tdir, &hitCount, 
// 			NULL, &u, &v, &t, NULL);
// 		if (hitCount > 0) {
// 			swprintf(mShareMessage, L"你选中了%s\n", g_ObjectsName[i]);
// 			*mShareChanged = true;
// 			return;
// 		}
// 
// 	}

}