#pragma once
#include <map>
#include "TaskPool.h"
#include "SingletonHolder.h"
class TaskPoolInstance final: public SingletonHolder<TaskPoolInstance>
{
private:
	TaskPoolPtr m_pTaskPool = nullptr;
	TaskPoolInstance();
public:
	friend class SingletonHolder<TaskPoolInstance>;
	~TaskPoolInstance();

	uint32_t addOnceTimeTask(TaskPtr pTask);
	uint32_t addRepeateTask(TaskPtr pTask);
	void removeTask(uint32_t id);
	void executeTaskList();
};
