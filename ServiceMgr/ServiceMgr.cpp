#include "stdafx.h"
#include <ServiceMgr.h>
#include <log.h>
#include <base_define.h>

#include <TestService.h>
#include <MemoryTracker.h>
#include <SceneService.h>
#include <RenderService.h>
#include <ConfigService.h>
#include <FrameService.h>
#include <ContentService.h>
#include <LogService.h>

ServiceMgr::ServiceMgr() :
	m_scheduler(SCHEDULER_INTERVAL),
	m_message_dispatcher(std::bind(&ServiceMgr::dispatch, this))
{
}

ServiceMgr::~ServiceMgr()
{
}

void ServiceMgr::InitService()
{
	m_service_directory[TEST_SERVICE_ID] = std::make_shared<TestService>();
	m_service_directory[MEMORY_TRACKER_SERVICE_ID] = std::make_shared<MemoryTracker>();
	m_service_directory[SCENE_SERVICE_ID] = std::make_shared<SceneService>();
	m_service_directory[RENDER_SERVICE_ID] = std::make_shared<RenderService>();
	m_service_directory[CONFIG_SERVICE_ID] = std::make_shared<ConfigService>();
	m_service_directory[FRAME_SERVICE_ID] = std::make_shared<FrameService>();
	m_service_directory[CONTENT_SERVICE_ID] = std::make_shared<ContentService>();
	m_service_directory[LOG_SERVICE_ID] = std::make_shared<LogService>();
}

void ServiceMgr::onStartService()
{
	//<start all service
	for (auto &service : m_service_directory)
	{
		service->onStartService();
	}

	//<start scheduler
	m_scheduler.start();
}

void ServiceMgr::EndService()
{
	//<finish all task in scheduler before end other services
	m_scheduler.wait();

	//<call end service for each service
	for (auto &Service : m_service_directory)
	{
		Service->onEndService();
	}
}

void ServiceMgr::broadcastMessageToServices(const Message &message)
{
	//<publish message event to register service
	for (auto &Service : m_service_handlers[message.m_id])
	{
		Service->onMessageReceive(message);
	}
}

void ServiceMgr::broadcastMessageToServicesAsync(const Message &message)
{
	//<publish message event to register service
	for (auto &Service : m_service_handlers[message.m_id])
	{
		Task *pTask = new SubTask<IService, void, Message>(Service, &IService::onMessageReceive, message);
		m_scheduler.addPendingTask(pTask);
	}
}

void ServiceMgr::dispatch()
{
	while (1)
	{
		//<wait until message queue not empty
		m_message_queue.wait_until_have_one();

		Message msg = m_message_queue.pop();
		printf("%s : MessageID[%lld]\n", __FUNCTION__, msg.m_id);
		switch (msg.m_id)
		{
		case SERVICE_START:
			onStartService();
			break;
		case SERVICE_END:
			EndService();
			return;
		default:
			//<broadcast message to all other services
			broadcastMessageToServices(msg);
			break;
		}

		//<execute callback list
		if (!m_event_handlers[msg.m_id].empty())
		{
			listTaskType &task_list = m_event_handlers[msg.m_id];

			//<generate task associated with message id to scheduler
			for (auto pTask : task_list)
			{
				m_scheduler.addPendingTask(pTask->Clone());
			}
		}


	}//<End main loop
}

void ServiceMgr::StartService()
{
	FUNC_IN("");

	//<send message to start all service
	sendMessage({ SERVICE_START, 0ULL });

	//<start Message dispatch
	m_message_dispatcher.start();

	if (true == m_message_queue.wait_for({ RENDER_CONFIG_LOADED, 0ULL }))
	{
		//< Run render service processing
		std::dynamic_pointer_cast<RenderService>(m_service_directory[RENDER_SERVICE_ID])->processing();
	}

	//<Send message to end all service
	sendMessage({ SERVICE_END, 0ULL });

	//<wait for message dispatch stop
	m_message_dispatcher.wait();

	FUNC_OUT("");
}

void ServiceMgr::sendMessage(const Message &message)
{
	m_message_queue.push(message);
}

bool ServiceMgr::waitForMessage(uint64_t MSG_ID)
{
	return m_message_queue.wait_for({ MSG_ID, 0ULL });
}

void ServiceMgr::sendMessageToUIThread(const Message &message)
{
	SDL_Event event;
	event.type = SDL_USEREVENT;
	_ASSERT(message.m_id <= ULONG_MAX);
	event.user.code = message.m_id & ULONG_MAX;
	_ASSERT(1 == SDL_PushEvent(&event));
}

void ServiceMgr::registerOnEvent(IService* service, uint64_t service_event_id)
{
	_ASSERT(service);
	m_service_handlers[service_event_id].push_back(service);
}

void ServiceMgr::unregisterOnEvent(IService* service, uint64_t service_event_id)
{
	_ASSERT(service);
	m_service_handlers[service_event_id].remove(service);
}

task_identifier ServiceMgr::addRepetitionTask(uint32_t m_interval, callable function)
{
	Task* pTask = new RepetitionTask(m_interval, function);
	m_scheduler.addPendingTask(pTask);
	return{ *pTask };
}

task_identifier ServiceMgr::addOnceTimeTask(uint64_t service_event_id, callable function)
{
	Task* pTask = new OnceTimeTask(function);
	m_event_handlers[service_event_id].push(pTask);
	return{ *pTask, service_event_id };
}

task_identifier ServiceMgr::addBackgroundTask(callable function)
{
	Task* pTask = new OnceTimeTask(function);
	m_scheduler.addPendingTask(pTask);
	return{ *pTask };
}

void ServiceMgr::removeRepetitionTask(const task_identifier& id)
{
	//<repetition task only on scheduler
	m_scheduler.removeTask(static_cast<Task*>(id.__task));
}

void ServiceMgr::removeOnceTimeTask(const task_identifier& id)
{
	Task* pTask = static_cast<Task*>(id.__task);
	if (SERVICE_START == id.__msg_id)
	{
		//<is background task
		m_scheduler.removeTask(pTask);
	}
	else
	{
		//<is event handler task
		m_event_handlers[id.__msg_id].remove(pTask);
	}
}

void ServiceMgr::removeBackgroundTask(const task_identifier& id)
{
	Task* pTask = static_cast<Task*>(id.__task);
	//<background task only on scheduler
	m_scheduler.removeTask(pTask);
}