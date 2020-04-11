#pragma once
#include "SingletonHolder.h"
#include "base_define.h"
#include "type_define.h"
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
	void onMessageReceive(const Message&) override;
};


typedef std::shared_ptr<FrameService> FrameServicePtr;