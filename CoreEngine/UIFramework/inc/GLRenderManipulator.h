#pragma once
#include <SDL.h>
#include "GLFrameBufferObject.h"
#include <string>
/**
 * @brief      This class describes a render clip manipulator.
 */
class GLRenderClipManipulator
{
private:
	SDL_Rect m_clip_target;
	SDL_Rect m_render_area;
	bool	 m_bSetClipTarget = false;
public:
	GLRenderClipManipulator(const SDL_Rect& target);
	~GLRenderClipManipulator();

	bool HasIntersection();
	bool GetIntersection(SDL_Rect&);

	void SetRenderClipTarget();
};

/**
 * @brief      This class describes a render draw manipulator.
 */
//class GLRenderDrawManipulator
//{
//private:
//	SDL_BlendMode m_RenderBlendMode = SDL_BLENDMODE_NONE;
//	SDL_Color	  m_RenderColor;
//public:
//	GLRenderDrawManipulator(SDL_BlendMode mode = SDL_BLENDMODE_BLEND, SDL_Color color = { 0,0,0,255 });
//	~GLRenderDrawManipulator();
//
//	int SetDrawColor(SDL_Color);
//	int SetBlendMode(SDL_BlendMode);
//
//	int FillRect(SDL_Rect);
//};