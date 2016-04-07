#include "Application.hpp"


using namespace EyeStereo;

Application* Application::m_app = NULL;


Application::Application() {
	m_app = this;
}

Application::~Application() {

}

void Application::initCommon() {
	DXUTSetCallbackMsgProc( this->MsgProc );
    DXUTSetCallbackKeyboard( this->OnKeyboard );
	DXUTSetCallbackFrameMove(this->OnUpdateScene);
	DXUTSetCallbackDeviceChanging(this->ModifyDeviceSettings);

	DXUTSetCallbackD3D10DeviceCreated(this->OnD3D10CreateDevice);
	DXUTSetCallbackD3D10SwapChainResized(this->OnD3D10ResizedSwapChain);
	DXUTSetCallbackD3D10DeviceDestroyed(this->OnD3D10DestroyDevice);
	DXUTSetCallbackD3D10FrameRender(this->OnD3D10FrameRender);
	
}

void Application::initApp() {
	return;
}

void Application::mainloop() {
	DXUTMainLoop();
}

int Application::returnCode() {
	return DXUTGetExitCode();
}


HRESULT CALLBACK Application::OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext ) {
    return S_OK;
}

HRESULT CALLBACK Application::OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) {
    return S_OK;
}

void CALLBACK Application::OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) {}


void CALLBACK Application::OnD3D10DestroyDevice( void* pUserContext ){}

bool CALLBACK Application::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext ){}


void CALLBACK Application::OnUpdateScene( double fTime, float fElapsedTime, void* pUserContext ){}


LRESULT CALLBACK Application::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext ){
	return S_OK;
}

void CALLBACK Application::OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext ){
		memset(pKeyBoard, 0, sizeof(KeyBoard));
		
		if (bKeyDown) {
			
			switch (nChar) {
			case VK_UP:		{(*pKeyBoard)[0] = 1;break;}
			case VK_LEFT:	{(*pKeyBoard)[3] = 1;break;}
			case VK_RIGHT:	{(*pKeyBoard)[2] = 1;break;}
			case VK_DOWN:	{(*pKeyBoard)[1] = 1;break;}
			case 'Q': case 'q':		{(*pKeyBoard)[4] = 1;break;}
			case 'W': case 'w':		{(*pKeyBoard)[5] = 1;break;}
			case 'E': case 'e':		{(*pKeyBoard)[6] = 1;break;}				//case VK_RETURN: {g_pRandomDotRender->collectData(); break;}
			case 'r':		{(*pKeyBoard)[7] = 1;break;}
			case 't':		{(*pKeyBoard)[8] = 1;break;}
			}
		}
}


void CALLBACK Application::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext ){
}
