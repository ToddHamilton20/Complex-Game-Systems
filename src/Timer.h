#pragma once

class TickCounter
{
public:
	TickCounter() : tickTime(0), ticksRemaining(0), timeRemaining(0) {}
	void Start(float a_tickTime, int a_ticks, bool a_immediateTick = true);
	int GetTicksPassed(float a_deltaTime);

private:
	float ticksRemaining;
	float tickTime;
	float timeRemaining;
};

class Cooldown
{
public:
	Cooldown() : timeLeft(0) {}
	void Start(float a_time);
	void Update(float a_deltaTime);
	const bool Ready() const;

private:
	float timeLeft;
};

class Timer
{
public:
	Timer() : time(0), running(false) {}
	void Start();
	void Stop();
	void Reset();
	void Update(float a_deltaTime);
	const float GetTime() const;

private:
	float time;
	bool running;
};