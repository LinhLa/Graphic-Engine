#pragma once
#include <unordered_map>
#include "Task.h"
#include "creator.h"
class TaskPool final: public creator<TaskPool>
{
private:
	uint32_t GenerateID();
	std::unordered_map<uint32_t, TaskPtr> m_OnceTimeTaskList;
	std::unordered_map<uint32_t, TaskPtr> m_RepeateTaskList;
	TaskPool();
public:
	friend class creator<TaskPool>;
	~TaskPool();

	uint32_t addOnceTimeTask(TaskPtr pTask);
	uint32_t addRepeateTask(TaskPtr pTask);
	void removeTask(uint32_t id);
	void executeTaskList();
};

typedef std::shared_ptr<TaskPool> TaskPoolPtr;
