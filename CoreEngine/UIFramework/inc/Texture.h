#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include "Surface.h"
#include "creator.h"
#include "RenderContext.h"
class Texture: public creator<Texture>, public ITextureRender
{
private:
	bool m_isLoadToGPU = true;

	int m_width = 0;
	int m_height = 0;

	SDL_Texture* m_pTexture = nullptr;
	SurfacePtr 	m_pSurface = nullptr;

	void cleanUp();
	SDL_Texture* getSDLTexture() override;
protected:
	Texture(SDL_Renderer*, SurfacePtr);
	Texture(SDL_Renderer*, const std::string&);
	Texture(SDL_Renderer*, void*, size_t);
public:
	virtual ~Texture();
	friend class creator<Texture>;

	void setAlpha(Uint8);

	int GetWidth() const;
	int GetHeight() const;

	void LoadToGPU(bool flag = true);
	bool IsLoadToGPU() const;

	std::shared_ptr<Texture> Clone(SDL_Renderer*, float scale = 1.0F);
};

typedef std::shared_ptr<Texture> TexturePtr;