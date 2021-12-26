#include "stdafx.h"
#include "GLRenderManipulator.h"
#include "UIHelper.h"

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "log.h"
#include "WindowRender.h"
#include <stdexcept>

#define IS_VALID_RECT(rect)		(0 != rect.x && 0 != rect.y && 0 != rect.h && 0 != rect.w)
#define INT2FLOAT(x) (static_cast<float>(x))
#define FLOAT2INT(x) (static_cast<int>(x))
#define SDL_INT2FLOAT(r) (SDL_FRect{INT2FLOAT(r.x), INT2FLOAT(r.y), INT2FLOAT(r.w), INT2FLOAT(r.h)})
#define SDL_FLOAT2INT(r) (SDL_Rect{FLOAT2INT(r.x), FLOAT2INT(r.y), FLOAT2INT(r.w), FLOAT2INT(r.h)})

static glm::vec4 flipY(glm::vec4 box)
{
	auto screenHeight = static_cast<float>(WindowRender::GetInstance()->getHeight());
	auto windowHeight = box[3];
	box.y = screenHeight - windowHeight - box.y;
	return box;
}

static bool isValidRect(glm::vec4 box)
{
	return (0.0f != box[2] || 0.0f != box[3]);
}

static glm::vec4 toGLMVEC4(SDL_Rect rect)
{
	return glm::vec4(
		rect.x,
		rect.y,
		rect.w,
		rect.h
	);
}

static SDL_Rect toSDLRECT(glm::vec4 box)
{
	return {FLOAT2INT(box[0]), FLOAT2INT(box[1]), FLOAT2INT(box[2]), FLOAT2INT(box[3])};
}

GLRenderClipManipulator::GLRenderClipManipulator(const glm::vec4& target): m_clip_target(target)
{
	glGetFloatv(GL_SCISSOR_BOX, glm::value_ptr(m_render_area));
	m_render_area = flipY(m_render_area);
	if (!isValidRect(m_render_area))
	{
		m_render_area = toGLMVEC4(UIHelper::GetWindowRect());
	}
}

GLRenderClipManipulator::~GLRenderClipManipulator()
{
	if (m_bSetClipTarget)
	{
		auto box = flipY(m_render_area);
		glScissor(box[0], box[1], box[2], box[3]);
	}
}

bool GLRenderClipManipulator::HasIntersection()
{
	auto clip_target = toSDLRECT(m_clip_target);
	auto render_area = toSDLRECT(m_render_area);
	return (SDL_TRUE == SDL_HasIntersection(&clip_target, &render_area));
}

bool GLRenderClipManipulator::GetIntersection(glm::vec4&result)
{
	auto clip_target = toSDLRECT(m_clip_target);
	auto render_area = toSDLRECT(m_render_area);
	SDL_Rect sdlResult;
	auto ret = SDL_IntersectRect(&clip_target, &render_area, &sdlResult);
	result = toGLMVEC4(sdlResult);
	return ret;
}

void GLRenderClipManipulator::SetRenderClipTarget()
{
	if (!m_bSetClipTarget)
	{
		auto box = flipY(m_clip_target);
		glScissor(box[0], box[1], box[2], box[3]);
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