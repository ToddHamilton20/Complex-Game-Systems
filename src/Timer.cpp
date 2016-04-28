#include "Timer.h"

#include "GLFW\glfw3.h"

void Cooldown::Start(float a_time)
{
	timeLeft = a_time;
}

void Cooldown::Update(float a_deltaTime)
{
	if (timeLeft > 0.0f)
		timeLeft -= a_deltaTime;
	if (timeLeft < 0.0f)
		timeLeft = 0.0f;
}

const bool Cooldown::Ready() const
{
	return timeLeft == 0.0f;
}

void Timer::Start()
{
	running = true;
}

void Timer::Stop()
{
	running = false;
}

void Timer::Reset()
{
	time = 0;
}

void Timer::Update(float a_deltaTime)
{
	if (running)
		time += a_deltaTime;
}

const float Timer::GetTime() const
{
	return time;
}