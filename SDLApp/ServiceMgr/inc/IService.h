#pragma once
#include <atomic>
#include <base_define.h>
class ServiceMgr;
class IService
{
protected:
	static ServiceMgr* m_pServiceMgr;
public:
	IService();
	virtual ~IService();
	virtual void onStartService() = 0;
	virtual void onEndService() = 0;
	virtual void onMessageReceive(const Message&) = 0;
};