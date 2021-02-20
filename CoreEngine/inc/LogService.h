#pragma once
#include "IService.h"
#include "SingletonHolder.h"

class LogService : public IService, public SingletonHolder<LogService>
{
private:
	friend class ServiceMgr;
	friend class SingletonHolder<LogService>;
public:
	LogService();
	~LogService();

	void onStartService() override;
	void onEndService() override;
	void onMessageReceive(MessagePtr) override;
};