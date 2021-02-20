#include "stdafx.h"
#include "TaskPoolInstance.h"

/**
 * @brief      Constructs a new instance.
 */
TaskPoolInstance::TaskPoolInstance()
{
	m_pTaskPool = TaskPool::create();
}

/**
 * @brief      Destroys the object.
 */

TaskPoolInstance::~TaskPoolInstance(){}

/*!
 * ... text ...
 *
 * @param[in]  task  The task
 *
 * @return     { description_of_the_return_value }
 */
uint32_t TaskPoolInstance::addOnceTimeTask(TaskPtr pTask)
{
	return m_pTaskPool->addOnceTimeTask(pTask);
}

/**
 * @brief      Adds a repeate task.
 *
 * @param[in]  task  The task
 *
 * @return     { description_of_the_return_value }
 */
uint32_t TaskPoolInstance::addRepeateTask(TaskPtr pTask)
{
	return m_pTaskPool->addRepeateTask(pTask);
}

/*!
 * ... text ...
 *
 * @param[in]  id    The identifier
 */
void TaskPoolInstance::removeTask(uint32_t id)
{
	m_pTaskPool->removeTask(id);
}

/**
 * @brief      { function_description }
 */
void TaskPoolInstance::executeTaskList()
{
	m_pTaskPool->executeTaskList();
}