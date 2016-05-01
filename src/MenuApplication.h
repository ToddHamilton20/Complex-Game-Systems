#pragma once

#include "BaseApplication.h"

class MenuApplication : public BaseApplication
{
public:
	MenuApplication(Engine* a_engine) : BaseApplication(a_engine) {}

	bool Startup();
	void Shutdown();

	bool Update(float deltaTime);
	void Draw();

private:
};