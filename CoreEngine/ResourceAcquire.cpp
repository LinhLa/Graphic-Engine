#include "stdafx.h"
#include <ResourceAcquire.h>
#include "Scene.h"
#include "time_suffix.h"
ResourceAcquire::ResourceAcquire():m_timeout(300)
{}

ResourceAcquire::~ResourceAcquire() {}

UIObjectPtr ResourceAcquire::QuerryResourceSync(std::string url)
{
	return  Scene::GetInstance()->GetResource(url);
}

void ResourceAcquire::QuerryResourceAsync(ResourceListType&)
{
	///<TODO: add timer here bind to lambda *object = Scene::GetInstance()->GetResource(url);
	//AutoSpinLock auto_lock(m_internal_lock);
	//m_acquire_table[std::this_thread::get_id()].push_back(url);
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

void ResourceAcquire::ResourceAcquired(std::string url)
{
	{
		AutoSpinLock auto_lock(m_internal_lock);
		m_acquire_table[std::this_thread::get_id()].erase(url);
	}
	m_condition_any.notify_all();
}