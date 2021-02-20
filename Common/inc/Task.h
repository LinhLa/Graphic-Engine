#pragma once
#include <functional>
#include "timer.h"
#include "creator.h"
typedef std::function<void(void)> callable;
class Task
{
public:
	Task() {};
	virtual void start() = 0;
	virtual ~Task() {};
};

typedef std::shared_ptr<Task> TaskPtr;

class RepetitionTask final : public Task, public creator<RepetitionTask>
{
private:
	callable 	m_action = nullptr;
	timer		m_timer;
	uint64_t 	m_interval = 0;
	RepetitionTask(uint64_t interval, callable funct);
public:
	friend class creator<RepetitionTask>;
	virtual ~RepetitionTask();

	void start() override;
};

class OnceTimeTask final : public Task, public creator<OnceTimeTask>
{
private:
	callable 	m_action = nullptr;
	timer		m_timer;
	uint64_t 	m_delay = 0;
	bool		m_isDone = false;
	OnceTimeTask(uint64_t delay, callable funct);
public:
	friend class creator<OnceTimeTask>;
	virtual ~OnceTimeTask();

	void start() override;
	bool isDone() const;
};