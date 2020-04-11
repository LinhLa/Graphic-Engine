#include "stdafx.h"
#include "Font.h"
#include <SDL_image.h>

Font::Font()
{
}

Font::~Font()
{
	cleanUp();
}

bool Font::loadFont(TTF_Font* font, const char* text, SDL_Color fore_ground_color, SDL_Renderer* pRenderer)
{
	SDL_Surface* pSurface = TTF_RenderText_Solid(font, text, fore_ground_color);
	_ASSERT(pSurface);

	m_pFontTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
	_ASSERT(m_pFontTexture);

	SDL_FreeSurface(pSurface);
	pSurface = nullptr;

	return true;
}

void Font::cleanUp()
{
	m_pFontTexture ? SDL_DestroyTexture(m_pFontTexture) : false;
	m_pFontTexture = nullptr;
}

SDL_Texture* Font::GetFontTexture() const
{
	return m_pFontTexture;
}