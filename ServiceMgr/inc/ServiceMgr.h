#pragma once
#include "base_define.h"
#include "type_define.h"
#include "SingletonHolder.h"
#include "TaskPool.h"
#include <map>

class ServiceMgr: public SingletonHolder<ServiceMgr>
{
private:
	friend class  SingletonHolder<ServiceMgr>;

	messageQueueType				m_message_queue;
	std::shared_ptr<IService> 		m_service_directory[SERVICE_ID_MAX];

	std::map<uint64_t, listServiceType> m_service_handlers;
	std::map<uint64_t, listTaskType>	m_event_handlers;

	TaskPool		m_scheduler;
	OnceTimeTask	m_message_dispatcher;

	ServiceMgr();
	~ServiceMgr();

	void dispatch();

	void broadcastMessageToServices(const Message&);
	void broadcastMessageToServicesAsync(const Message&);

	void EndService();

public:
	void InitService();
	void onStartService();
	void StartService();

	void sendMessage(const Message &message);
	bool waitForMessage(uint64_t);
	void sendMessageToUIThread(const Message &message);

	void registerOnEvent(IService* service, uint64_t service_event_id);
	void unregisterOnEvent(IService* service, uint64_t service_event_id);

	task_identifier addRepetitionTask(uint32_t m_interval, callable function);
	task_identifier addOnceTimeTask(uint64_t service_event_id, callable function);
	task_identifier addBackgroundTask(callable function);

	void removeOnceTimeTask(const task_identifier& id);
	void removeBackgroundTask(const task_identifier& id);
	void removeRepetitionTask(const task_identifier& id);

	template<typename SERVICE>
	SERVICE* getServiceInstance(SERVICE_ID servie_id)
	{
		return dynamic_cast<SERVICE*>(m_service_directory[servie_id].get());
	}
};