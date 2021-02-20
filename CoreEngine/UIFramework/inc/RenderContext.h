#pragma once
#include <SDL.h>
#include "creator.h"
class ITextureRender
{
public:
	ITextureRender();
	virtual ~ITextureRender();
	virtual SDL_Texture* getSDLTexture() = 0;
};
typedef std::shared_ptr<ITextureRender> ITextureRenderPtr;

class IContext
{
public:
	IContext();
	virtual ~IContext();

	/**
	* The Context delegates some work to the Behavior object instead of
	* implementing +multiple versions of the algorithm on its own.
	*/
	virtual void excute(ITextureRenderPtr) = 0;
};
typedef std::shared_ptr<IContext> IContextPtr;

class RenderExContext : public IContext, public creator<RenderExContext>
{
public:
	RenderExContext(SDL_Renderer* pRenderer,
		const SDL_Rect&  srcrect,
		const SDL_Rect&  dstrect,
		double           angle,
		const SDL_Point& center,
		SDL_RendererFlip flip);
	virtual ~RenderExContext();
	friend class creator<RenderExContext>;

	SDL_Renderer* 	m_pRenderer = nullptr;
	SDL_Rect  		m_srcrect;
	SDL_Rect  		m_dstrect;
	double     		m_angle;
	SDL_Point	 	m_center;
	SDL_RendererFlip m_flip;

	void excute(ITextureRenderPtr) override;
};
typedef std::shared_ptr<RenderExContext> RenderExContextPtr;

class RenderContext : public IContext, public creator<RenderContext>
{
public:
	RenderContext(SDL_Renderer* pRenderer,
		const SDL_Rect& srcrect,
		const SDL_Rect& dstrect);
	virtual ~RenderContext();
	friend class creator<RenderContext>;

	SDL_Renderer* 	m_pRenderer = nullptr;
	SDL_Rect 		m_srcrect;
	SDL_Rect 		m_dstrect;


	void excute(ITextureRenderPtr) override;
};
typedef std::shared_ptr<RenderContext> RenderContextPtr;