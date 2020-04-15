#include "stdafx.h"
#include "Texture.h"
#include <SDL_image.h>

Texture::Texture()
{
}

Texture::~Texture()
{
	cleanUp();
}

//Loads media
bool Texture::loadImage(const char* path, SDL_Renderer* pRenderer)
{
	/* Open the image file */
	SDL_Surface* pSurface = IMG_Load(path);
	_ASSERT(pSurface);
	if (pSurface) {
		m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
		SDL_FreeSurface(pSurface);
		pSurface = nullptr;
	}

	if (!m_pTexture) {
		SDL_Log("Couldn't load %s: %s\n", path, SDL_GetError());
		_ASSERT(false);
	}

	int result = SDL_QueryTexture(m_pTexture, NULL, NULL, &m_width, &m_height);
	_ASSERT(-1 != result);
	return true;
}

Texture& Texture::CreateTextureFromSurface(SDL_Surface* pSurface, SDL_Renderer* pRenderer)
{
	_ASSERT(pSurface);
	//<Clean texture if not null
	cleanUp();

	//<Create texture from surface pixels
	m_pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
	if (nullptr == m_pTexture)
	{
		LOG_DEBUG("Unable to create texture from surface! SDL Error: %s", SDL_GetError());
	}
	return *this;
}

Texture& Texture::operator=(SDL_Texture* other)
{
	_ASSERT(other);
	cleanUp();

	m_pTexture = other;
	return *this;
}

SDL_Texture* Texture::getSDLTexture()
{
	return m_pTexture;
}

void Texture::cleanUp()
{
	m_pTexture ? SDL_DestroyTexture(m_pTexture) : false;
	m_pTexture = nullptr;
}

void Texture::Transparent(const SDL_Color& color, SDL_Renderer* pRenderer)
{
	_ASSERT(pRenderer);
	//<TODO: implement later
	/*SDL_Color old_color = {0};
	SDL_GetRenderDrawColor(pRenderer, &old_color.r, &old_color.g, &old_color.b, &old_color.a);
	SDL_SetRenderDrawColor(pRenderer, old_color.r, old_color.g, old_color.b, old_color.a);
*/
}


int Texture::GetWidth() const
{
	return m_width;
}

int Texture::GetHeight() const
{
	return m_height;
}
