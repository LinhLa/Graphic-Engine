#include "stdafx.h"
#include <exception>
#include <Surface.h>
#include <SDL_image.h>
#include "TextureManipulator.h"
#include "UIHelper.h"
#include "log.h"

Surface::Surface(const char* path)
{
	/* Open the image file */
	m_pSurface = IMG_Load(path);
	if(!m_pSurface)
	{
		SDL_Log("Couldn't load %s: %s\n", path, SDL_GetError());
		_ASSERT(false);
	}

	if (0 <  SDL_SetSurfaceBlendMode(m_pSurface, SDL_BLENDMODE_BLEND))
	{
		SDL_Log("SDL_SetSurfaceBlendMode() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
}

Surface::Surface(TTF_Font *pfont, const char *text, SDL_Color color)
{
	m_pSurface = TTF_RenderUTF8_Blended(pfont, text, color);
	if (!m_pSurface)
	{
		SDL_Log("Couldn't load font: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	if (0 <  SDL_SetSurfaceBlendMode(m_pSurface, SDL_BLENDMODE_BLEND))
	{
		SDL_Log("SDL_SetSurfaceBlendMode() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
}


Surface::Surface(void* mem, size_t size)
{
	SDL_RWops* SdlHandle = SDL_RWFromMem(mem, size);

	m_pSurface = IMG_LoadPNG_RW(SdlHandle);
	if (!m_pSurface)
	{
		SDL_Log("Couldn't create surface: %s\n", IMG_GetError());
		_ASSERT(false);
	}

	if (0 <  SDL_SetSurfaceBlendMode(m_pSurface, SDL_BLENDMODE_BLEND))
	{
		SDL_Log("SDL_SetSurfaceBlendMode() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
}

Surface::Surface(SurfacePtr pSource, float scale)
{
	if (!pSource)
	{
		throw std::logic_error("invalid argument");
	}

	/* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order, as expected by OpenGL for textures */
    uint32_t rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	int width = static_cast<int>(float(pSource->Width()) * scale);
	int height = static_cast<int>(float(pSource->Height()) * scale);
	int depth = pSource->BytePerPixel();

	m_pSurface = SDL_CreateRGBSurface(0, width, height, depth, rmask, gmask, bmask, amask);
    /* or using the default masks for the depth: */
	//m_pSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	if (NULL == m_pSurface)
	{
		SDL_Log("SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	BlitSurface(pSource->m_pSurface, scale);
}

Surface::~Surface()
{
	cleanUp();
}

void Surface::cleanUp()
{
	if (nullptr != m_pSurface)
	{
		SDL_FreeSurface(m_pSurface);
		m_pSurface = nullptr;
	}
}

int Surface::SetBlendMode(SDL_BlendMode blendMode)
{
	int iResult = SDL_SetSurfaceBlendMode(m_pSurface, blendMode);
	if (0 < iResult)
	{
		SDL_Log("SDL_SetSurfaceBlendMode() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return iResult;
}

int Surface::GetBlendMode(SDL_BlendMode* blendMode)
{
	int iResult = SDL_GetSurfaceBlendMode(m_pSurface, blendMode);
	if (0 < iResult)
	{
		SDL_Log("SDL_GetSurfaceBlendMode() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return iResult;
}

void Surface::CreateTextureFromSurface(SDL_Renderer* pRenderer, SDL_Texture** pTexture)
{
	if (!pRenderer || !pTexture)
	{
		LOG_DEBUG("invalid argument");
		_ASSERT(false);
		return;
	}

    //Create blank streamable texture
	*pTexture = SDL_CreateTexture(pRenderer, m_pSurface->format->format, SDL_TEXTUREACCESS_STREAMING, m_pSurface->w, m_pSurface->h);
	if (NULL == *pTexture)
	{
		SDL_Log("Couldn't create texture: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//Copy loaded/formatted surface pixels by using Texture Manip
	TextureManipulator(*pTexture).CopyPixelData(m_pSurface->pixels, m_pSurface->pitch * m_pSurface->h);
	if (0 > SDL_SetTextureBlendMode(*pTexture, SDL_BLENDMODE_BLEND))
	{
		SDL_Log("Couldn't set blending mode: %s\n", SDL_GetError());
		_ASSERT(false);
	}

}

int Surface::BlitSurface(SDL_Surface* pSource, float scale)
{
	int iResult = -1;
	if (!pSource)
	{
		LOG_DEBUG("invalid argument");
		_ASSERT(false);
		return iResult;
	}

	ConvertToFormat(pSource, pSource->format);
	iResult = SDL_BlitSurface(pSource, NULL, m_pSurface, NULL);
	//We not use scale surface iResult = SDL_BlitScaled(pSource, NULL, m_pSurface, NULL);
	if (0 > iResult)
	{
		SDL_Log("Failed to blit surface: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return iResult;
}

void Surface::ConvertToFormat(SDL_Surface* pSource, const SDL_PixelFormat* pixelFormat)
{
	m_pSurface = SDL_ConvertSurface(pSource, pixelFormat, 0);
	if (!m_pSurface)
	{
		SDL_Log("SDL_ConvertSurface() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
}

int Surface::Width() const
{
	return m_pSurface->w;
}
int Surface::Height() const
{
	return m_pSurface->h;
}
int Surface::BytePerPixel() const
{
	return m_pSurface->format->BytesPerPixel;
}
uint32_t Surface::PixelFormat() const
{
	return m_pSurface->format->format;
}