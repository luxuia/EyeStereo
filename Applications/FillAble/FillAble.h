#ifndef __FillAble__
#define __FillAble__

#include "Utility/PCH.h"

#include "Stereo/StereoSetting.h"
#include "Event/KeyBoardState.h"

#include "Helper/MeshLoader10.h"
#include "Helper/MeshHelper.h"
#include "FillObject.h"


namespace EyeStereo {


	class FillAble {

	public:

		ID3D10Device*					pd3dDevice;
		ID3D10Effect*					pEffect;
		ID3D10EffectTechnique*			pTech;
		ID3D10InputLayout*				pInputLayout;

		ID3D10EffectShaderResourceVariable* pShaderRV;

		IDXGISwapChain*					pSwapChain;

		CModelViewerCamera*				pCamera;

		long int						windowsWidth, windowsHeight;
		StereoSetting*					pStereoSetting;
		bool							bactive;

		D3DXVECTOR3						eyePos;

		bool							bStereo;
		float							initDepth, initRadius;
		WCHAR*							mShareMessage;
		bool*							mShareChanged;

		ID3D10RasterizerState*			pSolidState;

		float							g_fLastMoveTime;
		float							g_fObjectMoveSpace;
		/*	CMeshLoader10					g_earthMesh;*/
		ModelObjectCreator*				g_pFillAbleCreator;
		MeshHelper*						g_pMeshHelper;
		KeyBoard*						g_pKeyBoard;

		CMeshLoader10				*g_BackGroundMesh;
		D3DXVECTOR3					g_BackGroundPosition;
		UINT						g_ActiveCube;

		typedef std::pair<UINT, UINT>  uupair;

		std::vector< uupair  >	g_Cubesflg;
		//enum {CONST_Z_ = 5};

		FillAble() :initRadius(2), initDepth(4), pd3dDevice(NULL) {}

		bool init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera, KeyBoard* pkeyboard);

		void Restart();

		UINT FindConnectedCubes(std::vector< uupair > &stack, ModelObject::Status dir);


		bool Collision(ModelObject::Status dir);
		void DestoryCubes(std::vector< std::pair<UINT, UINT> > &stack);
		void DeleteIfFullLine();

		void UpdateObjectPosition(float fTime);

		void stereoRender(float fTime);

		void Rotate90();

		void HandMessages(UINT uMsg, LPARAM lParam) {

			switch (uMsg) {
			case WM_LBUTTONDOWN:
				pick(LOWORD(lParam), HIWORD(lParam));
				return;
			};
		}

		void ShareMessage(WCHAR* sharestr, bool *sharechanged) {
			mShareChanged = sharechanged;
			mShareMessage = sharestr;
		}


		~FillAble() {
			SAFE_RELEASE(pd3dDevice);
			SAFE_RELEASE(pEffect);
			//pTech;
			SAFE_RELEASE(pInputLayout);
			SAFE_RELEASE(pSwapChain);
			SAFE_RELEASE(pSolidState);
			SAFE_DELETE(g_pMeshHelper);
			SAFE_DELETE(g_pFillAbleCreator);
			SAFE_DELETE(g_BackGroundMesh);
			//SAFE_RELEASE(pShaderRV);
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

	//protected:
	};
}


#endif //__FillAble_H__