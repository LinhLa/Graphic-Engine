#include "stdafx.h"
#include "Texture.h"
#include <SDL_image.h>
#include <assert.h>
#include "log.h"
#include "UIHelper.h"
#include "TextureManipulator.h"

/**
 * @brief      Destroys the object.
 */
Texture::~Texture()
{
	cleanUp();
}


/**
 * @brief      Constructs a new instance.
 *
 * @param      pTexture  The texture
 */
Texture::Texture(SDL_Renderer* pRenderer, SurfacePtr pSurface)
{
	m_pSurface = pSurface;
	if (!m_pSurface)
	{
		throw std::logic_error("invalid argument");
	}

	m_pSurface->CreateTextureFromSurface(pRenderer, &m_pTexture);

	m_width = m_pSurface->Width();
	m_height = m_pSurface->Height();
}

/**
 * @brief      Constructs a new instance.
 *
 * @param      pRenderer     The renderer
 * @param[in]  texture_path  The texture path
 */
Texture::Texture(SDL_Renderer* pRenderer, const std::string& texture_path)
{
	/* Open the image file */
	m_pSurface = Surface::create(texture_path.c_str());
	m_pSurface->CreateTextureFromSurface(pRenderer, &m_pTexture);

	m_width = m_pSurface->Width();
	m_height = m_pSurface->Height();
}

/**
 * @brief      Constructs a new instance.
 *
 * @param      pRenderer  The renderer
 * @param      mem        The memory
 * @param[in]  size       The size
 */
Texture::Texture(SDL_Renderer* pRenderer, void* mem, size_t size)
{
	m_pSurface = Surface::create(mem, size);
	m_pSurface->CreateTextureFromSurface(pRenderer, &m_pTexture);

	m_width = m_pSurface->Width();
	m_height = m_pSurface->Height();
}


/**
 * @brief      Gets the sdl texture.
 *
 * @return     The sdl texture.
 */
SDL_Texture* Texture::getSDLTexture()
{
	return m_pTexture;
}

/**
 * @brief      { function_description }
 */
void Texture::cleanUp()
{
	if (nullptr != m_pTexture)
	{
		SDL_DestroyTexture(m_pTexture);
		m_pTexture = nullptr;
	}
}

/**
 * @brief      Sets the alpha.
 *
 * @param[in]  alpha  The alpha
 */
void Texture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(m_pTexture, alpha);
}


/**
 * @brief      Gets the width.
 *
 * @return     The width.
 */
int Texture::GetWidth() const
{
	return m_width;
}

/**
 * @brief      Gets the height.
 *
 * @return     The height.
 */
int Texture::GetHeight() const
{
	return m_height;
}

/**
 * @brief      Loads to gpu.
 *
 * @param[in]  flag  The flag
 */
void Texture::LoadToGPU(bool flag)
{
	m_isLoadToGPU = flag;
}

/**
 * @brief      Determines if load to gpu.
 *
 * @return     True if load to gpu, False otherwise.
 */
bool Texture::IsLoadToGPU() const
{
	return m_isLoadToGPU;
}

TexturePtr Texture::Clone(SDL_Renderer* pRenderer, float scale)
{
	//Create new texture
	return Texture::create(UIHelper::GetRenderer(), Surface::create(m_pSurface, scale));
}