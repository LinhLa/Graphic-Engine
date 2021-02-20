#pragma once
#include "SingletonHolder.h"
#include "base_define.h"
#include "ServiceMgr.h"
class MemoryTracker: public IService, public SingletonHolder<MemoryTracker>
{
private:
	friend class ServiceMgr;
	friend class SingletonHolder<MemoryTracker>;
	uint32_t m_memory_report_task_id;

	void MemoryReport();
public:
	MemoryTracker();
	~MemoryTracker();

	void onStartService() override;
	void onEndService() override;
	void onMessageReceive(MessagePtr) override;
};


typedef std::shared_ptr<MemoryTracker> MemoryTrackerPtr;