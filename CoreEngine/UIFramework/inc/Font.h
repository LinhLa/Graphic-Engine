#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "RenderContext.h"
#include <memory>
#include "creator.h"
class Font: public creator<Font>, public ITextureRender
{
private:
	int m_width = 0;
	int m_height = 0;

	SDL_Texture*	m_pFontTexture = nullptr;
	TTF_Font*		m_pTTFfont = nullptr;

	void cleanUp();
	SDL_Texture* getSDLTexture() override;

	Font();
public:
	virtual ~Font();
	friend class creator<Font>;

	bool loadFont(const char*, SDL_Color, SDL_Renderer*);
	bool OpenFont(const char*, int32_t );

	void setAlpha(Uint8);

	int GetWidth() const;
	int GetHeight() const;
};

typedef std::shared_ptr<Font> FontPtr;