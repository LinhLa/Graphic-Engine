#pragma once
#include <SDL.h>
#include "RenderContext.h"
class PixelManipulator
{
private:
	SDL_PixelFormat* m_pPixelFormat = nullptr;
public:
	PixelManipulator(uint32_t);
	~PixelManipulator();
	SDL_PixelFormat* GetPixelFormat();
	uint32_t MapRGBA(SDL_Color);
	void GetRGBA(uint32_t, SDL_Color&);
};

class TextureManipulator
{
private:
	SDL_Texture* m_pTexture = nullptr;
	Uint32* m_PixelMatrix = nullptr;
	int m_Pitch = 0;

	bool lockTexture();
	bool unlockTexture();
public:
	TextureManipulator(ITextureRenderPtr);
	TextureManipulator(SDL_Texture*);
	~TextureManipulator();

	void* getPixels();
	int getPitch();

	void CopyPixelData(void*, size_t);
	void ColorKey(SDL_Color);
};