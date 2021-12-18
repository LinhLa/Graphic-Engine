#include "stdafx.h"
#include <stdexcept>
#include "GLTexture.h"
#include <assert.h>
#include "log.h"
#include "UIHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/**
 * @brief      Destroys the object.
 */
GLTexture::~GLTexture()
{
	if (m_textureID)
	{
		glDeleteTextures(1, &m_textureID);
	}
}

/**
 * @brief      Constructs a new instance.
 *
 * @param      pRenderer     The renderer
 * @param[in]  texture_path  The texture path
 */
GLTexture::GLTexture(const std::string &name, const std::string& path, GLenum target):m_name(name),m_path(path), m_target(target)
{
}

/**
 * @brief      Constructs a new instance.
 *
 * @param      pRenderer     The renderer
 * @param[in]  texture_path  The texture path
 */
GLTexture::GLTexture(const std::string& name, GLenum target, int w, int h, GLint format):
	m_name(name), m_target(target), m_width(w), m_height(h),m_iformat(format)
{
	//Create empty texture2D
	glGenTextures(1, &m_textureID);
	glBindTexture(m_target, m_textureID);

	glTexImage2D(m_target, 0, m_iformat, m_width, m_height, 0, m_iformat, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLTexture::GLTexture(const std::string& name, GLuint textureID):m_name(name), m_textureID(textureID), m_target(GL_TEXTURE_2D)
{
	glBindTexture(m_target, m_textureID);
	glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_WIDTH, &m_width);
	glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_HEIGHT, &m_height);
	glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_INTERNAL_FORMAT, &m_iformat);
	glBindTexture(m_target, 0U);
}

/**
 * @brief      Initializes the object.
 */
void GLTexture::init()
{
	gen();
	bind();
	setParam();
	load();
	unbind();
}

/**
 * @brief      load texture
 */
void GLTexture::load()
{
	// turn on flip vertical image for top left origin
	stbi_set_flip_vertically_on_load(true);
	
	// load and generate the texture
	unsigned char* data = stbi_load(m_path.c_str(), &m_width, &m_height, &m_Channels, 0);
	if (!data)
	{
		throw std::logic_error("Failed to load texture");
	}
	if (4 == m_Channels)
	{
		m_iformat = GL_RGBA;
	}
	else
	{
		m_iformat = GL_RGB;
	}
	glTexImage2D(m_target, 0, m_iformat, m_width, m_height, 0, m_iformat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(m_target);
	stbi_image_free(data);
}

/**
 * @brief      Sets the texture parameter.
 *
 * @param[in]  func   The function
 * @param[in]  pname  The pname
 * @param[in]  param  The parameter
 */
void GLTexture::setTextureParam(GL_TEXTURE_FUNC func, GLenum pname, GLenum param)
{
	switch(func)
	{
		case TEXT_PARAM_F:
		glTexParameterf(m_target, pname, static_cast<GLfloat>(param));
		break;
		case TEXT_PARAM_I:
		glTexParameteri(m_target, pname, static_cast<GLint>(param));
		break;
		case TEXTURE_PARAM_F:
		glTextureParameterf(m_target, pname, static_cast<GLfloat>(param));
		break;
		case TEXTURE_PARAM_I:
		glTextureParameteri(m_target, pname, static_cast<GLint>(param));
		break;
		default:
		break;
	}
}

/**
 * @brief      Sets the texture parameter.
 *
 * @param[in]  func    The function
 * @param[in]  pname   The pname
 * @param[in]  params  The parameters
 */
void GLTexture::setTextureParam(GL_TEXTURE_FUNC func, GLenum pname, const void* params)
{
	switch(func)
	{
		case TEXT_PARAM_FV:
		glTexParameterfv(m_target, pname, static_cast<const GLfloat*>(params));
		break;
		case TEXT_PARAM_IV:
		glTexParameteriv(m_target, pname, static_cast<const GLint*>(params));
		break;
		case TEXT_PARAM_IIV:
		glTexParameterIiv(m_target, pname, static_cast<const GLint*>(params));
		break;
		case TEXT_PARAM_IUIV:
		glTexParameterIuiv(m_target, pname, static_cast<const GLuint*>(params));
		break;
		case TEXTURE_PARAM_FV:
		glTextureParameterfv(m_target, pname, static_cast<const GLfloat*>(params));
		break;
		case TEXTURE_PARAM_IV:
		glTextureParameteriv(m_target, pname, static_cast<const GLint*>(params));
		break;
		case TEXTURE_PARAM_IIV: 
		glTextureParameterIiv(m_target, pname, static_cast<const GLint*>(params));
		break;
		case TEXTURE_PARAM_IUIV:
		glTexParameterIuiv(m_target, pname, static_cast<const GLuint*>(params));
		break;
		default:
		break;
	}
}

/**
 * @brief      Adds a texture parameter.
 *
 * @param[in]  func   The function
 * @param[in]  pname  The pname
 * @param[in]  param  The parameter
 */
void GLTexture::addTextureParam(GL_TEXTURE_FUNC func, GLenum pname, GLenum param)
{
	m_scalarParamList.push_back(ScalarParam(func, pname, param));
}

/**
 * @brief      Adds a texture parameter.
 *
 * @param[in]  func    The function
 * @param[in]  pname   The pname
 * @param[in]  params  The parameters
 */
void GLTexture::addTextureParam(GL_TEXTURE_FUNC func, GLenum pname, const void* params)
{
	m_vectorParamList.push_back(VectorParam(func, pname, params));
}

/**
 * @brief      Sets the parameter.
 */
void GLTexture::setParam()
{
	for(auto &p: m_scalarParamList)
	{
		setTextureParam(p.func, p.pname, p.param);
	}

	for(auto &p: m_vectorParamList)
	{
		setTextureParam(p.func, p.pname, p.params);
	}
}

/**
 * @brief      Gets the width.
 *
 * @return     The width.
 */
int GLTexture::GetWidth() const
{
	return m_width;
}

/**
 * @brief      Gets the height.
 *
 * @return     The height.
 */
int GLTexture::GetHeight() const
{
	return m_height;
}


GLint GLTexture::getFormat() const
{
	return m_iformat;
}

GLenum GLTexture::getTarget() const
{
	return m_target;
}

void GLTexture::setLocation(GLenum location)
{
	m_location = location;
}

/**
 * @brief      Loads to gpu.
 *
 * @param[in]  flag  The flag
 */
void GLTexture::loadToGPU(bool flag)
{
	m_isLoadToGPU = flag;
}

/**
 * @brief      Determines if load to gpu.
 *
 * @return     True if load to gpu, False otherwise.
 */
bool GLTexture::isLoadToGPU() const
{
	return m_isLoadToGPU;
}

/**
 * @brief      { function_description }
 */
void GLTexture::gen()
{
	glGenTextures(1, &m_textureID);
}

/**
 * @brief      active texture
 */
void GLTexture::active()
{
	if ((0U != m_location) && (0U != m_textureID))
	{
		glActiveTexture(m_location);
		glBindTexture(m_target, m_textureID);
	}
}

/**
 * @brief      bind texture
 */
void GLTexture::bind()
{
	glBindTexture(m_target, m_textureID);
}

void GLTexture::unbind()
{
	glBindTexture(m_target, 0);
}

std::string GLTexture::getName() const
{
	return m_name;
}

GLuint GLTexture::getID() const
{
	return m_textureID;
}

