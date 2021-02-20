#pragma once
#include <map>
#include <list>
#include <memory>
#include <vector>
#include "SingletonHolder.h"
#include "TaskPool.h"
#include "IService.h"
#include "message_struct.h"
#include "spinlock.h"
#include "base_define.h"

typedef std::list<OnceTimeTask> listTaskType;
typedef std::list<IService*> listServiceType;
typedef std::list<MessagePtr> messageQueueType;

class ServiceMgr: public SingletonHolder<ServiceMgr>
{
private:
	friend class  SingletonHolder<ServiceMgr>;
	spinlock 									m_internal_lock;
	messageQueueType							m_message_queue;
	std::vector<std::shared_ptr<IService> > 	m_service_directory;
	TaskPoolPtr									m_pTaskPool = nullptr;
	std::map<uint64_t, listServiceType> m_service_handlers;

	ServiceMgr();
	~ServiceMgr();

	void broadcastMessage(MessagePtr);
	void onStartService();

public:
	void dispatch();
	void AttachService(IService*);
	void StartService();
	void EndService();

	uint32_t addOnceTimeTask(TaskPtr pTask);
	uint32_t addRepeateTask(TaskPtr pTask);
	void removeTask(uint32_t id);
	void executeTaskList();

	void sendKeyInputMessage(MessagePtr);
	void sendMessage(MessagePtr);

	void registerEvent(IService* service, uint64_t event_id);
	void unregisterEvent(IService* service, uint64_t event_id);

	template<typename SERVICE>
	SERVICE* getServiceInstance(SERVICE_ID service_id)
	{
		return dynamic_cast<SERVICE*>(m_service_directory[service].get());
	}
};