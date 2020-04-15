#pragma once
#include "IService.h"
#include "base_define.h"
#include "type_define.h"
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
	void onMessageReceive(const Message&) override;
};

typedef std::shared_ptr<LogService> LogServicePtr;