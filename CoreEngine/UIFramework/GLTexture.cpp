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
	// load and generate the texture
	unsigned char* data = stbi_load(m_path.c_str(), &m_width, &m_height, &m_Channels, 0);
	GLint iformat;
	if (!data)
	{
		throw std::logic_error("Failed to load texture");
	}
	if (4 == m_Channels)
	{
		iformat = GL_RGBA;
	}
	else
	{
		iformat = GL_RGB;
	}
	glTexImage2D(m_target, 0, iformat, m_width, m_height, 0, iformat, GL_UNSIGNED_BYTE, data);
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
void GLTexture::active(GLenum location)
{
	glActiveTexture(location);
	glBindTexture(m_target, m_textureID);
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

