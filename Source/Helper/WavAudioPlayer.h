#include <xaudio2.h>
#include "SDKwavefile.h" //in DXUT

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif



class WavAudioPlayer {
public:

	HRESULT hr;
	IXAudio2*	pXAudio2 ;
	IXAudio2MasteringVoice*	pMasteringVoice;

	WavAudioPlayer(){};
	int Init(){
		UINT flags = 0;
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if ( FAILED (hr = XAudio2Create( &pXAudio2, flags) )) {
			return 0;
		}
		pMasteringVoice = NULL;
		if (FAILED (hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice))) {
			return 0;
		}
	}

	HRESULT PlayPCM(LPCWSTR szFilename );
	HRESULT FindMediaFileCch(WCHAR* strDestPath, int cchDest, LPCWSTR strFilename);

	~WavAudioPlayer() {
		pMasteringVoice->DestroyVoice();
		SAFE_RELEASE( pXAudio2 );
		
	}
};