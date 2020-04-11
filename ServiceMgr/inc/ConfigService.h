#pragma once
#include "IService.h"
#include "base_define.h"
#include "type_define.h"
#include "SingletonHolder.h"

class ConfigService : public IService, public SingletonHolder<ConfigService>
{
private:
	friend class ServiceMgr;
	friend class SingletonHolder<ConfigService>;

	configuration m_config;

	void loadConfiguration();
public:
	ConfigService();
	~ConfigService();

	virtual void onStartService() override;
	virtual void onEndService() override;
	virtual void onMessageReceive(const Message&) override;

};

typedef std::shared_ptr<ConfigService> ConfigServicePtr;