#include "stdafx.h"
#include "TaskPool.h"

/*!
* ... text ...
*/
TaskPool::TaskPool(const uint32_t &interval)
{
	m_interval = interval;
}
/*!
* ... text ...
*/
TaskPool::~TaskPool()
{

}
/*!
* ... text ...
*/
Task* TaskPool::Clone()
{
	return nullptr;
}

/*!
* ... text ...
*/
void TaskPool::addPendingTask(Task *pTask)
{
	_ASSERT(!pTask->is_start());
	m_taskPending.push(pTask);
}
/*!
* ... text ...
*/
void TaskPool::removeTask(Task* pTask)
{
	//<check task pending queue and task doing queue
	m_taskPending.remove(pTask);
	m_taskDoing.remove(pTask);
}
/*!
* ... text ...
*/
void TaskPool::start()
{
	m_f_result = std::async(std::launch::async, &TaskPool::processing, this);
}
/*!
* ... text ...
*/
void TaskPool::wait()
{
	//<stop main loop
	m_stop.store(true, std::memory_order_release);

	//<wakeup processing thread
	m_taskPending.notify_all();

	//<wait until main loop end
	m_f_result.wait();

	//<wait until all doing task completed
	auto stop_and_delete_task = [](Task* task) -> void
	{
		task->stop();
		task->wait();
		delete task;
	};

	//<stop all on processing task
	m_taskDoing.do_each(stop_and_delete_task);

	//<Delete task pointer on pending and done list
	auto delete_task = [](Task* task) -> void {delete task; };
	m_taskPending.do_each(delete_task);
	m_taskDone.do_each(delete_task);
}

/*!
* ... text ...
*/
void TaskPool::processing()
{
	auto ClearDoneTaskFunct = [&](Task* task) -> bool
	{
		bool done = false;
		if (task->is_done())
		{
			done = true;
			m_taskDone.push(task);
		}
		return done;
	};

	//<loop until stop
	while (!m_stop.load(std::memory_order_acquire))
	{
		//<check pending task list
		m_taskPending.wait_until_have_one(m_interval);

		if (!m_taskPending.empty())
		{
			//<start and push to doing task queue
			Task* t = m_taskPending.pop();
			t->start();
			m_taskDoing.push(t);
		}

		//<check and move finished task to done task queue
		m_taskDoing.remove_if(ClearDoneTaskFunct);

	}

	//<Set done flag on finished
	m_done.store(true, std::memory_order_release);

	return;
}
