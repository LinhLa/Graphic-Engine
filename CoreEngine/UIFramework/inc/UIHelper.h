#pragma once
#include <SDL.h>
#include <string>
#include "Surface.h"

namespace UIHelper
{
	enum MOUSE_STATE: uint8_t
	{
		MOUSE_OUT,
		MOUSE_OVER_MOTION,
		MOUSE_DOWN,
		MOUSE_UP,
	};

	MOUSE_STATE onMouseEvent(const SDL_Event& event, const SDL_Rect& display_rect);
	SDL_Rect GetScaleRect(int x, int y, int w, int h, float scale_x, float scale_y);
	std::string GetAssetPath();
	SDL_Renderer* GetRenderer();
	uint32_t WindowPixelFormat();
	const SDL_PixelFormat* WindowSurfaceFormat();
	SDL_Rect GetWindowRect();
}