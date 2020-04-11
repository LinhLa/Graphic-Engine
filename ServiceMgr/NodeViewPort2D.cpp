#include "stdafx.h"
#include <base_define.h>
#include <NodeViewPort2D.h>

NodeViewPort2D::NodeViewPort2D() {}

NodeViewPort2D::~NodeViewPort2D() {}

void NodeViewPort2D::onKeyInputEvent(KeyInputArgumentType &arg)
{
	//<define here
}

void NodeViewPort2D::onInit(RenderInitArgumentType &arg)
{
	//<load texture to render
	SDL_RenderSetViewport(m_pSceneConfig->GetRenderer(), &m_dest_rect);
}

void NodeViewPort2D::onDraw(RenderDrawArgumentType &arg)
{
	if (!m_bVisible)
	{
		return;
	}
	//Render texture to screen
	SDL_RenderCopy(m_pSceneConfig->GetRenderer(), m_texture.getSDLTexture(), NULL, NULL);
}

void NodeViewPort2D::onClean(RenderCleanArgumentType &arg)
{
	//<destroy texture
	m_texture.cleanUp();
}

void NodeViewPort2D::onLoop(RenderLoopArgumentType& arg) {}