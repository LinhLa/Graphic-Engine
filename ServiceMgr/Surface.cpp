#include "stdafx.h"
#include <Surface.h>
#include <SDL_image.h>

Surface::Surface()
{
}

Surface::~Surface()
{
	cleanUp();
}

//<Loads media from image
bool Surface::loadImage(const char* path, SDL_Renderer*)
{
	/* Open the image file */
	m_pSurface = IMG_Load(path);
	if (m_pSurface)
	{
		SDL_Log("Couldn't load %s: %s\n", path, SDL_GetError());
		_ASSERT(false);
	}

	return true;
}

bool Surface::draw(SDL_Surface* Surf_Src, SDL_Rect dest)
{
	_ASSERT(nullptr != Surf_Src);
	SDL_BlitSurface(Surf_Src, NULL, m_pSurface, &dest);
	return true;
}

bool Surface::draw(SDL_Surface* Surf_Src, SDL_Rect dest, SDL_Rect src)
{
	_ASSERT(nullptr != Surf_Src);
	SDL_BlitSurface(Surf_Src, &src, m_pSurface, &dest);
	return true;
}

SDL_Surface* Surface::getSDLSurface() const
{
	return m_pSurface;
}

void Surface::cleanUp()
{
	if (m_pSurface)
	{
		m_pSurface = nullptr;
	}
}
