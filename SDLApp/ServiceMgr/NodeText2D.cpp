#include "stdafx.h"
#include <NodeText2D.h>
#include <SceneConfig.h>

NodeText2D::NodeText2D(){}

NodeText2D::~NodeText2D(){}

void NodeText2D::onKeyInputEvent(KeyInputArgumentType &arg)
{
}

void NodeText2D::onInit(RenderInitArgumentType &arg)
{
	//load a font into memory:
	m_font = TTF_OpenFont(m_font_name.c_str(), m_point_size);
	_ASSERT(m_font);
}

void NodeText2D::onDraw(RenderDrawArgumentType &arg)
{
	if (!m_bVisible)
	{
		return;
	}

	//<load font to texture
	m_FontTexture.loadFont(m_font, m_text.c_str(), m_fore_ground_color, m_pSceneConfig->GetRenderer());

	//<load texture to render
	SDL_RenderCopyEx(m_pSceneConfig->GetRenderer(), m_FontTexture.GetFontTexture(), &m_src_rect, &m_dest_rect, m_angle, &m_centerPoint, m_flip);
}

void NodeText2D::onClean(RenderCleanArgumentType &arg)
{
	TTF_CloseFont(m_font);
	m_font = nullptr;
}

void NodeText2D::onLoop(RenderLoopArgumentType& arg) {}