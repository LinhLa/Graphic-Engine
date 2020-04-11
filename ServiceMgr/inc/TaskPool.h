#pragma once
#include "type_define.h"
#include <condition_variable>

class TaskPool: public Task
{
private:
	uint32_t m_interval = ~0;

	listTaskType m_taskPending;
	listTaskType m_taskDoing;
	listTaskType m_taskDone;

public:
	TaskPool(const uint32_t &interval);
	virtual ~TaskPool();

	void addPendingTask(Task *pTask);
	void removeTask(Task *pTask);

	void processing();

	void start() override;
	void wait() override;
	Task* Clone() override;

};
