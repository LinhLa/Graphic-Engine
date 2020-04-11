#pragma once
#include <SDL.h>
#include <memmgr.h>
#include <SDL_ttf.h>
class Font
{
private:
	int m_width = 0;
	int m_height = 0;
	SDL_Texture*	m_pFontTexture = nullptr;
public:
	Font();
	~Font();

	bool loadFont(TTF_Font*, const char*, SDL_Color, SDL_Renderer*);
	SDL_Texture* GetFontTexture() const;
	void cleanUp();

	FREE_LIST_MEMORY_MANAGER(Font)
};

typedef std::shared_ptr<Font> FontPtr;