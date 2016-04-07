#ifndef __Main_Application__
#define __Main_Application__

#include "Utility/PCH.h"
#include "Utility/Log.h"
#include "Utility/Timer.h"

#include "Event/BasicEventType.h"
#include "Event/KeyBoardState.h"

namespace EyeStereo {

#define Version 0.9
#define CopyRight L"UESTC_LIFE_EYE_PROJECT"

	class Application {
	public:
		Application();
		virtual ~Application();

		static Application* getApplication();

		virtual void initCommon();
		virtual void initApp();
		virtual void mainloop();
		virtual int returnCode();

		virtual HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
		                                      void* pUserContext ) ;

		virtual HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) ;

		virtual void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) ;

		virtual void CALLBACK OnD3D10DestroyDevice( void* pUserContext );

		virtual bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

		virtual void CALLBACK OnUpdateScene( double fTime, float fElapsedTime, void* pUserContext );

		virtual LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
		                          void* pUserContext );

		virtual void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);

		virtual void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);

		KeyBoard*					pKeyBoard = NULL;

		WCHAR*						pShareMessage;
		bool						bShareChanged;

	protected:
		static Application *m_app;
	};
}

#endif