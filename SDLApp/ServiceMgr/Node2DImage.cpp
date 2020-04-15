#include "stdafx.h"
#include <base_define.h>
#include <Node2DImage.h>

Node2DImage::Node2DImage(){}

Node2DImage::~Node2DImage() {}

void Node2DImage::onKeyInputEvent(KeyInputArgumentType &arg)
{
	//<define here
}

void Node2DImage::onInit(RenderInitArgumentType &arg)
{
	//<create resource path
	std::string resource_path = m_pSceneConfig->GetAssetPath() + m_resource_name;

	//<load texture
	m_texture_background.loadImage(resource_path.c_str(), m_pSceneConfig->GetRenderer());
}

void Node2DImage::onDraw(RenderDrawArgumentType &arg)
{
	if (!m_bVisible)
	{
		return;
	}
	//<load texture to render
	SDL_RenderCopyEx(m_pSceneConfig->GetRenderer(), m_texture_background.getSDLTexture(), &m_src_rect, &m_dest_rect, m_angle, &m_centerPoint, m_flip);
}

void Node2DImage::onClean(RenderCleanArgumentType &arg)
{
	//<destroy texture
	m_texture_background.cleanUp();
}

void Node2DImage::onLoop(RenderLoopArgumentType& arg) {}