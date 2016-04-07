#ifndef __MAZE__
#define __MAZE__

#include "Utility/PCH.h"
#include "Stereo/StereoSetting.h"
#include "Geometry/Light.h"
#include "Helper/MeshLoader10.h"

#include "MazeObject.h"


namespace EyeStereo {

	class Maze {

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

		ID3D10RasterizerState*			pSolidState;

		MazeObject*						MazeObj;

		enum{ MAX_OBJECTS = 20 };
		

		CMeshLoader10					g_earthMesh;

		ID3D10EffectScalarVariable*     g_pBrightness_left; // ◊Û—€ ¡¡∂»
		ID3D10EffectScalarVariable*     g_pBrightness_right; // ”“—€ ¡¡∂»
		float g_brightness_left;
		float g_brightness_right;
		

		Maze();

		bool init(ID3D10Device* pd3d, StereoSetting* pstereo, CModelViewerCamera *pcamera);

		void stereoRender(float fTime);

		void RenderSubset(UINT id, UINT iSubSet);

		void pick(int sx, int sy);

		void HandMessages(UINT uMsg, LPARAM lParam);

		void ShareMessage(WCHAR* sharestr, bool *sharechanged);

		~Maze();

		void ClearScreen(ID3D10Device *pd3dDevice);

	};

}

#endif //__SELECT_STEREO_H__