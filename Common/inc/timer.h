#pragma once
#include <chrono>
uint64_t GetTickCount();

//The application time based timer
class timer
{
private:
	//The clock time when the timer started
	uint64_t m_StartTicks = 0;

	//The ticks stored when the timer was paused
	uint64_t m_PausedTicks = 0;

	//The timer status
	bool m_Paused = false;
	bool m_Started = false;

public:
	//Initializes variables
	timer();
	~timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	uint64_t getTicks() const;
	uint64_t getMiliseconds() const;
	uint64_t getSeconds() const;

	//Checks the status of the timer
	bool isStarted() const;
	bool isPaused() const;
};