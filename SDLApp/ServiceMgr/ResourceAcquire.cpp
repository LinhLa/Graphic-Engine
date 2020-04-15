#include "stdafx.h"
#include <ResourceAcquire.h>
#include <ServiceMgr.h>

ResourceAcquire::ResourceAcquire():m_pServiceMgr(ServiceMgr::GetInstance()), m_timeout(300)
{
	_ASSERT(m_pServiceMgr);
}

ResourceAcquire::~ResourceAcquire() {}

bool ResourceAcquire::SendQuerryResourceMessageSync(GameObject** address_pointer, UI_TYPE type, const char* resource_name)
{
	std::thread::id thread_id = std::this_thread::get_id();
	bool bIsResourceAcquired = false;

	AutoSpinLock auto_lock(m_internal_lock);
	m_acquire_table[thread_id].push_back(address_pointer);
	m_pServiceMgr->sendMessage({ SCENE_QUERRY_RESOURCE_SYNC, message_resource(reinterpret_cast<void**>(address_pointer), int32_t(type), resource_name) });

	auto waitFunct = [&]() -> bool
	{
		ResourceListType &ref_list = m_acquire_table[thread_id];
		if (ref_list.empty())
		{
			bIsResourceAcquired = true;
		}
		return bIsResourceAcquired;
	};

	m_condition_any.wait_until(m_internal_lock, std::chrono::system_clock::now() + m_timeout * 1_ms, waitFunct);
	return bIsResourceAcquired;
}

void ResourceAcquire::SendQuerryResourceMessageAsync(GameObject** address_pointer, UI_TYPE type, const char* resource_name)
{
	m_pServiceMgr->sendMessage({ SCENE_QUERRY_RESOURCE_ASYNC, message_resource(reinterpret_cast<void**>(address_pointer), int32_t(type), resource_name) });

	AutoSpinLock auto_lock(m_internal_lock);
	m_acquire_table[std::this_thread::get_id()].push_back(address_pointer);
}

bool ResourceAcquire::WaitResourceAcquire(std::thread::id thread_id, uint32_t timeout)
{
	bool bIsResourceAcquired = false;
	AutoSpinLock auto_lock(m_internal_lock);
	if (m_acquire_table.find(thread_id) == m_acquire_table.end())
	{
		return false;
	}

	auto waitFunct = [&]() -> bool
	{
		ResourceListType &ref_list = m_acquire_table[thread_id];
		if (ref_list.empty())
		{
			bIsResourceAcquired = true;
		}
		return bIsResourceAcquired;
	};
	m_condition_any.wait_until(m_internal_lock, std::chrono::system_clock::now() + timeout * 1_ms, waitFunct);
	return bIsResourceAcquired;
}

void ResourceAcquire::ResourceAcquired(const message_resource& msg_resource)
{
	{
		AutoSpinLock auto_lock(m_internal_lock);
		m_acquire_table[msg_resource.m_thread_id].remove((GameObject**)(msg_resource.m_pObject));
	}
	m_condition_any.notify_all();
}

void ResourceAcquire::AddResourceMessageAsync(GameObject** address_pointer)
{
	_ASSERT(address_pointer);
	m_pServiceMgr->sendMessage({ SCENE_ADD_RESOURCE, message_resource(reinterpret_cast<void**>(address_pointer), int32_t(0), NULL) });
}
