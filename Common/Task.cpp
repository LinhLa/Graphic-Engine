#include "stdafx.h"
#include "Task.h"
#include "memmgr.h"
#include <SDL.h>
#include <sstream>

TaskTimer::TaskTimer()
{
}

TaskTimer::~TaskTimer()
{
}


void TaskTimer::stop()
{
	if (!m_start.load(std::memory_order_acquire) || m_done.load(std::memory_order_acquire))
	{
		return;
	}
	else
	{
		m_stop.store(true, std::memory_order_release);
	}
}

void TaskTimer::pause()
{
	if (m_start.load(std::memory_order_acquire) && !m_done.load(std::memory_order_acquire) && !m_pause.load(std::memory_order_acquire))
	{
		m_pause.store(true, std::memory_order_release);
		//Calculate the paused ticks
		m_pause_ticks = SDL_GetTicks() - m_start_ticks;
		m_start_ticks = 0;
	}
}

void TaskTimer::resume()
{
	if (m_start.load(std::memory_order_acquire) && !m_done.load(std::memory_order_acquire) && m_pause.load(std::memory_order_acquire))
	{
		m_pause.store(false, std::memory_order_release);

		//Reset the starting ticks
		m_start_ticks = SDL_GetTicks() - m_pause_ticks;

		//Reset the paused ticks
		m_pause_ticks = 0;
	}
}

bool TaskTimer::is_done()
{
	return m_done.load(std::memory_order_acquire);
}

bool TaskTimer::is_start()
{
	return m_start.load(std::memory_order_acquire);
}

void TaskTimer::setInterval(uint32_t interval)
{
	m_interval = interval;
}

uint32_t TaskTimer::getTicks()
{
	//The actual timer time
	uint32_t time = 0;

	//If the timer is running
	if (m_start.load(std::memory_order_acquire))
	{
		//If the timer is paused
		if (m_pause.load(std::memory_order_acquire))
		{
			//Return the number of ticks when the timer was paused
			time = m_pause_ticks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - m_start_ticks;
		}
	}

	return time;
}


void Task::set_post_processing(const std::function<void(Task*)> &funct)
{
	AutoSpinLock lk(m_internal_lock);
	m_post_processing = funct;
}

void Task::wait()
{
	//blocking on result
	m_stop.store(true, std::memory_order_release);
	return m_f_result.wait();
}

RepetitionTask::~RepetitionTask()
{
}

RepetitionTask::RepetitionTask(const uint32_t &interval, const callable &funct) :m_action(funct)
{
	_ASSERT(nullptr != m_action);
	m_interval = interval;
}


/**
 * @brief      { function_description }
 */
void RepetitionTask::start()
{
	//Be sure thread is not started yet
	_ASSERT(!m_done.load(std::memory_order_acquire) && !m_start.load(std::memory_order_acquire));

	//set start flag
	m_start.store(true, std::memory_order_release);

	//Get the current clock time
	m_start_ticks = SDL_GetTicks();

	auto main_loop = [&]() {
		//set thread id
		m_thread_id = std::this_thread::get_id();

		//loop until stop
		while (!m_stop.load(std::memory_order_acquire))
		{
			//check on pause
			if (!m_pause.load(std::memory_order_acquire))
			{
				m_action();
				uint32_t run_time = this->getTicks();
				if (run_time < m_interval)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(m_interval - run_time));
				}
			}
		}

		//do post processing
		m_post_processing(this);

		//Set done flag on finished
		m_done.store(true, std::memory_order_release);

		std::stringstream stream;
		stream << std::hex << std::setw(8) << m_thread_id;
		printf("OnceTimeTask::%s ThreadID[0x%s] done/n", __FUNCTION__, stream.str().c_str());

		return;
	};

	Task::m_f_result = std::async(std::launch::async, main_loop);

}

/*!
 * ... text ...
 *
 * @return     { description_of_the_return_value }
 */
Task* RepetitionTask::Clone()
{
	return new RepetitionTask(m_interval, m_action);
}


OnceTimeTask::OnceTimeTask(const callable &funct) :m_action(funct)
{
	_ASSERT(nullptr != funct);
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
	//Be sure thread is not started yet
	_ASSERT(!m_done.load(std::memory_order_acquire) && !m_start.load(std::memory_order_acquire));

	//set start flag
	m_start.store(true, std::memory_order_release);

	auto function = [&]()
	{
		//set thread id
		m_thread_id = std::this_thread::get_id();

		m_action();

		//do post processing
		m_post_processing(this);

		//Set done flag on finished
		m_done.store(true, std::memory_order_release);

		std::stringstream stream;
		stream << std::hex << std::setw(8) << m_thread_id;
		printf("OnceTimeTask::%s ThreadID[0x%s] done/n", __FUNCTION__, stream.str().c_str());
	};
	Task::m_f_result = std::async(std::launch::async, function);
}

/*!
 * ... text ...
 *
 * @return     { description_of_the_return_value }
 */
Task* OnceTimeTask::Clone()
{
	return new OnceTimeTask(m_action);
}


task_identifier::task_identifier(Task &task, uint64_t msg_id) :m_thread_id(&task.m_thread_id),__task(&task), __msg_id(msg_id){}
task_identifier::task_identifier() {}
task_identifier::~task_identifier() {}

bool task_identifier::operator==(const Task& task) const
{
	return (m_thread_id == &task.m_thread_id);
}

task_identifier& task_identifier::operator=(const task_identifier& other)
{
	m_thread_id = other.m_thread_id;
	__task = other.__task;
	__msg_id = other.__msg_id;
	return *this;
}
