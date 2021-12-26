#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include "creator.h"
#include <glm/glm.hpp>

#include "GLTexture.h"

class Character final: public creator<Character>
{
public:
	uint8_t			mCharacter = 0U;
	GLuint			mTextureID = 0U;  // ID handle of the glyph texture
	glm::ivec2		mSize;       // Size of glyph
	glm::ivec2		mBearing;    // Offset from baseline to left/top of glyph
	unsigned int	mAdvance = 0;    // Offset to advance to next glyph

	~Character();
protected:
	friend class creator<Character>;
	Character(
	uint8_t		character,
	GLuint		TextureID,
	glm::ivec2	Size,
	glm::ivec2  Bearing,
	unsigned int Advance);

	void cleanUp();
};

typedef std::shared_ptr<Character> CharacterPtr;

class GLFont final: public creator<GLFont>
{
private:
	std::vector<CharacterPtr> m_CharacterList;

	FT_Library			m_ft = nullptr;
	FT_Face				m_face = nullptr;

	void cleanUp();

	GLFont();
public:
	~GLFont();
	friend class creator<GLFont>;

	bool loadFont(const char*);
	bool OpenFont(const char*, int32_t );

	std::vector<CharacterPtr> getCharacterList();
};

typedef std::shared_ptr<GLFont> GLFontPtr;