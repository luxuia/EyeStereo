#ifndef __PINGPANG_H__
#define __PINGPANG_H__



#include "Utility\PCH.h"
#include "Geometry\Sphere.hpp"
#include "Geometry\Ball.h"
#include "Geometry\LightBox.h"
#include "Geometry\Light.h"

#include "Stereo\StereoSetting.h"
#include "Event\KeyBoardState.h"

#include "Geometry\Vertex.h"

namespace EyeStereo {
	class PingPang {

		class LifeTimeBox : public LightBox{
		public:
			enum LIVE_STATE{ DIE, LIVE, EVER };
			LIVE_STATE liveState;
			LifeTimeBox(float ll, float hh, float ww, D3DXCOLOR diff, D3DXCOLOR spec = YELLOW) {
				liveState = LIVE;
				LightBox::LightBox(ll, hh, ww, diff, spec);
			}
		};
	public:
		ID3D10Device*	pd3dDevice;
		ID3D10Effect*	pEffect;
		ID3D10EffectTechnique*	pTech;
		ID3D10InputLayout*		pInputLayout;


		IDXGISwapChain*			pSwapChain;
		CModelViewerCamera*		pCamera;

		int blockNumL, blockNumH;


		ID3D10EffectMatrixVariable*		mfxWVP;
		ID3D10EffectMatrixVariable*		mfxWorld;
		ID3D10EffectVariable*			mfxEyePosVal;
		ID3D10EffectVariable*			mfxLightVal[3];
		ID3D10EffectScalarVariable*		mfxLightType;

		ID3D10EffectShaderResourceVariable* pShaderRV;

		D3DXMATRIX						matWorld;
		D3DXMATRIX						matBlockWorld[20];
		D3DXMATRIX						matWorldViewProjection;


		Ball*							pBall;
		std::vector<LifeTimeBox*>			pBoxVector;

		long int						windowsWidth, windowsHeight;
		StereoSetting*					pStereoSetting;
		bool							bactive;
		bool							bstereo;

		Light							lights[3];
		int								lightType;
		D3DXVECTOR3						eyePos;
		// 
		// 	ID3D10RasterizerState*			pSolidState;

		KeyBoard*						pKeyBoard;

		PingPang() :pShaderRV(NULL), pd3dDevice(NULL), pEffect(NULL), pTech(NULL), pInputLayout(NULL), pSwapChain(NULL), pBall(NULL), blockNumH(3), blockNumL(4) {
		}

		~PingPang() {
			SAFE_RELEASE(pEffect);
			SAFE_RELEASE(pInputLayout);
			SAFE_RELEASE(pSwapChain);
			/*		SAFE_DELETE(pSolidState);*/
			if (pBall != NULL)
				delete pBall;

			for (UINT i = 0; i < pBoxVector.size(); ++i) {
				pBoxVector[i]->destory();
			}
			pBoxVector.clear();
		}


		bool init(ID3D10Device* pdevice, StereoSetting* stereo, CModelViewerCamera* pcamera, KeyBoard* key) ; 


		void CreateBounds();

		void initLight();

		void Restart();

		D3DXVECTOR3 RefectBall(D3DXVECTOR3 v, D3DXVECTOR3 normal);


		void CaculateBallV(float fTime);

		void stereoRender(float fTime);

		void renderScene(int index);

		void ClearScreen(ID3D10Device *pd3dDevice);

	};

}
#endif //__PINGPANG_H__