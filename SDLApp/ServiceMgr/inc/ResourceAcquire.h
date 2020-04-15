#pragma once
#include <GameObject.h>
#include <spinlock.h>
#include <SingletonHolder.h>
#include <unordered_map>

typedef std::list<GameObject**> ResourceListType;

typedef std::unordered_map<std::thread::id,	ResourceListType> ObjectTableType;

class ServiceMgr;
class ResourceAcquire final : public SingletonHolder<ResourceAcquire>
{
private:
	friend class SingletonHolder<ResourceAcquire>;

	ServiceMgr*			m_pServiceMgr = nullptr;
	ObjectTableType		m_acquire_table;
	spinlock			m_internal_lock;
	std::condition_variable_any m_condition_any;

	uint32_t m_timeout;
protected:
	ResourceAcquire();
	~ResourceAcquire();
public:
	bool SendQuerryResourceMessageSync(GameObject** address_pointer, UI_TYPE type, const char* resource_name);
	void SendQuerryResourceMessageAsync(GameObject** address_pointer, UI_TYPE type, const char* resouce_name);

	bool WaitResourceAcquire(std::thread::id thread_id = std::this_thread::get_id(), uint32_t timeout = 300/*miliseconds*/);
	void ResourceAcquired(const message_resource& msg_resource);

	void AddResourceMessageAsync(GameObject** address_pointer);
};

/*
Object: pointer to pointer object
Type:	enum for classification object
Asset:  unique resource name
*/
#define QuerryResourceAsync(ObjectPointer, Type , Asset) \
(ResourceAcquire::GetInstance()->SendQuerryResourceMessageAsync(reinterpret_cast<GameObject**>(&ObjectPointer), Type, Asset))

#define QuerryResourceSync(ObjectPointer, Type , Asset) \
(ResourceAcquire::GetInstance()->SendQuerryResourceMessage(reinterpret_cast<GameObject**>(&ObjectPointer), Type, Asset))

#define AddResourceAsync(ObjectPointer) \
(ResourceAcquire::GetInstance()->SendAddResourceMessageAsync(reinterpret_cast<GameObject**>(&ObjectPointer)))