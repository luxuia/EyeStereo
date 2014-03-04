#ifndef __STEREOSETTING__
#define __STEREOSETTING__

#include "nvapi.h"
#include "NvApiDriverSettings.h"
#pragma comment(lib, "nvapi.lib")

namespace EyeStereo {
	class StereoSetting {

	public:

		StereoHandle	g_StereoHandle;
		float			g_EyeSeparation;
		float			g_Separation;
		float			g_Convergence;

		ID3D10Texture2D*	g_pBigTexture;
		ID3D10Texture2D*	g_pBackBuffer;


		int windowWidth;
		int windowHeight;

		D3D10_BOX scrBox;

		StereoSetting():g_pBackBuffer(NULL), g_pBigTexture(NULL) {

			 g_StereoHandle = 0;
			 g_EyeSeparation = 1;
			 g_Separation = 0.01;
			 g_Convergence = 1;
		 
		}

		~StereoSetting() {
			SAFE_RELEASE(g_pBackBuffer);
			SAFE_RELEASE(g_pBigTexture);
		}

		bool init(ID3D10Device *pd3ddevice) {

			if (InitStereo()) {


				g_pBigTexture = NULL;
				makeBigTexture(pd3ddevice);
				//InitStereoParam(pd3ddevice);
				//InitStereoParam(pd3ddevice);
				return true;
			}
			return false;

		}
	

		bool InitStereo()
		{
			// 初始化
		
			/*
			NvAPI_Status status;
			status = NvAPI_Initialize();
			if (status != NVAPI_OK)
			{
				NvAPI_ShortString errorMessage;
				NvAPI_GetErrorMessage(status, errorMessage);
				MessageBoxA(NULL, errorMessage, "Unable to initialize NVAPI", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
				return false;
			}

			// 手动模式
			NvAPI_Stereo_SetDriverMode(NVAPI_STEREO_DRIVER_MODE_DIRECT);

			// 检测驱动及硬件是否支持
			NvU8 isStereoEnabled;
			status = NvAPI_Stereo_IsEnabled(&isStereoEnabled);
			if ( status != NVAPI_OK)
			{
				MessageBoxA(NULL, "Stereo is not available\nMake sure the stereo driver is installed correctly", "Stereo not available", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
				return false;
			}

			// 打开立体效果
			if(!isStereoEnabled)
			{
				MessageBoxA(NULL, "Stereo is available but not enabled\nLet's enable it", "Stereo not enabled", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
				status = NvAPI_Stereo_Enable();
			}

			NvAPI_Stereo_CreateConfigurationProfileRegistryKey(NVAPI_STEREO_DEFAULT_REGISTRY_PROFILE);
		
			*/
			windowWidth = DXUTGetWindowWidth();
			windowHeight = DXUTGetWindowHeight();
		
			scrBox.front = 0;
			scrBox.back = 1;
			scrBox.top = 0;
			scrBox.bottom = windowHeight;
			scrBox.left = 0;
			scrBox.right = windowWidth;
		
			return true;
		}

		void InitStereoParam(ID3D10Device *pd3dDevice)
		{
			NvAPI_Status status;
			status = NvAPI_Stereo_CreateHandleFromIUnknown(pd3dDevice , &g_StereoHandle);
			if (NVAPI_OK != status )
			{
				MessageBoxA(NULL, "Couldn't create the StereoHandle", "NvAPI_Stereo_CreateHandleFromIUnknown failed", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
			}

			NvAPI_Stereo_GetEyeSeparation(g_StereoHandle, &g_EyeSeparation);
			NvAPI_Stereo_GetSeparation( g_StereoHandle, &g_Separation );
			NvAPI_Stereo_GetConvergence( g_StereoHandle, &g_Convergence );
			g_Separation *= 0.01;
		}
		//*/
		enum {LEFT_EYE, RIGHT_EYE};


		void makeBigTexture(ID3D10Device *pd3dDevice) {

			if (g_pBigTexture != NULL) {
				g_pBigTexture->Release();
			}

			windowWidth = DXUTGetWindowWidth();
			windowHeight = DXUTGetWindowHeight();


			scrBox.bottom = windowHeight;
			scrBox.right = windowWidth;


			printf("Trying to Make a Big Texture %d %d\n", windowWidth, windowHeight);


			unsigned int pixelSize = 4;

			// Allocate the buffer sys mem data to write the stereo tag and stereo params
			D3D10_SUBRESOURCE_DATA sysData;
			sysData.pSysMem = new unsigned char[ pixelSize*windowWidth*2 * (windowHeight+1)] ; //*pixel
			sysData.SysMemPitch = 2*windowWidth*pixelSize; //*pixel


			// Stereo Blit defines.
			#define NVSTEREO_IMAGE_SIGNATURE 0x4433564e //NV3D

			typedef struct  _Nv_Stereo_Image_Header
			{
				unsigned int    dwSignature;
				unsigned int    dwWidth;
				unsigned int    dwHeight;
				unsigned int    dwBPP;
				unsigned int    dwFlags;
			} NVSTEREOIMAGEHEADER, *LPNVSTEREOIMAGEHEADER;

			// ORed flags in the dwFlags fiels of the _Nv_Stereo_Image_Header structure above
			#define     SIH_SWAP_EYES               0x00000001
			#define		SIH_SCALE_TO_FIT			0x00000002

			LPNVSTEREOIMAGEHEADER pStereoImageHeader = (LPNVSTEREOIMAGEHEADER)(((unsigned char *) sysData.pSysMem) + (sysData.SysMemPitch * windowHeight));
			pStereoImageHeader->dwSignature = NVSTEREO_IMAGE_SIGNATURE;
			pStereoImageHeader->dwBPP =  8*pixelSize; //*pixel

			// We'll put left image on the left and right image on the right, so no need for swap
			//pStereoImageHeader->dwFlags = 0; ///**/SIH_SWAP_EYES;
			pStereoImageHeader->dwWidth = windowWidth*2;
			pStereoImageHeader->dwHeight = windowHeight;


			D3D10_TEXTURE2D_DESC desc;
			memset( &desc, 0, sizeof(D3D10_TEXTURE2D_DESC) );
			desc.Width = windowWidth*2;
			desc.Height = windowHeight+1;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D10_USAGE_DEFAULT;
			desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			HRESULT hr = pd3dDevice->CreateTexture2D( &desc, &sysData, &g_pBigTexture );
			//V_RETURN();

			printf("Hr = %d\n", hr);

			delete sysData.pSysMem;
		}


		void draw(ID3D10Device* pd3dDevice, int flg) {

			DXUTGetDXGISwapChain()->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&g_pBackBuffer);

			unsigned int desx = 0;

			if ( LEFT_EYE == flg ) {
				desx = 0;
			} else if ( RIGHT_EYE == flg ){
				desx = windowWidth;
			}

			// print out the Big Texture
			//if ( NO_STEREO != flg )
			pd3dDevice->CopySubresourceRegion( g_pBigTexture, 0, 
				desx, 0,		//x , y	
				0,
				g_pBackBuffer, 
				0,
				&scrBox );

			g_pBackBuffer->Release();

		}

		void draw(ID3D10Device *pd3dDevice) {
			DXUTGetDXGISwapChain()->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&g_pBackBuffer);
			pd3dDevice->CopySubresourceRegion( g_pBackBuffer, 0, 
				0, 0, 
				0, 
				g_pBigTexture,
				0,
				&scrBox);
			g_pBackBuffer->Release();
		}


	};

}


#endif //__STEREOSETTING__