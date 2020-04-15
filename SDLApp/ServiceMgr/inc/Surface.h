#pragma once
#include <memmgr.h>
#include <SDL.h>
class Surface
{
private:
	SDL_Surface*	m_pSurface = nullptr;
public:
	Surface();
	~Surface();

	bool draw(SDL_Surface* Surf_Src, SDL_Rect dest);
	bool draw(SDL_Surface* Surf_Src, SDL_Rect dest, SDL_Rect src);

	bool loadImage(const char* path, SDL_Renderer*);

	SDL_Surface* getSDLSurface() const;

	void cleanUp();

	FREE_LIST_MEMORY_MANAGER(Surface)
};