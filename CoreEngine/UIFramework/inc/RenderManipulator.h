#pragma once
#include <SDL.h>
#include <string>

/**
 * @brief      This class describes a render clip manipulator.
 */
class RenderClipManipulator
{
private:
	SDL_Renderer* m_pRenderer = nullptr;
	SDL_Rect m_clip_target;
	SDL_Rect m_render_area;
	bool	 m_bSetClipTarget = false;
public:
	RenderClipManipulator(SDL_Renderer* pRenderer, const SDL_Rect& target);
	~RenderClipManipulator();

	bool HasIntersection();
	bool GetIntersection(SDL_Rect&);

	void SetRenderClipTarget();
};

/**
 * @brief      This class describes a render draw manipulator.
 */
class RenderDrawManipulator
{
private:
	SDL_Renderer* m_pRenderer = nullptr;
	SDL_BlendMode m_RenderBlendMode = SDL_BLENDMODE_NONE;
	SDL_Color	  m_RenderColor;
public:
	RenderDrawManipulator(SDL_Renderer* pRenderer = nullptr, SDL_BlendMode mode = SDL_BLENDMODE_BLEND, SDL_Color color = { 0,0,0,255 });
	~RenderDrawManipulator();

	int SetDrawColor(SDL_Color);
	int SetBlendMode(SDL_BlendMode);

	int FillRect(SDL_Rect);
};

/**
 * @brief      This class describes a render target manipulator.
 */
class RenderTargetManipulator
{
private:
	SDL_Renderer* m_pRenderer = nullptr;
	SDL_Texture* m_pRenderTarget = nullptr;
	SDL_Texture* m_pTarget = nullptr;
public:
	RenderTargetManipulator(SDL_Renderer* pRenderer = nullptr, SDL_Texture* pTarget = nullptr);
	~RenderTargetManipulator();

	int RenderCopy(SDL_Texture* pTexture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
};

/**
 * @brief      This class describes a render scale manipulator for different resolution
 */
class RenderScaleManipulator
{
private:
	SDL_Renderer* m_pRenderer = nullptr;
	float        m_scaleX = 1.0F;
    float        m_scaleY = 1.0F;
public:
	RenderScaleManipulator(SDL_Renderer* pRenderer = nullptr, float scaleX = 1.0F, float scaleY = 1.0F);
	~RenderScaleManipulator();
};