#include "stdafx.h"
#include <ContentService.h>
#include <ServiceMgr.h>
#include <ResourceAcquire.h>

typedef function_member<ContentService, RenderDrawArgumentType> DrawCallbackType;
typedef function_member<ContentService, KeyInputArgumentType> KeyInputCallbackType;
ContentService::ContentService() :m_pRabbit(new Rabbit)
{
	//Allocate function callback object
	m_pRenderCallback.reset(new DrawCallbackType(this, &ContentService::onDraw));
	m_pKeyInputCallback.reset(new KeyInputCallbackType(this, &ContentService::onKeyInputEvent));
}

ContentService::~ContentService()
{
	//<Remove trigger event to service manager
}

void ContentService::onStartService()
{
	m_pServiceMgr->registerOnEvent(this, SCENE_LOAD_RESOURCE_DONE);

	//<Add trigger event to render service
	RENDER_REGISTER_CALLBACK(m_pServiceMgr, m_pRenderCallback.get(), RENDER_EVENT_DRAW);
	RENDER_REGISTER_CALLBACK(m_pServiceMgr, m_pKeyInputCallback.get(), RENDER_EVENT_KEY_INPUT);
}

void ContentService::onEndService()
{
	m_pServiceMgr->unregisterOnEvent(this, SCENE_LOAD_RESOURCE_DONE);

	//<Remove trigger event to render service
	RENDER_UNREGISTER_CALLBACK(m_pServiceMgr, m_pRenderCallback.get(), RENDER_EVENT_DRAW);
	RENDER_UNREGISTER_CALLBACK(m_pServiceMgr, m_pKeyInputCallback.get(), RENDER_EVENT_KEY_INPUT);
}

void ContentService::onKeyInputEvent(KeyInputArgumentType &arg)
{
	//< Add key input handler to node
	//m_pBackGround->addMessageHandler(this, &ContentService::onKeyInputEvent);
	//m_pSky->addMessageHandler(this, &ContentService::onKeyInputEvent);
	m_pRabbit->onKeyInputEvent(arg);
}

void ContentService::AcquireResource()
{

	m_pRabbit->AcquireResource();
	QuerryResourceAsync(m_pBackGround, NODE_2D_IMAGE_TYPE, GROUND_ASSET);
	QuerryResourceAsync(m_pSky, NODE_2D_IMAGE_TYPE, SKY_ASSET);

	_ASSERT(ResourceAcquire::GetInstance()->WaitResourceAcquire());

	m_pRabbit->setVisible(true);
	m_pBackGround->setVisible(true);
	m_pSky->setVisible(true);
}

void ContentService::onDraw(RenderDrawArgumentType &arg)
{

}

void ContentService::onMessageReceive(const Message &msg)
{
	switch (msg.m_id)
	{
	case SCENE_LOAD_RESOURCE_DONE:
		m_pServiceMgr->addBackgroundTask(std::bind(&ContentService::AcquireResource, this));
		break;
	default:
		break;
	}
}