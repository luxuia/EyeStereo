#ifndef __Stereo_Application__
#define __Stereo_Application__

#include "Application.hpp"
#include "Stereo/StereoSetting.h"

namespace EyeStereo {
	class StereoApplication : public Application {
	public:
		StereoApplication();
		virtual ~StereoApplication();

		static Application* getApplication();

		StereoSetting* 		pStereoSetting;
		bool				bStereoActive;

	};
}




#endif // __Stereo_Application__