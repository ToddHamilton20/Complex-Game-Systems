#include "Engine.h"

#include "MenuApplication.h"
#include "Window.h"

void Engine::Run(BaseApplication* a_state)
{
	PushState(a_state);
	double prevTime = glfwGetTime();
	double currTime = glfwGetTime();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (stateStack.size() > 0)
	{
		currTime = glfwGetTime();
		BaseApplication* state = stateStack[stateStack.size() - 1];
		state->Update(currTime - prevTime);
		glfwPollEvents();

		if (quitFlag || stateStack.size() <= 0)
			break;
		if (popFlag)
		{
			popFlag = false;
			continue;
		}

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// NOT CLEARING DEPTH BUFFER, DEPTH BUFFER IS OFF.
		glClear(GL_COLOR_BUFFER_BIT);

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
	popFlag = true;
}

void Engine::Quit()
{
	quitFlag = true;
}