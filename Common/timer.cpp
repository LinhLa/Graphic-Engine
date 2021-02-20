#include "stdafx.h"
#include <timer.h>
#include <ctime>

#define CLOCKS_PER_MILISECOND (CLOCKS_PER_SEC / 1000)

uint64_t GetTickCount()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}


timer::timer(){}
timer::~timer() {}

/*Our constructor initializes the internal data members.*/
void timer::start()
{
	//Start the timer
	m_Started = true;

	//Unpause the timer
	m_Paused = false;

	//Get the current clock time
	m_StartTicks = GetTickCount();
	m_PausedTicks = 0;
}

/*The start function sets the started and paused flags, gets the timer's start time and initializes the pause time to 0. For this timer,
if we want to restart it we just call start again.
Since we can start the timer if it is paused and/or running,
we should make sure to clear out the paused data.*/
void timer::stop()
{
	//Stop the timer
	m_Started = false;

	//Unpause the timer
	m_Paused = false;

	//Clear tick variables
	m_StartTicks = 0;
	m_PausedTicks = 0;
}

/*The stop function basically reinitializes all the variables.*/
void timer::pause()
{
	//If the timer is running and isn't already paused
	if (m_Started && !m_Paused)
	{
		//Pause the timer
		m_Paused = true;

		//Calculate the paused ticks
		m_PausedTicks = GetTickCount() - m_StartTicks;
		m_StartTicks = 0;
	}
}

/*When pausing, we want to check if the timer is running because it doesn't make sense to pause a timer that hasn't started.
If the timer is running, we set the pause flag, store the time when the timer was paused in m_PausedTicks, and reset the start time.
*/
void timer::unpause()
{
	//If the timer is running and paused
	if (m_Started && m_Paused)
	{
		//Unpause the timer
		m_Paused = false;

		//Reset the starting ticks
		m_StartTicks = GetTickCount() - m_PausedTicks;

		//Reset the paused ticks
		m_PausedTicks = 0;
	}
}

/* We should check if the timer is running and paused because
we can't resume a timer that's stopped or running.
We set the paused flag to false and set the new start time.

Say if you start the timer when GetTickCount() reports 5000 ms and then you pause it at 10000ms.
This means the relative time at the time of pausing is 5000ms.
If we were to resume it when GetTickCount was at 20000, the new start time would be 20000 - 5000ms or 15000ms.
This way the relative time will still be 5000ms away from the current GetTickCount time.
*/
uint64_t timer::getTicks() const
{
	//The actual timer time
	uint64_t time = 0;

	//If the timer is running
	if (m_Started)
	{
		//If the timer is paused
		if (m_Paused)
		{
			//Return the number of ticks when the timer was paused
			time = m_PausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = GetTickCount() - m_StartTicks;
		}
	}

	return time;
}

uint64_t timer::getMiliseconds() const
{
	return (getTicks()/ CLOCKS_PER_MILISECOND);
}

uint64_t timer::getSeconds() const
{
	return (getTicks()/ CLOCKS_PER_SEC);
}

/*Getting the time is a little bit tricky since our timer can be running, paused, or stopped.If the timer is stopped,
/we just return the initial 0 value.If the timer is paused, we return the time stored when paused.
If the timer is running and not paused, we return the time relative to when it started.
*/
bool timer::isStarted() const
{
	//timer is running and paused or unpaused
	return m_Started;
}

bool timer::isPaused() const
{
	//timer is running and paused
	return m_Paused && m_Started;
}