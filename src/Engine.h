// Author:					Todd Hamilton
// Last Change:				03/05/16
// Description of File:		Engine class

#pragma once

#include <vector>

class BaseApplication;

class Engine
{
public:
	Engine() : quitFlag(false) {}
	void Run(BaseApplication* a_state);
	void PushState(BaseApplication* a_state);
	void PopState();
	void Quit();

private:
	std::vector<BaseApplication*> stateStack;
	bool quitFlag, popFlag;
};