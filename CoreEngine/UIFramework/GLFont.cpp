#include "stdafx.h"

#include "log.h"
#include "Configuration.h"

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <string>
#include "GLFont.h"

#include "GLRenderManipulator.h"
#include "Renderer3D.h"

Character::Character(
                uint8_t character,
                GLuint		TextureID,
                glm::ivec2  Size,
                glm::ivec2  Bearing,
                unsigned int	Advance) :mCharacter(character), mTextureID(TextureID), mSize(Size), mBearing(Bearing), mAdvance(Advance)
{
}

Character::~Character()
{
    cleanUp();
}

void Character::cleanUp()
{
    if (0 != mTextureID)
    {
        glDeleteTextures(1, &mTextureID);
    }
}

GLFont::GLFont(){}

GLFont::~GLFont()
{
	cleanUp();
}

bool GLFont::loadFont(const char* text)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    glEnable(GL_TEXTURE_2D);
    std::string str(text);
    m_CharacterList.clear();
    for(unsigned char c : str)
    {
        // load character glyph 
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
        {
            LOG_DEBUG("ERROR::FREETYTPE: Failed to load Glyph");
            _ASSERT(false);
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            m_face->glyph->bitmap.width,
            m_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            m_face->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        auto pcharacter = Character::create(
            c,
            texture,
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            static_cast<unsigned int>(m_face->glyph->advance.x)
        );
        m_CharacterList.push_back(pcharacter);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    cleanUp();
	return true;
}

bool GLFont::OpenFont(const char* fontName, int32_t pointSize)
{
	if (FT_Init_FreeType(&m_ft))
	{
		LOG_DEBUG("ERROR::FREETYPE: Could not init FreeType Library");
        _ASSERT(false);
	}

	std::string fontPath = std::string(Configuration::GetInstance()->font_path) + std::string(fontName);
	if (FT_New_Face(m_ft, fontPath.c_str(), 0, &m_face))
	{
		LOG_DEBUG("ERROR::FREETYPE: Failed to load font");
        _ASSERT(false);
	}
	
	if (FT_Set_Pixel_Sizes(m_face, 0, pointSize))
	{
		LOG_DEBUG("ERROR::FREETYPE: Failed to set font size");
        _ASSERT(false);
	}
	return true;
}

void GLFont::renderTexture(
    GLTexturePtr	pTexture,
    glm::vec2 coordinator,
    glm::vec2 scale,
    float angle,
    glm::vec2 pivot,
    glm::vec4 color)
{
    Renderer3D::GetInstance()->DrawText2D(m_CharacterList, coordinator, scale, angle, pivot, color.a, glm::vec3(color.r, color.g, color.b));
}

void GLFont::cleanUp()
{
	if (m_face)
	{
		FT_Done_Face(m_face);
		m_face = nullptr;
	}

	if (m_ft)
	{
		FT_Done_FreeType(m_ft);
		m_ft = nullptr;
	}
}