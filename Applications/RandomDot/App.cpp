//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------


#include "Utility/PCH.h"

#include "Utility/Log.h"
#include "Utility/Timer.h"

//MY_CLASS
#include "Stereo/StereoSetting.h"

#include "Event/KeyBoardState.h"

#include "RandomDotRender.hpp"

//#define DEBUG_VS   // Uncomment this line to debug D3D9 vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug D3D9 pixel shaders 
#define filename "test.txt"

using namespace EyeStereo;

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
CModelViewerCamera          g_Camera;               // A model viewing camera
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_SettingsDlg;          // Device settings dialog
CDXUTTextHelper*            g_pTxtHelper = NULL;
CDXUTDialog                 g_HUD;                  // dialog for standard controls
CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls

// Direct3D 9 resources
//extern ID3DXFont*           g_pFont9;
//extern ID3DXSprite*         g_pSprite9;

ID3D10Device*				g_pd3dDevice = NULL;

// Direct3D 10 resources
ID3DX10Font*                g_pFont10 = NULL;
ID3DX10Sprite*              g_pSprite10 = NULL;
ID3D10Effect*               g_pEffect10 = NULL;
ID3D10InputLayout*          g_pVertexLayout = NULL;
ID3D10EffectMatrixVariable* g_pmWorldViewProj = NULL;
ID3D10EffectMatrixVariable* g_pmWorld = NULL;
ID3D10EffectScalarVariable* g_pfTime = NULL;




//MY_ADDED
StereoSetting*				g_pStereoSetting = NULL;
RandomDotRender*			g_pRandomDotRender = NULL;
KeyBoard*					g_pKeyBoard = NULL;


WCHAR						g_pShareMessage[64] = L"UESTC_LIFE";
bool						g_bShareChanged = false;
clock_t                     start = 0,finish;
//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------

enum {

 IDC_TOGGLEFULLSCREEN,
 IDC_TOGGLEREF,
 IDC_CHANGEDEVICE,
 IDC_TOGGLEWARP,

 IDC_RANDOMDOT_SWITCH,
 IDC_RANDOMDOT_SEPERATION_SILDER,
 IDC_RANDOMDOT_DISTANCE_VALUE,
 IDC_RANDOMDOT_DISTANCE_VALUE_OK,

 IDC_STEREO_OPEN_SWITCH,
 IDC_STEREO_SEPARATION_SILDER,

 IDC_SHARE_MESSAGE,

 IDC_FINISHI_TEST
};


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );


bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext );
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext );
void CALLBACK OnD3D10DestroyDevice( void* pUserContext );

void InitApp();
void RenderText();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
//int main()
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	#define  new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
	
#endif
	  //_CrtSetBreakAlloc(698);
    // DXUT will create and use the best device (either D3D9 or D3D10) 
    // that is available on the system depending on which D3D callbacks are set below


    // Set DXUT callbacks
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );


    DXUTSetCallbackD3D10DeviceAcceptable( IsD3D10DeviceAcceptable );
    DXUTSetCallbackD3D10DeviceCreated( OnD3D10CreateDevice );
    DXUTSetCallbackD3D10SwapChainResized( OnD3D10ResizedSwapChain );
    DXUTSetCallbackD3D10SwapChainReleasing( OnD3D10ReleasingSwapChain );
    DXUTSetCallbackD3D10DeviceDestroyed( OnD3D10DestroyDevice );
    DXUTSetCallbackD3D10FrameRender( OnD3D10FrameRender );

    InitApp();
    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"Random Dot Experience" );
    DXUTCreateDevice( true, 640, 480 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );


    g_HUD.SetCallback( OnGUIEvent ); int iY = 10;
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN,	L"Toggle full screen",	35, iY,		  125, 22 );
    g_HUD.AddButton( IDC_CHANGEDEVICE,		L"Change device (F2)",	35, iY += 24, 125, 22, VK_F2 );
    g_HUD.AddButton( IDC_TOGGLEREF,			L"Toggle REF (F3)",		35, iY += 24, 125, 22, VK_F3 );
    g_HUD.AddButton( IDC_TOGGLEWARP,		L"Toggle WARP (F4)",	35, iY += 24, 125, 22, VK_F4 );
	
	g_HUD.AddButton( IDC_STEREO_OPEN_SWITCH,	L"��Stereo",	35,	iY+=24, 125, 22);
	g_HUD.AddButton( IDC_RANDOMDOT_SWITCH,	L"���ںϼ��",	35, iY+=24, 125, 22);
	g_HUD.AddButton( IDC_FINISHI_TEST,	L"�Ѿ�������ͼ��",	35, iY+=24, 125, 22);

	iY = 0;
	g_SampleUI.AddSlider(	IDC_RANDOMDOT_SEPERATION_SILDER,				35, iY,		125, 22, -100, 100, 20);
	g_SampleUI.AddEditBox(	IDC_RANDOMDOT_DISTANCE_VALUE,		L"0.02",		35, iY+=30, 125, 30);
	g_SampleUI.AddButton(	IDC_RANDOMDOT_DISTANCE_VALUE_OK,	L"Accept",	35, iY+=32, 125, 25);


	g_SampleUI.AddSlider( IDC_STEREO_SEPARATION_SILDER,		35, iY+=32, 125, 22, 0, 100, 100);

	g_SampleUI.AddStatic( IDC_SHARE_MESSAGE,	g_pShareMessage, 35, iY+=32, 125, 40);

	g_SampleUI.GetSlider( IDC_RANDOMDOT_SEPERATION_SILDER)->SetVisible(false);
	g_SampleUI.GetEditBox( IDC_RANDOMDOT_DISTANCE_VALUE )->SetVisible(false);
	g_SampleUI.GetButton( IDC_RANDOMDOT_DISTANCE_VALUE_OK )->SetVisible( false); 

    g_SampleUI.SetCallback( OnGUIEvent ); iY = 10;

	g_pStereoSetting = new StereoSetting();

	g_pRandomDotRender = new RandomDotRender();

	g_pKeyBoard = new KeyBoard();

}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    g_pTxtHelper->Begin();
    g_pTxtHelper->SetInsertionPos( 5, 5 );
    g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );
    g_pTxtHelper->End();
}


//--------------------------------------------------------------------------------------
// Reject any D3D10 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext )
{
    HRESULT hr;

    V_RETURN( D3DX10CreateSprite( pd3dDevice, 500, &g_pSprite10 ) );
    V_RETURN( g_DialogResourceManager.OnD3D10CreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnD3D10CreateDevice( pd3dDevice ) );
    V_RETURN( D3DX10CreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                                OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                L"Arial", &g_pFont10 ) );
    g_pTxtHelper = new CDXUTTextHelper( NULL, NULL, g_pFont10, g_pSprite10, 15 );

	    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye( 0.0f, 0.0f, -5.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );


	g_pStereoSetting->init(pd3dDevice);
	//g_pStereoSetting->makeBigTexture(pd3dDevice);


	g_pRandomDotRender->init(pd3dDevice, g_pStereoSetting, &g_Camera, 0.02, g_pKeyBoard);
	g_pRandomDotRender->bactive = false;

	g_pd3dDevice = pd3dDevice;

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    V_RETURN( g_SettingsDlg.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    g_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300 );
    g_SampleUI.SetSize( 170, 300 );

	g_pStereoSetting->makeBigTexture(pd3dDevice);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D10 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    
	static int ppppp = 0;
	if (ppppp  < 3) {ppppp++;return;};


	D3DXMATRIX mWorldViewProjection;
    D3DXMATRIX mWorld;
    D3DXMATRIX mView;
    D3DXMATRIX mProj;

	//printf("%0.2f, %0.2f\n", fTime, fElapsedTime);

    float ClearColor[4] = { 0.f, 0.f, 0.f, 0.f };
    ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
    pd3dDevice->ClearRenderTargetView( pRTV, ClearColor );

    // Clear the depth stencil
    ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
    pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH, 1.0, 0 );

    // If the settings dialog is being shown, then render it instead of rendering the app's scene
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }

	bool flgStereo = false;
	
	if ( g_pRandomDotRender->bactive == true ) {
		g_pRandomDotRender->stereoRender(fElapsedTime);
		flgStereo = true;
	}

	if (g_bShareChanged == true) {
		g_SampleUI.GetStatic( IDC_SHARE_MESSAGE )->SetText(g_pShareMessage);
	}

    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );
    RenderText();
    g_HUD.OnRender( fElapsedTime );
    g_SampleUI.OnRender( fElapsedTime );
    DXUT_EndPerfEvent();
}


//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext )
{
    g_DialogResourceManager.OnD3D10ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10DestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D10DestroyDevice();
    g_SettingsDlg.OnD3D10DestroyDevice();
	
    SAFE_RELEASE( g_pFont10 );
    SAFE_RELEASE( g_pEffect10 );
    SAFE_RELEASE( g_pVertexLayout );
    SAFE_RELEASE( g_pSprite10 );
    SAFE_DELETE( g_pTxtHelper );
	
	delete	g_pRandomDotRender;

	delete	g_pStereoSetting;
	//_CrtDumpMemoryLeaks();
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    if( pDeviceSettings->ver == DXUT_D3D9_DEVICE )
    {
        IDirect3D9* pD3D = DXUTGetD3D9Object();
        D3DCAPS9 Caps;
        pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &Caps );

        // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
        // then switch to SWVP.
        if( ( Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
            Caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
        {
            pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
	}
        // Debugging vertex shaders requires either REF or software vertex processing 
        // and debugging pixel shaders requires REF.  

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
            ( DXUT_D3D10_DEVICE == pDeviceSettings->ver &&
              pDeviceSettings->d3d10.DriverType == D3D10_DRIVER_TYPE_REFERENCE ) )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    // Update the camera's position based on user input 
    g_Camera.FrameMove( fElapsedTime );
}




//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{



    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass messages to settings dialog if its active
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;


    // Pass all remaining windows messages to camera so it can respond to user input
    
	g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );
	
	
    return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
		memset(g_pKeyBoard, 0, sizeof(KeyBoard));
		
		if (bKeyDown) {
			
			switch (nChar) {
			case VK_UP:		{(*g_pKeyBoard)[0] = 1;break;}
			case VK_LEFT:	{(*g_pKeyBoard)[3] = 1;break;}
			case VK_RIGHT:	{(*g_pKeyBoard)[2] = 1;break;}
			case VK_DOWN:	{(*g_pKeyBoard)[1] = 1;break;}
			case 'Q': case 'q':		{(*g_pKeyBoard)[4] = 1;break;}
			case 'W': case 'w':		{(*g_pKeyBoard)[5] = 1;break;}
			case 'E': case 'e':		{(*g_pKeyBoard)[6] = 1;break;}				//case VK_RETURN: {g_pRandomDotRender->collectData(); break;}
			case 'r':		{(*g_pKeyBoard)[7] = 1;break;}
			case 't':		{(*g_pKeyBoard)[8] = 1;break;}
			}
		}

}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN: {
			DXUTToggleFullScreen();
			//g_pStereoSetting->makeBigTexture(g_pd3dDevice);
			break;
								   }
        case IDC_TOGGLEREF:
            DXUTToggleREF(); break;
        case IDC_TOGGLEWARP:
            DXUTToggleWARP(); break;
        case IDC_CHANGEDEVICE:
            g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
			
		case IDC_STEREO_OPEN_SWITCH: {

			g_pRandomDotRender->bStereo = !g_pRandomDotRender->bStereo;
			if (g_pRandomDotRender->bStereo) {
				g_HUD.GetButton( IDC_STEREO_OPEN_SWITCH )->SetText(L"����Stereo");
			} else {
				g_HUD.GetButton( IDC_STEREO_OPEN_SWITCH )->SetText(L"��ʼStereo");
			}

			break;
							  } 
		case IDC_RANDOMDOT_SWITCH: {

			bool flg = g_pRandomDotRender->bactive;
			flg = !flg;
			g_pRandomDotRender->bactive = flg;	  

			g_SampleUI.GetEditBox( IDC_RANDOMDOT_DISTANCE_VALUE )->SetVisible( flg );
			g_SampleUI.GetSlider( IDC_RANDOMDOT_SEPERATION_SILDER )->SetVisible( flg );
			g_SampleUI.GetButton( IDC_RANDOMDOT_DISTANCE_VALUE_OK )->SetVisible( flg ); 


			if (g_pRandomDotRender->bactive) {
				g_HUD.GetButton( IDC_RANDOMDOT_SWITCH)->SetText(L"�������ں�");
			} else {
				g_HUD.GetButton( IDC_RANDOMDOT_SWITCH)->SetText(L"��ʼ���ں�");
			}
			break;
		}

		case IDC_RANDOMDOT_SEPERATION_SILDER: {

			float dotDist = g_SampleUI.GetSlider( IDC_RANDOMDOT_SEPERATION_SILDER )->GetValue()/100.0f;
			WCHAR num[10];

			swprintf(num, L"%0.2f", dotDist);

			g_SampleUI.GetEditBox( IDC_RANDOMDOT_DISTANCE_VALUE )->SetText( num );

			break;
		}

		case IDC_RANDOMDOT_DISTANCE_VALUE_OK: {

			bool bstereo = g_pRandomDotRender->bStereo;
			float x = g_pRandomDotRender->x, y = g_pRandomDotRender->y;


			g_pRandomDotRender->~RandomDotRender();

			float dist = _wtof(g_SampleUI.GetEditBox( IDC_RANDOMDOT_DISTANCE_VALUE )->GetText());
			g_pRandomDotRender->init(g_pd3dDevice, g_pStereoSetting, &g_Camera, dist, g_pKeyBoard);
			g_pRandomDotRender->bactive = true;
			g_pRandomDotRender->bStereo = bstereo;

			g_pRandomDotRender->x = x;
			g_pRandomDotRender->y = y;

			start = clock();

			break;
		}

		case IDC_STEREO_SEPARATION_SILDER: {
			float separation = g_SampleUI.GetSlider( IDC_STEREO_SEPARATION_SILDER )->GetValue()/1000.0f;
			g_pStereoSetting->g_Separation = separation;
			WCHAR num[10];
			swprintf(num, L"%0.2f", separation);

			g_SampleUI.GetEditBox( IDC_RANDOMDOT_DISTANCE_VALUE )->SetText( num );
			break;

		}

		case IDC_FINISHI_TEST: {
			g_pRandomDotRender->bIsfinish = !g_pRandomDotRender->bIsfinish;
			if (g_pRandomDotRender->bIsfinish) {
//				g_HUD.GetButton( IDC_FINISHI_TEST )->SetText(L"�ٴε����ȷ��");
				if(!start) {
					std::ofstream output_str(filename);
					output_str<<"0"<<std::endl;
				}
				else {
					finish = clock();
					clock_t delta = finish - start;
					double tim = (double) delta / CLOCKS_PER_SEC;
//					printf("%f\n",tim);
					std::ofstream output_str(filename);
					output_str<<std::fixed<<std::setprecision(2)<<tim<<std::endl;
				}
			} else {
				g_HUD.GetButton( IDC_FINISHI_TEST )->SetText(L"�����˳�����");
			}
			break;
		}

    }
}

