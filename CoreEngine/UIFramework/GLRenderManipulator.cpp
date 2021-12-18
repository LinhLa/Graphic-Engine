#include "stdafx.h"
#include "GLRenderManipulator.h"
#include "UIHelper.h"

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "log.h"
#include "Renderer3D.h"

#include <stdexcept>

#define IS_VALID_RECT(rect)		(0 != rect.x && 0 != rect.y && 0 != rect.h && 0 != rect.w)
#define INT2FLOAT(x) (static_cast<float>(x))
#define FLOAT2INT(x) (static_cast<int>(x))
#define SDL_INT2FLOAT(r) (SDL_FRect{INT2FLOAT(r.x), INT2FLOAT(r.y), INT2FLOAT(r.w), INT2FLOAT(r.h)})
#define SDL_FLOAT2INT(r) (SDL_Rect{FLOAT2INT(r.x), FLOAT2INT(r.y), FLOAT2INT(r.w), FLOAT2INT(r.h)})

GLRenderClipManipulator::GLRenderClipManipulator(const SDL_Rect& target): m_clip_target(target)
{
	GLfloat fbox[4];
	glGetFloatv(GL_SCISSOR_BOX, fbox);
	m_render_area.x = FLOAT2INT(fbox[0]);
	m_render_area.y = FLOAT2INT(fbox[1]);
	m_render_area.w = FLOAT2INT(fbox[2]);
	m_render_area.h = FLOAT2INT(fbox[3]);
	if (!IS_VALID_RECT(m_render_area))
	{
		m_render_area = UIHelper::GetWindowRect();
	}
}

GLRenderClipManipulator::~GLRenderClipManipulator()
{
	if (m_bSetClipTarget)
	{
		glScissor(m_render_area.x, m_render_area.y, m_render_area.w, m_render_area.h);
	}
}

bool GLRenderClipManipulator::HasIntersection()
{
	return (SDL_TRUE == SDL_HasIntersection(&m_clip_target, &m_render_area));
}

bool GLRenderClipManipulator::GetIntersection(SDL_Rect &sdlResult)
{
	return (SDL_TRUE == SDL_IntersectRect(&m_clip_target, &m_render_area, &sdlResult));
}

void GLRenderClipManipulator::SetRenderClipTarget()
{
	if (!m_bSetClipTarget)
	{
		glScissor(m_clip_target.x, m_clip_target.y, m_clip_target.w, m_clip_target.h);
	}
	m_bSetClipTarget = true;
}
//
//GLRenderDrawManipulator::GLRenderDrawManipulator(SDL_BlendMode mode, SDL_Color color)
//{
//	SDL_GetRenderDrawColor(m_pRenderer, &m_RenderColor.r, &m_RenderColor.g, &m_RenderColor.b, &m_RenderColor.a);
//	SDL_GetRenderDrawBlendMode(m_pRenderer, &m_RenderBlendMode);
//	SetBlendMode(mode);
//	SetDrawColor(color);
//}
//
//GLRenderDrawManipulator::~GLRenderDrawManipulator()
//{
//	SDL_SetRenderDrawBlendMode(m_pRenderer, m_RenderBlendMode);
//	SDL_SetRenderDrawColor(m_pRenderer, m_RenderColor.r, m_RenderColor.g, m_RenderColor.b, m_RenderColor.a);
//}
//
//int GLRenderDrawManipulator::SetDrawColor(SDL_Color color)
//{
//	int iResult = SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
//	if (0 != iResult)
//	{
//		SDL_Log("SDL_SetRenderDrawColor() failed: %s\n", SDL_GetError());
//		_ASSERT(false);
//	}
//	return iResult;
//}
//
//int GLRenderDrawManipulator::SetBlendMode(SDL_BlendMode mode)
//{
//	int iResult = SDL_SetRenderDrawBlendMode(m_pRenderer, mode);
//	if (0 != iResult)
//	{
//		SDL_Log("SDL_SetRenderDrawBlendMode() failed: %s\n", SDL_GetError());
//		_ASSERT(false);
//	}
//	return iResult;
//}
//
//int GLRenderDrawManipulator::FillRect(SDL_Rect rect)
//{
//	int iResult = SDL_RenderFillRect(m_pRenderer, &rect);
//	if (0 != iResult)
//	{
//		SDL_Log("SDL_RenderFillRect() failed: %s\n", SDL_GetError());
//		_ASSERT(false);
//	}
//	return iResult;
//}
//