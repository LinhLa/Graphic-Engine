#include "stdafx.h"
#include <base_define.h>
#include <NodeButton2D.h>

NodeButton2D::NodeButton2D(){}

NodeButton2D::~NodeButton2D() {}

void NodeButton2D::onKeyInputEvent(KeyInputArgumentType &arg)
{
	//<define here
}

void NodeButton2D::onInit(RenderInitArgumentType &arg)
{
	//<create resource path
	std::string resource_path = m_pSceneConfig->GetAssetPath() + m_resource_name;

	//<Init backgournd texture and text texture
	m_pBackground_texture->onInit(arg);
	m_pText_texture->onInit(arg);
}

void NodeButton2D::onDraw(RenderDrawArgumentType &arg)
{
	if (!m_bVisible)
	{
		return;
	}
	//<draw backgournd texture 1st and text texture 2nd
	m_pBackground_texture->onDraw(arg);
	m_pText_texture->onDraw(arg);
}

void NodeButton2D::onClean(RenderCleanArgumentType &arg)
{
	//<clean backgournd texture and text texture
	m_pBackground_texture->onClean(arg);
	m_pText_texture->onClean(arg);
}

void NodeButton2D::onLoop(RenderLoopArgumentType& arg) {}