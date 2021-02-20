#include "stdafx.h"
#include "RenderContext.h"
#include <exception>
ITextureRender::ITextureRender() {}
ITextureRender::~ITextureRender() {}

IContext::IContext() {}
IContext::~IContext() {}

RenderExContext::RenderExContext(SDL_Renderer* pRenderer,
	const SDL_Rect&  srcrect,
	const SDL_Rect&  dstrect,
	double           angle,
	const SDL_Point& center,
	SDL_RendererFlip flip) :
	m_pRenderer(pRenderer), m_srcrect(srcrect), m_dstrect(dstrect), m_angle(angle), m_center(center), m_flip(flip)
{
	if (!m_pRenderer)
	{
		throw std::exception("invalid argument");
	}
}

RenderExContext::~RenderExContext() {}

void RenderExContext::excute(ITextureRenderPtr pTextureRender)
{
	if (pTextureRender)
	{
		if (0 > SDL_RenderCopyEx(m_pRenderer,
			pTextureRender->getSDLTexture(),
			&m_srcrect,
			&m_dstrect,
			m_angle,
			&m_center,
			m_flip))
		{
			SDL_Log("SDL_RenderCopyEx failed: %s\n", SDL_GetError());
			_ASSERT(false);
		}
	}
}

RenderContext::RenderContext(SDL_Renderer* pRenderer,
	const SDL_Rect& srcrect,
	const SDL_Rect& dstrect) :
	m_pRenderer(pRenderer), m_srcrect(srcrect), m_dstrect(dstrect)
{
	if (!m_pRenderer)
	{
		throw std::exception("invalid argument");
	}
}
RenderContext::~RenderContext() {}

void RenderContext::excute(ITextureRenderPtr pTextureRender)
{
	if (pTextureRender)
	{
		if (0 > SDL_RenderCopy(m_pRenderer,
			pTextureRender->getSDLTexture(),
			&m_srcrect,
			&m_dstrect))
		{
			SDL_Log("SDL_RenderCopy failed: %s\n", SDL_GetError());
			_ASSERT(false);
		}
	}
}
