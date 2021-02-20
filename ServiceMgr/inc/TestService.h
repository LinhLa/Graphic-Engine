#pragma once
#include "ServiceMgr.h"

class TestService : public IService
{
public:
	TestService();
	~TestService();
	void process();
	void onStartService() override;
	void onEndService() override;
	void onMessageReceive(MessagePtr) override;
};

typedef std::shared_ptr<TestService> TestServicePtr;