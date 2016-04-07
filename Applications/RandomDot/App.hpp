#include "Render/Application.hpp"
#include "Stereo/StereoSetting.h"

namespace EyeStereo {
	class App: public Application {
	public:
		App();

		void virtual init();
		void virtual mainLoop();
		
		virtual HRESULT CALLBACK OnD3D10CreateDevice(ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
			void* pUserContext);

		virtual HRESULT CALLBACK OnD3D10ResizedSwapChain(ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
			const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

		virtual void CALLBACK OnD3D10FrameRender(ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);

		virtual bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);

		virtual void CALLBACK OnUpdateScene(double fTime, float fElapsedTime, void* pUserContext);

		virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
			void* pUserContext);

		virtual void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);


		bool				bStereoActive;
		StereoSetting*		pStereoSetting;

		CModelViewerCamera          g_Camera;               // A model viewing camera
		CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
		CD3DSettingsDlg             g_SettingsDlg;          // Device settings dialog
		CDXUTTextHelper*            g_pTxtHelper = NULL;
		CDXUTDialog                 g_HUD;                  // dialog for standard controls
		CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls


		ID3D10Device*				g_pd3dDevice = NULL;

		// Direct3D 10 resources
		ID3DX10Font*                g_pFont10 = NULL;
		ID3DX10Sprite*              g_pSprite10 = NULL;


		RandomDotRender*			g_pRandomDotRender = NULL;


	protected:

		enum AppEnum {
			IDC_RANDOMDOT_SWITCH,
			IDC_RANDOMDOT_SEPERATION_SILDER,
			IDC_RANDOMDOT_DISTANCE_VALUE,
			IDC_RANDOMDOT_DISTANCE_VALUE_OK,

			IDC_STEREO_OPEN_SWITCH,
			IDC_STEREO_SEPARATION_SILDER,

			IDC_SHARE_MESSAGE,

			IDC_FINISHI_TEST,
		};
	};
}