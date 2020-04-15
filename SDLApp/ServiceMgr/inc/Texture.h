#pragma once
#include <SDL.h>
#include <memmgr.h>
#include <SDL_ttf.h>
class Texture
{
private:
	int m_width = 0;
	int m_height = 0;
	SDL_Texture*	m_pTexture = nullptr;
public:
	Texture();
	~Texture();

	bool loadImage(const char* path, SDL_Renderer*);
	Texture& CreateTextureFromSurface(SDL_Surface* pSurface, SDL_Renderer* pRenderer);
	Texture& operator=(SDL_Texture*);
	SDL_Texture* getSDLTexture();
	void Transparent(const SDL_Color &color, SDL_Renderer* pRenderer);
	void cleanUp();

	int GetWidth() const;
	int GetHeight() const;

	FREE_LIST_MEMORY_MANAGER(Texture)
};

typedef std::shared_ptr<Texture> TexturePtr;