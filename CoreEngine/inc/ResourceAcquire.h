#pragma once
#include <UIObject.h>
#include <spinlock.h>
#include <SingletonHolder.h>
#include <unordered_map>
#include <condition_variable>

typedef std::unordered_map<std::string, UIObject*> ResourceListType;
typedef std::unordered_map<std::thread::id,	ResourceListType> ObjectTableType;

class ServiceMgr;
class ResourceAcquire final : public SingletonHolder<ResourceAcquire>
{
private:
	friend class SingletonHolder<ResourceAcquire>;

	ObjectTableType		m_acquire_table;
	spinlock			m_internal_lock;
	std::condition_variable_any m_condition_any;

	uint32_t m_timeout;
protected:
	ResourceAcquire();
	~ResourceAcquire();
public:
	UIObjectPtr QuerryResourceSync(std::string);
	void QuerryResourceAsync(ResourceListType&);

	bool WaitResourceAcquire(std::thread::id thread_id = std::this_thread::get_id(), uint32_t timeout = 300/*miliseconds*/);
	void ResourceAcquired(std::string url);
};

/*
Object: pointer to pointer object
Type:	enum for classification object
Asset:  unique resource name
*/
#define QUERRY_RESOURCE_ASYNC(url, refobj) \
(ResourceAcquire::GetInstance()->QuerryResourceAsync(url,refobj))

#define QUERRY_RESOURCE_SYNC(url) \
(ResourceAcquire::GetInstance()->QuerryResourceSync(url))
