#pragma once
#include "ServiceMgr.h"
#include "type_define.h"

class TestService : public IService
{
public:
	TestService();
	~TestService();
	void process();
	virtual void onStartService() override;
	virtual void onEndService() override;
	virtual void onMessageReceive(const Message&) override;
};

typedef std::shared_ptr<TestService> TestServicePtr;