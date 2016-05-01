#include "Engine.h"

#include "MenuApplication.h"
#include "Window.h"

void Engine::Run(BaseApplication* a_state)
{
	PushState(a_state);
	double prevTime = glfwGetTime();
	double currTime = glfwGetTime();

	while (stateStack.size() > 0)
	{
		currTime = glfwGetTime();
		BaseApplication* state = stateStack[stateStack.size() - 1];
		state->Update(currTime - prevTime);
		glfwPollEvents();

		if (quitFlag || stateStack.size() <= 0)
			break;

		state->Draw();
		Window::GetInstance().SwapBuffers();
		prevTime = currTime;
	}

	while (stateStack.size() > 0)
		PopState();
}

void Engine::PushState(BaseApplication* a_state)
{
	stateStack.push_back(a_state);
	a_state->Startup();
}

void Engine::PopState()
{
	BaseApplication* state = stateStack[stateStack.size() - 1];
	stateStack.pop_back();
	state->Shutdown();
	delete state;
}

void Engine::Quit()
{
	quitFlag = true;
}