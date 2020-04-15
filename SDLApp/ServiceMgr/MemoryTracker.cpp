#include "stdafx.h"
#include "ServiceMgr.h"
#include "MemoryTracker.h"

MemoryTracker::MemoryTracker(){}
MemoryTracker::~MemoryTracker() {}

void MemoryTracker::MemoryReport()
{
	int64_t data[3] = { IMemoryManager::total(), IMemoryManager::usage(), IMemoryManager::free() };
	m_pServiceMgr->sendMessage({ MEMORY_REPORT, reinterpret_cast<void*>(data), sizeof(data)});
}

void MemoryTracker::onEndService()
{
	m_pServiceMgr->removeRepetitionTask(m_memory_report_task_id);
}
void MemoryTracker::onMessageReceive(const Message &msg)
{

}

void MemoryTracker::onStartService()
{
	m_memory_report_task_id = m_pServiceMgr->addRepetitionTask(10000 , std::bind(&MemoryTracker::MemoryReport, this));
}