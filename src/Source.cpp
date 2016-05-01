#include <time.h>
#include <cstdlib>
#include "MenuApplication.h"
#include "EvolutionApplication.h"
#include "Window.h"

#include "Defines.h"
#include "Engine.h"

int main()
{
	Window::GetInstance().InitialiseWindow("Todd Hamilton - Evolution Application", SCREEN_X, SCREEN_Y);

	srand((unsigned int)time(0));
	
	Engine engine;
	BaseApplication* baseApplication = new EvolutionApplication(&engine);
	engine.Run(baseApplication);

	return 0;
}