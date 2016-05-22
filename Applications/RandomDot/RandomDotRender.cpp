#include "RandomDotRender.hpp"
#include "digit_numbers.h"

using namespace EyeStereo;


RandomDotRender::RandomDotRender() {
	pd3dDevice = NULL;
}

RandomDotRender:: ~RandomDotRender() {
	SAFE_RELEASE(pd3dDevice);
	SAFE_RELEASE(pEffect);
	//pTech;
	SAFE_RELEASE(pInputLayout);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pWireFrameRS);
	delete pRandomDot;
}



Geometry* RandomDotRender::getGeometry(Geometry *newGeo) {
	//Geometry* newGeo = new Geometry(n);

	return newGeo;
}

void RandomDotRender::makeDot(float dotDist) {

	//Geometry *Geo = new Geometry(1);
	/// TODO All Geo should drived from Geometry class and have same interface.
	pGeo = new digit_numbers();
	digit_numbers* _geo = (digit_numbers*)pGeo;
	//Dn->random_number();
	
	_geo->Scale(0.5);
	_geo->Pos(0.5, 0.5);
	_geo->generate();
	_geo->Select();
	pRandomDot = new RandomDot(maxPointNum);

	pRandomDot->dotDist = dotDist;

	// Generate RandomDot BoundX, BoundY, PointNum,  shap to describe 
	pRandomDot->init(pd3dDevice, 6, 6, pointNum, _geo);
	//delete Geo;
}

bool RandomDotRender::init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera, float dotDist, KeyBoard *key) {
	pd3dDevice = pd3d;
	pd3d->AddRef();
	pKeyBoard = key;

	HRESULT hr;
	WCHAR fxpath[MAX_PATH];
	if (FAILED( D3DX10CreateEffectFromFile( L"../Data/Shader/PointToSquare.fx", NULL, NULL, "fx_4_0", 
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
		pd3d, NULL, NULL, &pEffect, NULL, NULL ) ) ) {
			MessageBox( DXUTGetHWND(), L"Can't find fx file Pos4Color4.fx", DXUTGetWindowTitle(), MB_ICONERROR | MB_OK );
			//Log::Get().Write(L"Can't find fx file: Pos4Color4.fx.");
	}

	pTech = pEffect->GetTechniqueByName("RenderScene");

	pMatWorldVarible = pEffect->GetVariableByName("World")->AsMatrix();
	pMatView = pEffect->GetVariableByName("View")->AsMatrix();
	pMatProjection = pEffect->GetVariableByName("Projection")->AsMatrix();
	pFloatPointSize = pEffect->GetVariableByName("PointSize")->AsScalar();

	D3D10_INPUT_ELEMENT_DESC layout[] = {

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0}, 
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
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
	bIsfinish = false;

	D3DXMatrixIdentity(&matWorld);

	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));

	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;

	pd3dDevice->CreateRasterizerState(&rsDesc, &pWireFrameRS);

	y = 0;
	x = -4;

	makeDot(dotDist);
	return true;

}

void RandomDotRender::setOriginPosition(float x1, float y1) {
	this->x1 = x1;
	this->y1 = y1;
}
 
bool RandomDotRender::stereoRender(float fTime) {
	pRandomDot->iVertexNum = pointNum;
	pRandomDot->updateVertex(fTime);
	
	updatePosition(fTime);

	pFloatPointSize->SetFloat(pointSize);
	//pRandomDot->updateBuffer();

	ClearScreen(pd3dDevice);
	renderLeft();
	pStereoSetting->draw(pd3dDevice, pStereoSetting->LEFT_EYE);

	//	NvAPI_Stereo_SetActiveEye(pStereoSetting->g_StereoHandle, NVAPI_STEREO_EYE_RIGHT);
	ClearScreen(pd3dDevice);
	renderRight(fTime);
	pStereoSetting->draw(pd3dDevice, pStereoSetting->RIGHT_EYE);

	if (bStereo) {
		ClearScreen(pd3dDevice);
		pStereoSetting->draw(pd3dDevice);
	}

	return true;
}


void RandomDotRender::updatePosition(float fTime) {
	if ((*pKeyBoard)[3])
		x -= 0.5f*fTime;
	if ((*pKeyBoard)[0])
		y += 0.5f*fTime;
	if ((*pKeyBoard)[2])
		x += 0.5f*fTime;
	if ((*pKeyBoard)[1])
		y -= 0.5f*fTime;
}


/// In this module, render Left and Render Right is absolutly same.
bool RandomDotRender::renderLeft() {

	pd3dDevice->IASetInputLayout(pInputLayout);
	pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3DXMatrixTranslation(&matWorld, x1, y1, 0);


	pMatWorldVarible->SetMatrix((float *)matWorld);
	pMatView->SetMatrix((float*)pCamera->GetViewMatrix());
	pMatProjection->SetMatrix((float*)pCamera->GetProjMatrix());

	pd3dDevice->RSGetState(&pOriginRS);
	pd3dDevice->RSSetState(pWireFrameRS);

	UINT stride = sizeof(Vertex);

	UINT offset = 0;

	pd3dDevice->IASetVertexBuffers(0, 1, &(pRandomDot->vBuffer[0]), &stride, &offset);

	D3D10_TECHNIQUE_DESC tech_desc;
	pTech->GetDesc(&tech_desc);

	for (UINT p = 0; p < tech_desc.Passes; p++) {
		pTech->GetPassByIndex(p)->Apply(0);
		pd3dDevice->Draw(pRandomDot->iVertexNum, 0);
	}


	pd3dDevice->RSSetState(pOriginRS);
	pOriginRS->Release();

	return true;
}


bool RandomDotRender::renderRight(float fTime) {

	pd3dDevice->IASetInputLayout(pInputLayout);
	pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3DXMatrixTranslation(&matWorld, x1+x, y1+y, 0);

	pMatWorldVarible->SetMatrix((float *)matWorld);
	pMatView->SetMatrix((float*)pCamera->GetViewMatrix());
	pMatProjection->SetMatrix((float*)pCamera->GetProjMatrix());

	pd3dDevice->RSGetState(&pOriginRS);
	pd3dDevice->RSSetState(pWireFrameRS);

	UINT stride = sizeof(Vertex);

	UINT offset = 0;

	pd3dDevice->IASetVertexBuffers(0, 1, &(pRandomDot->vBuffer[1]), &stride, &offset);

	D3D10_TECHNIQUE_DESC tech_desc;
	pTech->GetDesc(&tech_desc);

	for (UINT p = 0; p < tech_desc.Passes; p++) {
		pTech->GetPassByIndex(p)->Apply(0);
		pd3dDevice->Draw(pRandomDot->iVertexNum, 0);
	}

	pd3dDevice->RSSetState(pOriginRS);
	pOriginRS->Release();

	return true;
}

void RandomDotRender::collectData() {
	FILE* fout;
	fopen_s(&fout, "record.txt", "a");

	time_t lt;
	lt = time( NULL );
	static int count = 0;

	fprintf( fout, ctime(&lt));
	fprintf( fout, "Left's Position:	X:%0.2f, Y:%0.2f\n",	x1-x, y1-y );
	fprintf( fout, "Right's Position:	X:%0.2f, Y:%0.2f\n",	x2+x, y2+y );

	fprintf( fout, "-------------BlendStero------------No. %d---Complete---\n\n", count++ );

	wchar_t log[80];
	swprintf(log, L"Left X:%0.2f, Y:%0.2f\nRight X:%0.2f, Y:%0.2f\n", x1-x, y1-y, x2+x, y2+y);
	//Log::Get().Write(log);

	fclose( fout );
}



void RandomDotRender::ClearScreen(ID3D10Device *pd3dDevice)
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
	pd3dDevice->ClearRenderTargetView( pRTV, ClearColor );

	// Clear the depth stencil
	ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
	pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH, 1.0, 0 );
	return;
}
//new code
void RandomDotRender::callback_data()
{

}
