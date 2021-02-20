#pragma once
#include "message_struct.h"
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
	virtual void onMessageReceive(MessagePtr) = 0;
};