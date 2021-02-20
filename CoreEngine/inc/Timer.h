#pragma once
#include <chrono>

//The application time based timer
class Timer
{
private:
	//The clock time when the timer started
	uint32_t m_StartTicks = 0;

	//The ticks stored when the timer was paused
	uint32_t m_PausedTicks = 0;

	//The timer status
	bool m_Paused = false;
	bool m_Started = false;

public:
	//Initializes variables
	Timer();
	~Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	uint32_t getTicks() const;
	uint32_t getMiliseconds() const;
	uint32_t getSeconds() const;

	//Checks the status of the timer
	bool isStarted() const;
	bool isPaused() const;
};