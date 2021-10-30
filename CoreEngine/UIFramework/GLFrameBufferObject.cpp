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

	if (m_fbo)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
}

/**
 * @brief      Constructs a new instance.
 *
 * @param      pRenderer     The renderer
 * @param[in]  texture_path  The texture path
 */
GLFrameBufferObject::GLFrameBufferObject(std::string name, GLTexturePtr pTexture)
{
	m_name = name;
	m_pTexture = pTexture;
	if (!m_pTexture)
	{
		throw std::logic_error("invalid argument");
	}
}

/**
 * @brief      Initializes the object.
 */
void GLFrameBufferObject::init()
{
	//Creating a framebuffer
	glGenFramebuffers(1, &m_fbo);

	// attach texture to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_pTexture->getTarget(), m_pTexture->getID(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_DEBUG("fail to attached frame buffer");
		_ASSERT(false);
	}
}

std::string GLFrameBufferObject::getName() const
{
	return m_name;
}

GLuint GLFrameBufferObject::getID() const
{
	return m_fbo;
}