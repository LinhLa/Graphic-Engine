#include "stdafx.h"
#include "GameObject.h"
#include <SceneConfig.h>
#include <ServiceMgr.h>

#include <Node2DImage.h>
#include <NodeButton2D.h>
#include <NodeText2D.h>
#include <NodeViewPort2D.h>

const SceneConfig* GameObject::m_pSceneConfig = SceneConfig::GetInstance();

GameObject::GameObject(){}


GameObject::~GameObject()
{
	onClean(RenderCleanArgumentType());
}

void GameObject::onInit(RenderInitArgumentType&)
{
	//< do nothing
}

void GameObject::onDraw(RenderDrawArgumentType&)
{
	//< do nothing
}

void GameObject::onClean(RenderCleanArgumentType&)
{
	//< do nothing
}

void GameObject::onKeyInputEvent(KeyInputArgumentType&)
{
	//< do nothing
}

void GameObject::onLoop(RenderLoopArgumentType&)
{
	//< do nothing
}