#include "stdafx.h"
#include "RenderManipulator.h"
#include "TextureManipulator.h"
#include "UIHelper.h"

#define IS_VALID_RECT(rect)		(0 != rect.x && 0 != rect.y && 0 != rect.h && 0 != rect.w)

RenderClipManipulator::RenderClipManipulator(SDL_Renderer* pRenderer, const SDL_Rect& target):m_clip_target(target), m_pRenderer(pRenderer)
{
	if (!m_pRenderer)
	{
		throw std::logic_error("invalid argument");
	}
	SDL_RenderGetClipRect(m_pRenderer, &m_render_area);
	if (SDL_FALSE == SDL_RenderIsClipEnabled(m_pRenderer) || !IS_VALID_RECT(m_render_area))
	{
		m_render_area = UIHelper::GetWindowRect();
	}
}

RenderClipManipulator::~RenderClipManipulator()
{
	if (m_bSetClipTarget)
	{
		SDL_RenderSetClipRect(m_pRenderer, &m_render_area);
	}
}

bool RenderClipManipulator::HasIntersection()
{
	return (SDL_TRUE == SDL_HasIntersection(&m_clip_target, &m_render_area));
}

bool RenderClipManipulator::GetIntersection(SDL_Rect &sdlResult)
{
	return (SDL_TRUE == SDL_IntersectRect(&m_clip_target, &m_render_area, &sdlResult));
}

void RenderClipManipulator::SetRenderClipTarget()
{
	if (!m_bSetClipTarget)
	{
		SDL_RenderSetClipRect(m_pRenderer, &m_clip_target);
	}
	m_bSetClipTarget = true;
}

RenderDrawManipulator::RenderDrawManipulator(SDL_Renderer* pRenderer, SDL_BlendMode mode, SDL_Color color) :m_pRenderer(pRenderer)
{
	if (!m_pRenderer)
	{
		throw std::logic_error("invalid argument");
	}
	SDL_GetRenderDrawColor(m_pRenderer, &m_RenderColor.r, &m_RenderColor.g, &m_RenderColor.b, &m_RenderColor.a);
	SDL_GetRenderDrawBlendMode(m_pRenderer, &m_RenderBlendMode);
	SetBlendMode(mode);
	SetDrawColor(color);
}

RenderDrawManipulator::~RenderDrawManipulator()
{
	SDL_SetRenderDrawBlendMode(m_pRenderer, m_RenderBlendMode);
	SDL_SetRenderDrawColor(m_pRenderer, m_RenderColor.r, m_RenderColor.g, m_RenderColor.b, m_RenderColor.a);
}

int RenderDrawManipulator::SetDrawColor(SDL_Color color)
{
	int iResult = SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	if (0 != iResult)
	{
		SDL_Log("SDL_SetRenderDrawColor() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return iResult;
}

int RenderDrawManipulator::SetBlendMode(SDL_BlendMode mode)
{
	int iResult = SDL_SetRenderDrawBlendMode(m_pRenderer, mode);
	if (0 != iResult)
	{
		SDL_Log("SDL_SetRenderDrawBlendMode() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return iResult;
}

int RenderDrawManipulator::FillRect(SDL_Rect rect)
{
	int iResult = SDL_RenderFillRect(m_pRenderer, &rect);
	if (0 != iResult)
	{
		SDL_Log("SDL_RenderFillRect() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return iResult;
}

RenderTargetManipulator::RenderTargetManipulator(SDL_Renderer* pRenderer, SDL_Texture* pTarget):m_pRenderer(pRenderer), m_pTarget(pTarget)
{
	if (nullptr == m_pRenderer || nullptr == m_pTarget)
	{
		throw std::logic_error("invalid argument");
	}
	m_pRenderTarget = SDL_GetRenderTarget(m_pRenderer);
	SDL_SetRenderTarget(m_pRenderer, m_pTarget);
}

RenderTargetManipulator::~RenderTargetManipulator()
{
	SDL_SetRenderTarget(m_pRenderer, m_pRenderTarget);
}

int RenderTargetManipulator::RenderCopy(SDL_Texture* pTexture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
	return SDL_RenderCopy(m_pRenderer, pTexture, srcrect, dstrect);
}

RenderScaleManipulator::RenderScaleManipulator(SDL_Renderer* pRenderer, float scaleX, float scaleY):m_pRenderer(pRenderer)
{
	if (nullptr == m_pRenderer)
	{
		throw std::logic_error("invalid argument");
	}
	SDL_RenderGetScale(m_pRenderer, &m_scaleX, &m_scaleY);

	if (0 > SDL_RenderSetScale(m_pRenderer, scaleX, scaleY))
	{
		SDL_Log("SDL_RenderSetScale() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
}

RenderScaleManipulator::~RenderScaleManipulator()
{
	if (0 > SDL_RenderSetScale(m_pRenderer, m_scaleX, m_scaleY))
	{
		SDL_Log("SDL_RenderSetScale() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
}