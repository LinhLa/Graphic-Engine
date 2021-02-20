#include "stdafx.h"
#include "TaskPool.h"

/**
 * @brief      Constructs a new instance.
 */
TaskPool::TaskPool()
{}

/**
 * @brief      Destroys the object.
 */
TaskPool::~TaskPool()
{
	m_OnceTimeTaskList.clear();
	m_RepeateTaskList.clear();
}

/*!
 * ... text ...
 *
 * @param[in]  task  The task
 *
 * @return     { description_of_the_return_value }
 */
uint32_t TaskPool::addOnceTimeTask(TaskPtr pTask)
{
	uint32_t id = GenerateID();
	m_OnceTimeTaskList.insert({ id, pTask });
	return id;
}

/**
 * @brief      Adds a repeate task.
 *
 * @param[in]  task  The task
 *
 * @return     { description_of_the_return_value }
 */
uint32_t TaskPool::addRepeateTask(TaskPtr pTask)
{
	uint32_t id = GenerateID();
	m_RepeateTaskList.insert({ id , pTask });
	return id;
}

/*!
 * ... text ...
 *
 * @param[in]  id    The identifier
 */
void TaskPool::removeTask(uint32_t id)
{
	m_OnceTimeTaskList.erase(id);
	m_RepeateTaskList.erase(id);
}

/**
 * @brief      { function_description }
 */
void TaskPool::executeTaskList()
{
	for(auto itr = m_OnceTimeTaskList.begin(); itr != m_OnceTimeTaskList.end(); )
	{
		auto pTask = std::dynamic_pointer_cast<OnceTimeTask>(itr->second);
		if (nullptr == pTask)
		{
			itr = m_OnceTimeTaskList.erase(itr);
			continue;
		}

		if (pTask->isDone())
		{
			itr = m_OnceTimeTaskList.erase(itr);
			continue;
		}
        else
        {
        	itr->second->start();
            ++itr;
        }
    }

    for(auto itr = m_RepeateTaskList.begin(); itr != m_RepeateTaskList.end(); itr++)
	{
	   	itr->second->start();
    }
}

/*!
 * ... text ...
 *
 * @return     { description_of_the_return_value }
 */
uint32_t TaskPool::GenerateID()
{
	static uint32_t id = 0;
	return ++id;
}