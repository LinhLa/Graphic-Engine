#include "stdafx.h"
#include <SDL.h>
#include "UIHelper.h"
#include "Configuration.h"
#include "Render.h"

namespace UIHelper
{
	MOUSE_STATE onMouseEvent(const SDL_Event& event, const SDL_Rect& display_rect)
	{
		MOUSE_STATE currentState = MOUSE_OUT;
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Apply render scale to mouse position
		x = static_cast<int>(static_cast<float>(x) / WindowRender::GetInstance()->scaleX());
		y = static_cast<int>(static_cast<float>(y) / WindowRender::GetInstance()->scaleY());

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < display_rect.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > (display_rect.x + display_rect.w))
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < display_rect.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > (display_rect.y + display_rect.h))
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			currentState = MOUSE_OUT;
		}
		else
		{
			//Mouse is inside button
			switch (event.type)
			{
				case SDL_MOUSEMOTION:
				currentState = MOUSE_OVER_MOTION;
				break;
				case SDL_MOUSEBUTTONDOWN:
				currentState = MOUSE_DOWN;
				break;
				case SDL_MOUSEBUTTONUP:
				currentState = MOUSE_UP;
				break;
				default:
				break;
			}
		}
		return currentState;
	}

	SDL_Rect GetScaleRect(int x, int y, int w, int h, float scale_x, float scale_y)
	{
		SDL_Rect sdlResult{0,0,0,0};
		sdlResult.x = x + static_cast<int>((w - (w * scale_x)) / 2);
		sdlResult.y = y + static_cast<int>((h - (h * scale_y)) / 2);

		sdlResult.w = static_cast<int>(w * scale_x);
		sdlResult.h = static_cast<int>(h * scale_y);
		return sdlResult;
	}

	std::string GetAssetPath()
	{
		return Configuration::GetInstance()->asset_path;
	}

	SDL_Renderer* GetRenderer()
	{
		return Render::GetInstance()->getRenderer();
	}

	uint32_t WindowPixelFormat()
	{
		return Render::GetInstance()->WindowPixelFormat();
	}

	SDL_Rect GetWindowRect()
	{
		Configuration *pConfig = Configuration::GetInstance();
		return SDL_Rect{ 0, 0, pConfig->height, pConfig->width };
	}

	const SDL_PixelFormat* WindowSurfaceFormat()
	{
		return Render::GetInstance()->WindowSurfaceFormat();
	}
}