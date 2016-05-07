// Author:					Todd Hamilton
// Last Change:				01/05/16
// Description of File:		Timer and Cooldown classes

#pragma once

// Counts down from specified time in seconds.
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

// Counts up from zero in seconds.
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