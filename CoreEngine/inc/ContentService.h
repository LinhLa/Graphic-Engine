#pragma once
#include <base_define.h>
#include <IService.h>

class ContentService : public IService
{
public:
	ContentService();
	~ContentService();

	void onStartService() override;
	void onEndService() override;
	void onMessageReceive(MessagePtr) override;
};