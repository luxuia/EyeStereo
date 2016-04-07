#ifndef __SAMETIME_CURSOR__
#define __SAMETIME_CURSOR__


#include "Utility/PCH.h"
#include "Stereo/StereoSetting.h"

using namespace EyeStereo;

	class SimPerception {
	public:


		enum STflg { LEFT = 0, RIGHT = 1, };

		ID3D10Device*					pd3dDevice;
		ID3D10Effect*					pEffect;
		ID3D10EffectTechnique*			pTech;
		ID3D10InputLayout*				pInputLayout;

		ID3D10BlendState*				pBlendStateNoBlend;


		ID3D10Resource*					pCursors[2];

		ID3D10DepthStencilState*		pDepthStencilState;

		ID3D10ShaderResourceView *		pshaderRV[2];

		IDXGISwapChain*					pSwapChain;


		ID3DX10Sprite*					pSprite;
		D3DX10_SPRITE					sprite10[2];
		D3D10_BOX						textureBox;

		long int						windowsWidth, windowsHeight;
		StereoSetting*					pStereoSetting;
		bool				bactive;
		bool				bStereo;
		POINT				pp;


		SimPerception() {
			pd3dDevice = NULL;
		}

		bool init(ID3D10Device* pd3d, StereoSetting* pSS) {
			pd3dDevice = pd3d;
			pd3d->AddRef();

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
			pTech->GetPassByIndex(0)->GetDesc(&PassDesc);
			pd3d->CreateInputLayout(layout, numElements,
				PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout);


			pSwapChain = DXUTGetDXGISwapChain();
			pSwapChain->AddRef();

			loadTexture(LEFT, L"../Data/Texture/left_bong.png");
			loadTexture(RIGHT, L"../Data/Texture/right_bong.png");

			windowsHeight = DXUTGetWindowHeight();
			windowsWidth = DXUTGetWindowWidth();

			textureBox.back = 1;
			textureBox.bottom = 80;
			textureBox.front = 0;
			textureBox.left = 0;
			textureBox.right = 80;
			textureBox.top = 0;

			pStereoSetting = pSS;

			D3DX10CreateSprite(pd3dDevice, 500, &pSprite);

			D3DXMATRIX matProjection;

			D3DXMatrixOrthoOffCenterLH(&matProjection,
				0., (float)windowsWidth,
				0., (float)windowsHeight,
				0.1, 10.);
			pSprite->SetProjectionTransform(&matProjection);


			bactive = false;
			bStereo = false;

			D3D10_BLEND_DESC BlendState;

			ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));

			BlendState.BlendEnable[0] = true;

			BlendState.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

			BlendState.SrcBlend = D3D10_BLEND_SRC_ALPHA;

			BlendState.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;

			BlendState.BlendOp = D3D10_BLEND_OP_ADD;

			BlendState.SrcBlendAlpha = D3D10_BLEND_ZERO;

			BlendState.DestBlendAlpha = D3D10_BLEND_ZERO;

			BlendState.BlendOpAlpha = D3D10_BLEND_OP_ADD;

			BlendState.AlphaToCoverageEnable = true;

			pd3dDevice->CreateBlendState(&BlendState, &pBlendStateNoBlend);


			return true;

		}


		bool loadTexture(STflg eyeFlg, WCHAR *pSrcFile) {

			D3DX10CreateShaderResourceViewFromFile(pd3dDevice, pSrcFile, NULL, NULL, &(pshaderRV[eyeFlg]), NULL);

			sprite10[eyeFlg].pTexture = pshaderRV[eyeFlg];

			sprite10[eyeFlg].TexCoord.x = 0;
			sprite10[eyeFlg].TexCoord.y = 0;

			sprite10[eyeFlg].TexSize.x = 1.0f;
			sprite10[eyeFlg].TexSize.y = 1.0f;

			sprite10[eyeFlg].TextureIndex = 0;

			sprite10[eyeFlg].ColorModulate = D3DXCOLOR(1, 1, 1, 1);

			return true;
		}


		bool stereoRender() {

			D3DXMATRIX matTranslation, matScaling;

			D3DXMatrixTranslation(&matTranslation,
				(float)windowsWidth / 2,
				(float)windowsHeight / 2,
				1.1f);

			D3DXMatrixScaling(&matScaling, 80, 100, 1);
			D3DXMATRIX test = matScaling*matTranslation;

			sprite10[LEFT].matWorld = test;
		

			//NvAPI_Stereo_SetActiveEye(pStereoSetting->g_StereoHandle, NVAPI_STEREO_EYE_LEFT);
			ClearScreen(pd3dDevice);
			renderLeft();
			pStereoSetting->draw(pd3dDevice, pStereoSetting->LEFT_EYE);

			//POINT pp;
			GetCursorPos(&pp);
			ScreenToClient(DXUTGetHWNDDeviceWindowed(), &pp);

			D3DXMatrixTranslation(&matTranslation,
				(float)pp.x,
				(float)windowsHeight - pp.y,
				1.1);

			printf("x:%f  y:%f\n", (float)pp.x, (float)pp.y);
			D3DXMatrixScaling(&matScaling, 80, 100, 1);
			test = matScaling*matTranslation;

			sprite10[RIGHT].matWorld = test;


			ClearScreen(pd3dDevice);
			renderRight();
			pStereoSetting->draw(pd3dDevice, pStereoSetting->RIGHT_EYE);

			if (bStereo) {
				ClearScreen(pd3dDevice);
				pStereoSetting->draw(pd3dDevice);
			}
			//pBackBuffer->Release();

			return true;
		}



		void ClearScreen(ID3D10Device *pd3dDevice)
		{
			float ClearColor[4] = { 0.176f, 0.196f, 0.667f, 0.0f };
			ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
			pd3dDevice->ClearRenderTargetView(pRTV, ClearColor);

			// Clear the depth stencil
			ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
			pd3dDevice->ClearDepthStencilView(pDSV, D3D10_CLEAR_DEPTH, 1.0, 0);
			return;
		}

		bool renderLeft() {

			pSprite->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);

			float blendFactor[] = { 0, 0, 0, 0 };

			pd3dDevice->OMSetBlendState(pBlendStateNoBlend, blendFactor, 0xffffffff);

			pSprite->DrawSpritesImmediate(&(sprite10[LEFT]), 1, 0, 0);

			pSprite->End();

			return true;
		}


		bool renderRight() {

			pSprite->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);

			float blendFactor[] = { 0, 0, 0, 0 };

			pd3dDevice->OMSetBlendState(pBlendStateNoBlend, blendFactor, 0xffffffff);
			pSprite->DrawSpritesImmediate(&(sprite10[RIGHT]), 1, 0, 0);

			pSprite->End();

			return true;
		}


		void HandMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

			if (uMsg == WM_LBUTTONUP) {
				dataRecord();
			}
		}


		void dataRecord() {
			FILE *fout;
			fopen_s(&fout, "record.txt", "a");

			time_t lt;
			lt = time(NULL);
			static int count = 0;

			fprintf(fout, ctime(&lt));
			fprintf(fout, "Left's Position:	X:%8d, Y:%8d\n", windowsWidth / 2, windowsHeight / 2);
			fprintf(fout, "Right's Position:	X:%8d, Y:%8d\n", pp.x - 20, windowsHeight - pp.y + 30);

			fprintf(fout, "----------------SameTimeCursor---No. %d---Complete---\n\n", count++);

			fclose(fout);
		}

		~SimPerception() {

			SAFE_RELEASE(pd3dDevice);
			SAFE_RELEASE(pEffect);
			//pTech;
			SAFE_RELEASE(pInputLayout);
			SAFE_RELEASE(pSwapChain);

			SAFE_RELEASE(pSprite);
			SAFE_RELEASE(pshaderRV[0]);
			SAFE_RELEASE(pshaderRV[1]);

			SAFE_RELEASE(pBlendStateNoBlend);

			ShowCursor(true);

			//SAFE_RELEASE()

		}





	};

#endif //__SAMETIME_CURSOR__