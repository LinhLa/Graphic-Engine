#pragma once
#include <atomic>
#include <future>
#include <functional>
#include <chrono>
#include <function_define.h>
#include "spinlock.h"
#include "memmgr.h"

typedef std::function<void(void)> callable;
class Task;
struct TaskDetruction
{
	static void destruct(Task* pTask);
};

class TaskTimer
{
protected:
	TaskTimer();
	virtual ~TaskTimer();

	std::atomic_bool m_stop = false;
	std::atomic_bool m_pause = false;
	std::atomic_bool m_done = false;
	std::atomic_bool m_start = false;

	//The clock time when the timer started
	uint32_t m_start_ticks = 0;

	//The ticks stored when the timer was paused
	uint32_t m_pause_ticks = 0;

	uint32_t m_interval = ~0;

public:
	void stop();
	void pause();
	void resume();
	bool is_done();
	bool is_start();

	uint32_t getTicks();
	void setInterval(uint32_t interval);
};

class Task;
class TaskClone
{
protected:
	TaskClone() {}
	virtual ~TaskClone() {}
public:
	virtual Task* Clone() = 0;

};

struct task_identifier;
class Task : public TaskTimer, public TaskClone
{
protected:
	friend class ServiceMgr;
	friend struct task_identifier;

	Task() {};

	mutable spinlock m_internal_lock;
	std::function<void(Task*)> m_post_processing = [](Task*) {return; };

	std::future<void> 	m_f_result;
	std::thread::id	  	m_thread_id;
public:
	virtual void start() = 0;

	void set_post_processing(const std::function<void(Task*)> &funct);

	void wait();
	std::future_status wait_for(uint32_t timeout = 300);
	virtual ~Task() {};
};

class RepetitionTask final : public Task
{
private:
	callable m_action = []() {return; };
public:
	RepetitionTask(const uint32_t &interval, const callable &funct);
	~RepetitionTask();

	void start() override;
	Task* Clone() override;

	FREE_LIST_MEMORY_MANAGER(RepetitionTask)
};

class OnceTimeTask final : public Task
{
private:
	callable m_action = []() {return; };
public:
	OnceTimeTask(const callable &funct);
	~OnceTimeTask();

	void start() override;
	Task* Clone() override;

	FREE_LIST_MEMORY_MANAGER(OnceTimeTask)
};

template<class CLASS, class RET, class PARAM>
class SubTask final : public Task
{
private:
	typedef RET(CLASS::*METHOD)(const PARAM&);
	PARAM		m_PrivateData;
	CLASS*		m_object = nullptr;
	METHOD		m_action = nullptr;
public:
	SubTask(CLASS *object, METHOD action, const PARAM& param) : m_object(object), m_action(action), m_PrivateData(param)
	{
		_ASSERT(m_object && m_action);
	}

	~SubTask() {}

	void start() override
	{
		//Be sure thread is not started yet
		_ASSERT(!m_start.load(std::memory_order_acquire));

		//set start flag
		m_start.store(true, std::memory_order_release);

		auto function = [&]()
		{
			//set thread id
			m_thread_id = std::this_thread::get_id();

			(m_object->*m_action)(m_PrivateData);

			//Set done flag on finished
			m_done.store(true, std::memory_order_release);

			//do post processing
			m_post_processing(this);

		};
		Task::m_f_result = std::async(std::launch::async, function);
	}

	Task* Clone() override
	{
		return new SubTask(m_object, m_action, m_PrivateData);
	}

	//FREE_LIST_MEMORY_MANAGER(ServiceTask)
};


class ServiceMgr;
struct task_identifier
{
public:
	task_identifier(Task &task, uint64_t msg_id = 0);
	task_identifier();
	~task_identifier();
	bool operator==(const Task& task) const;
	task_identifier& operator=(const task_identifier& other);
	FREE_LIST_MEMORY_MANAGER(task_identifier)
private:
	friend class ServiceMgr;
	std::string m_thread_name;
	std::thread::id* m_thread_id = nullptr;
	void* __task = nullptr;
	uint64_t __msg_id = 0;
};
