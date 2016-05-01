#include <time.h>
#include <cstdlib>
#include "EvolutionApplication.h"
#include "Window.h"


int main()
{
	Window::GetInstance().InitialiseWindow("Todd Hamilton - Evolution Application", SCREEN_X, SCREEN_Y);

	srand((unsigned int)time(0));
	EvolutionApplication* app = new EvolutionApplication();
	if (app->Startup())
		app->Run();
	app->Shutdown();

	return 0;
}