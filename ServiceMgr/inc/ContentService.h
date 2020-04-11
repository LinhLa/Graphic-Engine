#pragma once
#include <base_define.h>
#include <type_define.h>
#include <IArea.h>
#include <Rabbit.h>

class ContentService : public IService
{
private:

	friend class ServiceMgr;

	uint8_t			m_current_stage = STAGE_1;

	Node2DImage*	m_pBackGround = nullptr;
	Node2DImage*	m_pSky = nullptr;
	RabbitPtr		m_pRabbit = nullptr;

	std::shared_ptr<function_base>  m_pRenderCallback = nullptr;
	std::shared_ptr<function_base>  m_pKeyInputCallback = nullptr;

	void onKeyInputEvent(KeyInputArgumentType &arg);

	void AcquireResource();
	void onDraw(RenderDrawArgumentType &arg);

public:
	ContentService();
	~ContentService();

	virtual void onStartService() override;
	virtual void onEndService() override;
	virtual void onMessageReceive(const Message&) override;
};

typedef std::shared_ptr<ContentService> ContentServicePtr;