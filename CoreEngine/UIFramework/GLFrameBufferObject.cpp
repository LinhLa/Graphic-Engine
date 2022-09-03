#include "stdafx.h"
#include <stdexcept>
#include "GLRenderManipulator.h"
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
		m_framebuffer = 0U;
	}

	if (m_rbo)
	{
		glDeleteRenderbuffers(1, &m_rbo);
		m_rbo = 0U;
	}
}

/**
 * @brief      Constructs a new instance.
 *
 * @param      pRenderer     The renderer
 * @param[in]  texture_path  The texture path
 */
GLFrameBufferObject::GLFrameBufferObject(std::string name, int width, int height) :m_name(name)
{
	m_resolution = glm::i32vec2(width, height);
}

GLFrameBufferObject::GLFrameBufferObject(std::string name, glm::i32vec4 viewport) : m_name(name)
{
	m_resolution = glm::i32vec2(viewport[2], viewport[3]);
}

bool GLFrameBufferObject::isInit()
{
	return ((nullptr != m_pTexture) && (m_pTexture->getID() != 0) && (m_framebuffer != 0));
}

void GLFrameBufferObject::initTexture2D(GLenum attachment)
{
	if (isInit())
	{
		LOG_OFF("Frame buffer already init");
		return;
	}

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// generate texture
	unsigned int textureColorbuffer = 0U;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	// Give an empty image to OpenGL ( the last "0" )
	switch (attachment)
	{
	case GL_COLOR_ATTACHMENT0:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_resolution[0], m_resolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		break;
	case GL_DEPTH_STENCIL_ATTACHMENT:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_resolution[0], m_resolution[1], 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		break;
	default:
		break;
	}

	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureColorbuffer, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {attachment};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_resolution[0], m_resolution[1]);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_DEBUG("fail to attached frame buffer");
		_ASSERT(false);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, GLRenderFBOManipulator::top());

	m_pTexture = GLTexture::create(m_name, textureColorbuffer);
}

void GLFrameBufferObject::initTexture2DMS(GLenum attachment, GLsizei sample)
{
	if (isInit())
	{
		LOG_OFF("Frame buffer already init");
		return;
	}

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// generate texture
	unsigned int textureColorbuffer = 0U;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sample, GL_RGBA, m_resolution[0], m_resolution[1], GL_TRUE);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer, 0);
	
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {attachment};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample, GL_DEPTH24_STENCIL8, m_resolution[0], m_resolution[1]);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_DEBUG("fail to attached frame buffer");
		_ASSERT(false);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, GLRenderFBOManipulator::top());

	m_pTexture = GLTexture::create(m_name, textureColorbuffer);
}


void GLFrameBufferObject::initDepthMap(GLenum attachment)
{
	if (isInit())
	{
		LOG_OFF("Frame buffer already init");
		return;
	}

	glGenFramebuffers(1, &m_framebuffer);

	// generate texture
	unsigned int depthMapTexture = 0U;
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, attachment, m_resolution[0], m_resolution[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

	//  explicitly tell OpenGL we're not going to render any color data
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_DEBUG("fail to attached frame buffer");
		_ASSERT(false);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, GLRenderFBOManipulator::top());

	m_pTexture = GLTexture::create(m_name, depthMapTexture);
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


