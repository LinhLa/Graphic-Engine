
#include "stdafx.h"
#include "Task.h"
#include "log.h"
#include <sstream>
#include "time_suffix.h"

RepetitionTask::RepetitionTask(uint64_t interval, callable funct)
{
	m_action = funct;
	m_interval = interval;
}

RepetitionTask::~RepetitionTask()
{
}



/**
 * @brief      { function_description }
 */
void RepetitionTask::start()
{
	//<get run time
	uint64_t run_time = m_timer.getMiliseconds();
	if (!m_timer.isStarted() || run_time >= m_interval)
	{
		//<take action
		m_action();

		//<reset start tick
		m_timer.start();
	}
}


OnceTimeTask::OnceTimeTask(uint64_t delay, callable funct)
{
	m_action = funct;
	m_delay = delay;
}

/**
 * @brief      Destroys the object.
 */
OnceTimeTask::~OnceTimeTask() {}


/**
 * @brief      { function_description }
 */
void OnceTimeTask::start()
{
	if (!m_timer.isStarted())
	{
		//<start tick
		m_timer.start();
	}
	//<get run time
	uint64_t run_time = m_timer.getMiliseconds();
	if (run_time >= m_delay)
	{
		//<take action
		m_action();
		m_isDone = true;
	}
}

bool OnceTimeTask::isDone() const {return m_isDone;}