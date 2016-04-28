#include <time.h>
#include <cstdlib>
#include "EvolutionApplication.h"

int main()
{
	srand((unsigned int)time(0));
	EvolutionApplication* app = new EvolutionApplication();
	if (app->startup())
		app->run();
	app->shutdown();

	return 0;
}