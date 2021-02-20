#include "stdafx.h"
#include <SDL_image.h>
#include <string>
#include "Font.h"
#include "log.h"
#include "Surface.h"
#include "Configuration.h"

Font::Font(){}

Font::~Font()
{
	cleanUp();
}

SDL_Texture* Font::getSDLTexture()
{
	return m_pFontTexture;
}

bool Font::loadFont(const char* text, SDL_Color fore_ground_color, SDL_Renderer* pRenderer)
{
	SurfacePtr pSurface = Surface::create(m_pTTFfont, text, fore_ground_color);
	pSurface->CreateTextureFromSurface(pRenderer, &m_pFontTexture);
	m_width = pSurface->Width();
	m_height = pSurface->Height();

	if (m_pTTFfont)
	{
		TTF_CloseFont(m_pTTFfont);
		m_pTTFfont = nullptr;
	}

	return true;
}

bool Font::OpenFont(const char* fontName, int32_t pointSize)
{
	std::string fontPath = std::string(Configuration::GetInstance()->font_path) + std::string(fontName);
	m_pTTFfont = TTF_OpenFont(fontPath.c_str(), pointSize);
	if (!m_pTTFfont)
	{
		SDL_Log("Couldn't open font: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return (nullptr != m_pTTFfont);
}

void Font::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(m_pFontTexture, alpha);
}

void Font::cleanUp()
{
	if (m_pFontTexture)
	{
		SDL_DestroyTexture(m_pFontTexture);
		m_pFontTexture = nullptr;
	}
}

int Font::GetWidth() const
{
	return m_width;
}

int Font::GetHeight() const
{
	return m_height;
}
