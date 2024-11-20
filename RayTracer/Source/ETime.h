#pragma once
#include <chrono>

class Time
{
private:
	using clock = std::chrono::high_resolution_clock;

public:
	Time() : 
		m_startTime{ clock::now() }, 
		m_frameTime{ clock::now() } 
	{}

	void Tick();
	void Reset() { m_startTime = clock::now(); }
	float GetElapsedTime();

	float GetTime() const { return m_time; }
	float GetDeltatime() const { return m_deltatime; }

private:
	float m_time{ 0 };
	float m_deltatime{ 0 };

	clock::time_point m_startTime;
	clock::time_point m_frameTime;
};
