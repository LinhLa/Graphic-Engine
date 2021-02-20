#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include "creator.h"

class Surface;
typedef std::shared_ptr<Surface> SurfacePtr;

class Surface:public creator<Surface>
{
private:
	SDL_Surface*	m_pSurface = nullptr;
	void cleanUp();

	Surface(const char*);
	Surface(TTF_Font *, const char *, SDL_Color color);
	Surface(void*, size_t);
	Surface(SurfacePtr, float scale = 1.0F);

	int BlitSurface(SDL_Surface*, float scale = 1.0F);
	void ConvertToFormat(SDL_Surface*, const SDL_PixelFormat*);

public:
	~Surface();
	friend class creator<Surface>;

	int SetBlendMode(SDL_BlendMode);
	int GetBlendMode(SDL_BlendMode*);

	void CreateTextureFromSurface(SDL_Renderer*, SDL_Texture**);

	int Width() const;
	int Height() const;
	int BytePerPixel() const;
	uint32_t PixelFormat() const;
};