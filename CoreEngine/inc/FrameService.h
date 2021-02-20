#pragma once
#include "SingletonHolder.h"
#include "base_define.h"
class FrameService : public IService
{
private:
	friend class ServiceMgr;

	void FrameReport();
public:
	FrameService();
	~FrameService();

	void onStartService() override;
	void onEndService() override;
	void onMessageReceive(MessagePtr) override;
};


typedef std::shared_ptr<FrameService> FrameServicePtr;