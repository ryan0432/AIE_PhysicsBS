#include "_2DProject2ndYearApp.h"
#include "PhysicsEngineApp.h"
#include "BrickTest.h"
#include <Imgui.h>

int main()
{
	
	// allocation
	auto app = new PhysicsEngineApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}