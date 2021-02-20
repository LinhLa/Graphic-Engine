#include "stdafx.h"
#include <ServiceMgr.h>
#include <log.h>
#include <base_define.h>

ServiceMgr::ServiceMgr()
{
	m_pTaskPool = TaskPool::create();
}

ServiceMgr::~ServiceMgr()
{
}

void ServiceMgr::AttachService(IService* pService)
{
	if (pService)
	{
		m_service_directory.push_back(std::shared_ptr<IService>(pService));
	}
}

void ServiceMgr::onStartService()
{
	//<start all service
	for (auto &service : m_service_directory)
	{
		service->onStartService();
	}
}

/**
 * @brief      Ends a service.
 */
void ServiceMgr::EndService()
{
	//<call end service for each service
	for (auto &Service : m_service_directory)
	{
		Service->onEndService();
	}
}


/*!
* ... text ...
*
* @param[in]  task  The task
*
* @return     { description_of_the_return_value }
*/
uint32_t ServiceMgr::addOnceTimeTask(TaskPtr pTask)
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
uint32_t ServiceMgr::addRepeateTask(TaskPtr pTask)
{
	return m_pTaskPool->addRepeateTask(pTask);
}

/*!
* ... text ...
*
* @param[in]  id    The identifier
*/
void ServiceMgr::removeTask(uint32_t id)
{
	m_pTaskPool->removeTask(id);
}

/**
* @brief      { function_description }
*/
void ServiceMgr::executeTaskList()
{
	m_pTaskPool->executeTaskList();
}

void ServiceMgr::broadcastMessage(MessagePtr message)
{
	//<publish message event to register service
	for (auto &Service : m_service_handlers[message->m_id])
	{
		Service->onMessageReceive(message);
	}
}

/**
 * @brief      { function_description }
 */
void ServiceMgr::dispatch()
{
	messageQueueType messageList;
	{
		AutoSpinLock auto_lock(m_internal_lock);
		messageList = m_message_queue;
		m_message_queue.clear();
	}
	for (auto &msg : messageList)
	{
		LOG_DEBUG("MessageID[%lld]", msg->m_id);
		broadcastMessage(msg);
	}
	//<execute timer callback list
	m_pTaskPool->executeTaskList();
}

/**
 * @brief      Starts a service.
 */
void ServiceMgr::StartService()
{
	FUNC_IN("");

	for (auto &Service : m_service_directory)
	{
		Service->onStartService();
	}

	FUNC_OUT("");
}

/**
 * @brief      Sends a message.
 *
 * @param[in]  message  The message
 */
void ServiceMgr::sendMessage(MessagePtr message)
{
	AutoSpinLock auto_lock(m_internal_lock);
	m_message_queue.push_back(message);
}


/**
 * @brief      Sends a key input message.
 *
 * @param      event  The event
 */
void ServiceMgr::sendKeyInputMessage(MessagePtr msg)
{
	broadcastMessage(msg);
}

/**
 * @brief      { function_description }
 *
 * @param      service           The service
 * @param[in]  event_id  The service event identifier
 */
void ServiceMgr::registerEvent(IService* service, uint64_t event_id)
{
	_ASSERT(service);
	m_service_handlers[event_id].push_back(service);
}

/**
 * @brief      { function_description }
 *
 * @param      service           The service
 * @param[in]  event_id  The service event identifier
 */
void ServiceMgr::unregisterEvent(IService* service, uint64_t event_id)
{
	_ASSERT(service);
	m_service_handlers[event_id].remove(service);
}