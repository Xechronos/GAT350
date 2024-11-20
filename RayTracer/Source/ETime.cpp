#include "ETime.h"

void Time::Tick()
{
	clock::duration duration = clock::now() - m_startTime;
	m_time = duration.count() / (float)clock::duration::period::den;

	duration = clock::now() - m_frameTime;
	m_deltatime = duration.count() / (float)clock::duration::period::den;

	m_frameTime = clock::now();
}

float Time::GetElapsedTime()
{
	clock::duration duration = clock::now() - m_startTime;

	return duration.count() / (float)clock::duration::period::den;
}
