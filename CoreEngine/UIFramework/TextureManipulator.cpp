#include "stdafx.h"
#include "TextureManipulator.h"
#include "UIHelper.h"
#include <exception>
#include "UIHelper.h"
#include "log.h"

TextureManipulator::TextureManipulator(ITextureRenderPtr pTextureRender)
{
	m_pTexture = pTextureRender->getSDLTexture();
	if (!m_pTexture)
	{
		throw std::logic_error("invalid argument");
	}
	lockTexture();
}

TextureManipulator::TextureManipulator(SDL_Texture* pTexture)
{
    m_pTexture = pTexture;
    if (!m_pTexture)
    {
        throw std::logic_error("invalid argument");
    }
    lockTexture();
}

TextureManipulator::~TextureManipulator()
{
	unlockTexture();
}

bool TextureManipulator::lockTexture()
{
    bool success = true;

    //Texture is already locked
    if( m_PixelMatrix != nullptr )
    {
        LOG_DEBUG("Texture is already locked!");
        success = false;
    }
    else
    {
         //Lock texture
        if(0 != SDL_LockTexture(m_pTexture, nullptr, reinterpret_cast<void**>(&m_PixelMatrix), &m_Pitch))
        {
            SDL_Log("Unable to lock texture:%s", SDL_GetError());
            success = false;
        }
    }

    return success;
}

bool TextureManipulator::unlockTexture()
{
    bool success = true;

    //Texture is not locked
    if(nullptr == m_PixelMatrix)
    {
        LOG_DEBUG( "Texture is not locked!" );
        success = false;
    }
    //Unlock texture
    else
    {
        SDL_UnlockTexture(m_pTexture);
        m_PixelMatrix = nullptr;
        m_Pitch = 0;
    }

    return success;
}

void* TextureManipulator::getPixels()
{
    return m_PixelMatrix;
}

int TextureManipulator::getPitch()
{
    return m_Pitch;
}

void TextureManipulator::CopyPixelData(void* pixels, size_t len)
{
    if (pixels && len > 0)
    {
        //Copy loaded/formatted surface pixels
        memcpy(m_PixelMatrix, pixels, len);
    }
}

void TextureManipulator::ColorKey(SDL_Color color)
{
  //Get texture information
    uint32_t uPixelFormat = 0;
    int heigh = 0;
    int width = 0;
    int access = 0;
    if (0 > SDL_QueryTexture(m_pTexture, &uPixelFormat, &access, &width, &heigh))
    {
        SDL_Log("Couldn't query texture: %s", SDL_GetError());
        _ASSERT(false);
    }

    //Get pixel format
    PixelManipulator pixelManip(uPixelFormat);
    int pixelCount = ( m_Pitch / pixelManip.GetPixelFormat()->BytesPerPixel ) * heigh;

    //Color pixels
    for(int i = 0; i < pixelCount; ++i)
    {
        SDL_Color pixelColor;

        //Get RGBA pixel value
        pixelManip.GetRGBA(m_PixelMatrix[i], pixelColor);

        //Keep alpha
        color.a = pixelColor.a;

        //Set RGBA pixel value
        m_PixelMatrix[i] = pixelManip.MapRGBA(color);
    }
}

PixelManipulator::PixelManipulator(uint32_t format)
{
    //Allocate format from window
    m_pPixelFormat = SDL_AllocFormat(format);
    if (nullptr == m_pPixelFormat)
    {
        SDL_Log("SDL_AllocFormat error:%s", SDL_GetError());
        throw std::logic_error("invalid argument");
    }
}

PixelManipulator::~PixelManipulator()
{
    if (nullptr != m_pPixelFormat)
    {
        //Free format
        SDL_FreeFormat( m_pPixelFormat );
    }
}

SDL_PixelFormat* PixelManipulator::GetPixelFormat()
{
    return m_pPixelFormat;
}

uint32_t PixelManipulator::MapRGBA(SDL_Color pixelColor)
{
    return SDL_MapRGBA(m_pPixelFormat, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
}

void PixelManipulator::GetRGBA(uint32_t pixelValue, SDL_Color& pixelColor)
{
    SDL_GetRGBA(pixelValue, m_pPixelFormat, &pixelColor.r, &pixelColor.g, &pixelColor.b, &pixelColor.a);
}