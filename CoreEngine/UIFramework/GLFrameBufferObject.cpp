#include "stdafx.h"
#include <stdexcept>
#include "GLFrameBufferObject.h"
#include <assert.h>
#include "log.h"
#include "UIHelper.h"

/**
 * @brief      Destroys the object.
 */
GLFrameBufferObject::~GLFrameBufferObject()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (m_framebuffer)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
	}
}

/**
 * @brief      Constructs a new instance.
 *
 * @param      pRenderer     The renderer
 * @param[in]  texture_path  The texture path
 */
GLFrameBufferObject::GLFrameBufferObject(std::string name, int width, int height):m_name(name), m_width(width), m_height(height)
{
}

/**
 * @brief      Initializes the object.
 */
void GLFrameBufferObject::init()
{
	if (m_bInit)
	{
		return;
	}

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// generate texture
	unsigned int textureColorbuffer = 0U;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//glGenRenderbuffers(1, &m_rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_DEBUG("fail to attached frame buffer");
		_ASSERT(false);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pTexture = GLTexture::create(m_name, textureColorbuffer);
	m_bInit = true;
}

std::string GLFrameBufferObject::getName() const
{
	return m_name;
}

GLuint GLFrameBufferObject::getID() const
{
	return m_framebuffer;
}

GLuint GLFrameBufferObject::getTextureID() const
{
	return m_pTexture->getID();
}

GLTexturePtr GLFrameBufferObject::getTexture()
{
	return m_pTexture;
}


