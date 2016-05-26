#ifndef __SIMPLE_FPS__
#define __SIMPLE_FPS__

/** @file
	简单射击游戏
*/

#include "Utility/PCH.h"
#include "Stereo/StereoSetting.h"
#include "Geometry/Light.h"
#include "Geometry/DiffSpecularBox.h"
#include "Helper/MeshLoader10.h"
#include "Event\KeyBoardState.h"

/**
	@class DepthTrain

*/
namespace EyeStereo {

	class CollectSphere {

	public:

		ID3D10Device*					pd3dDevice;
		ID3D10Effect*					pEffect;
		ID3D10EffectTechnique*			pTech;
		ID3D10InputLayout*				pInputLayout;


		ID3D10EffectShaderResourceVariable* g_ptxDiffuseVariable;
		ID3D10EffectVectorVariable*         g_pAmbient;
		ID3D10EffectVectorVariable*         g_pDiffuse;
		ID3D10EffectVectorVariable*         g_pSpecular;
		ID3D10EffectScalarVariable*         g_pOpacity;
		ID3D10EffectScalarVariable*         g_pSpecularPower;
		ID3D10EffectVectorVariable*         g_pLightColor;
		ID3D10EffectVectorVariable*         g_pLightPosition;
		ID3D10EffectVectorVariable*         g_pCameraPosition;
		ID3D10EffectScalarVariable*         g_pTime;
		ID3D10EffectMatrixVariable*         g_pWorld;
		ID3D10EffectMatrixVariable*         g_pWorldViewProjection;

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
		int								mPlaneIdx;

		ID3D10RasterizerState*			pSolidState;

		enum{ MAX_OBJECTS = 10 };
		std::vector<CMeshLoader10*>		g_ObjectsMesh;
		std::vector<WCHAR*>				g_ObjectsName;
		std::vector<WCHAR*>				g_ObjectsPrintName;
		std::vector<D3DXVECTOR3>		g_ObjectsPosition;
		std::vector<D3DXVECTOR3>		g_ObjectsResize;
		std::vector<D3DXVECTOR3>		g_ObjectsSpeed;
		CMeshLoader10					g_earthMesh;
		DiffSpecularBox*				g_CollectBox;

		ID3D10EffectScalarVariable*         g_pBrightness_left; ///< 左眼亮度
		ID3D10EffectScalarVariable*         g_pBrightness_right; ///< 右眼亮度
		//	ID3D10EffectScalarVariable*         g_pContrast; // HLSL
		float g_brightness_left;
		float g_brightness_right;
		//	float g_contrast;
		KeyBoard*						pKeyBoard;

		CollectSphere();

		bool init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera, KeyBoard* key);

		void reset();

		void logicUpdate(float fTime);

		void stereoRender(float fTime);

		void RenderSubset(UINT id, UINT iSubSet);


		void createNewObj();

		/**
			@brief 根据鼠标在窗口上的坐标确定选中的物体
		*/
		void shot(int sx, int sy);

		void HandMessages(UINT uMsg, LPARAM lParam);

		void ShareMessage(WCHAR* sharestr, bool *sharechanged);

		~CollectSphere();

		void ClearScreen(ID3D10Device *pd3dDevice);

	};

}

#endif //__SELECT_STEREO_H__