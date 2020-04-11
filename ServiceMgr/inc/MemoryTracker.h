#pragma once
#include "SingletonHolder.h"
#include "base_define.h"
#include "type_define.h"
class MemoryTracker: public IService, public SingletonHolder<MemoryTracker>
{
private:
	friend class ServiceMgr;
	friend class SingletonHolder<MemoryTracker>;
	task_identifier m_memory_report_task_id;

	void MemoryReport();
public:
	MemoryTracker();
	~MemoryTracker();

	void onStartService() override;
	void onEndService() override;
	void onMessageReceive(const Message&) override;
};


typedef std::shared_ptr<MemoryTracker> MemoryTrackerPtr;