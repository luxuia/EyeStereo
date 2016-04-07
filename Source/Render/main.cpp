#include "Utility/PCH.h"
#include "Utility/Log.h"
#include "Application.hpp"

using namespace EyeStereo;

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	Application* app = Application::getApplication();
	if (!app) {
		exit(0);
	}
	app->setupMsg();
	app->init();
	app->mainloop();
	return app->returnCode();
}