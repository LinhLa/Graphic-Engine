#include "stdafx.h"
#include "MemoryTracker.h"
#include "memmgr.h"

MemoryTracker::MemoryTracker(){}
MemoryTracker::~MemoryTracker() {}

void MemoryTracker::MemoryReport()
{
	int64_t data[3] = { IMemoryManager::total(), IMemoryManager::usage(), IMemoryManager::free() };
	ServiceMgr::GetInstance()->sendMessage(Message::create(MEMORY_REPORT, reinterpret_cast<void*>(data), sizeof(data)));
}

void MemoryTracker::onEndService()
{
	ServiceMgr::GetInstance()->removeTask(m_memory_report_task_id);
}
void MemoryTracker::onMessageReceive(MessagePtr msg)
{

}

void MemoryTracker::onStartService()
{
	LOG_DEBUG("MemoryTracker is started");
	TaskPtr ptask = RepetitionTask::create(10000, std::bind(&MemoryTracker::MemoryReport, this));
	m_memory_report_task_id = ServiceMgr::GetInstance()->addRepeateTask(ptask);
}