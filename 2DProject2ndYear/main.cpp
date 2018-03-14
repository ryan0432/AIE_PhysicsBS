#include "_2DProject2ndYearApp.h"
#include "PhysicsEngineApp.h"
#include "BrickTest.h"
#include <vld.h>	// visual leak detection 
#include <crtdbg.h> // traditional method to check memory leak
#include <Imgui.h>

int main()
{
	// leak detection
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// allocation
	auto app = new PhysicsEngineApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}