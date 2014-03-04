#ifndef __BLENDSTEREO__
#define __BLENDSTEREO__


#include "Utility/PCH.h"
#include "Utility/Log.h"
#include "Event/KeyBoardState.h"
#include "Stereo/StereoSetting.h"


#include "RandDoter.hpp"

namespace EyeStereo {


	class RandomDotRender {
	public:

		enum STflg {LEFT = 0, RIGHT = 1, };

		ID3D10Device*					pd3dDevice;
		ID3D10Effect*					pEffect;
		ID3D10EffectTechnique*			pTech;
		ID3D10InputLayout*				pInputLayout;
	
		IDXGISwapChain*					pSwapChain;

		long int						windowsWidth, windowsHeight;
		StereoSetting*					pStereoSetting;
		bool							bactive;
		bool							bStereo;
		bool                            bIsfinish;

		RandomDot*						pRandomDot;

		ID3D10EffectMatrixVariable*		pMatWorldVarible;
		ID3D10EffectMatrixVariable*		pMatView;
		ID3D10EffectMatrixVariable*		pMatProjection;
		D3DXMATRIX						matWorld;

		ID3D10RasterizerState*			pWireFrameRS;
		ID3D10RasterizerState*			pOriginRS;
		CModelViewerCamera*				pCamera;	
		float x, y;

		KeyBoard*						pKeyBoard;

		static const int x1 = -2, y1 = -2, x2 = 1, y2 = -2;

		RandomDotRender();

		~RandomDotRender();

		Geometry *getGeometry(Geometry *newGeo);

		void makeDot(float dotDist);

		bool init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera, float dotDist, KeyBoard *key);

		bool stereoRender(float fTime);

		void updatePosition(float fTime);

		bool renderLeft();

		bool renderRight(float fTime);

		void collectData();

		void ClearScreen(ID3D10Device *pd3dDevice);
		//new code
		void callback_data();
	};


}

#endif //__BLENDSTEREO__