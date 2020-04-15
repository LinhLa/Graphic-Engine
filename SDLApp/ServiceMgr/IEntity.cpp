#include "stdafx.h"
#include <IEntity.h>
#include <Surface.h>

IEntity::IEntity() {}

IEntity::~IEntity() {}

void IEntity::loadTexture(const char* path, uint32_t MaxFrames, SDL_Renderer* pRenderer)
{
	_ASSERT(path && pRenderer);
	m_EntityTexture.loadImage(path, pRenderer);

	m_EntityTexture.Transparent({255,0,255}, pRenderer);

	this->setSourceWidth(m_EntityTexture.GetWidth());
	this->setSourceHeight(m_EntityTexture.GetHeight());

	m_AnimationControl.SetMaxFrames(MaxFrames);
}


void IEntity::onKeyInputEvent(KeyInputArgumentType &arg)
{
	//<define here
}

void IEntity::onInit(RenderInitArgumentType &arg)
{
	//<create resource path
	std::string resource_path = m_pSceneConfig->GetAssetPath() + m_resource_name;

	//<load texture
	m_EntityTexture.loadImage(resource_path.c_str(), m_pSceneConfig->GetRenderer());
}


void IEntity::onDraw(RenderDrawArgumentType &arg)
{
	//<TODO:
}

void IEntity::onClean(RenderCleanArgumentType &arg)
{
	//<destroy texture
	m_EntityTexture.cleanUp();
}

void IEntity::onLoop(RenderLoopArgumentType &arg)
{
	m_AnimationControl.onAnimate();
}