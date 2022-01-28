#pragma once
#include "stdafx.h"
#include <vector>
#include <fstream>
#include "ShaderProgram.h"
#include "GLInfoLog.h"
#include "log.h"
#include "Configuration.h"
#include "GLTexture.h"
#include "Library.h"
#include "UIObject.h"

ShaderProgram::ShaderProgram(const std::string& name):m_name(name)
{
	m_ProgramID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ProgramID);
}

void ShaderProgram::attachShader(ShaderPtr pShader)
{
	if (pShader)
	{
		glAttachShader(m_ProgramID, pShader->getId());

		if (GL_FRAGMENT_SHADER == pShader->type())
		{
			m_pFragment = pShader;
		}
		else if (GL_VERTEX_SHADER == pShader->type())
		{
			m_pVertex = pShader;
		}
		else{}
	}
}

void ShaderProgram::linkProgram()
{
	// link shaders
	glLinkProgram(m_ProgramID);
	if (!GLInfoLog(m_ProgramID, GL_LINK_STATUS).CheckGLSuccess())
	{
		LOG_DEBUG("Shader program[%s] link failed: %s", m_name.c_str(), GLInfoLog::GetInfoLog());
		_ASSERT(false);
	}
}

void ShaderProgram::useProgram()
{
	glUseProgram(m_ProgramID);
}

void ShaderProgram::read()
{
	// Load binary from file
	std::string path = Configuration::GetInstance()->shader_bin + m_name + ".bin";
	std::ifstream inputStream(path, std::ios::binary);
	std::istreambuf_iterator<char> startIt(inputStream), endIt;
	std::vector<char> buffer(startIt, endIt);  // Load file
	inputStream.close();
}

void ShaderProgram::write()
{
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	if( formats < 1 )
	{
		LOG_DEBUG("Driver does not support any binary formats");
		_ASSERT(false);
	}

	// Get the binary length
	GLint length = 0;
	glGetProgramiv(m_ProgramID, GL_PROGRAM_BINARY_LENGTH, &length);

	// Retrieve the binary code
	std::vector<GLubyte> buffer(length);
	GLenum format = 0;
	glGetProgramBinary(m_ProgramID, length, NULL, &format, buffer.data());

	// Write the binary to a file.
	std::string path = Configuration::GetInstance()->shader_bin + m_name + ".bin";
	LOG_DEBUG("Writing[%s] binary format[%d]" , path.c_str(), format);
	std::ofstream(path, std::ios::binary).write( reinterpret_cast<char *>(buffer.data()),length);
}

void ShaderProgram::debug()
{
	auto uniformList = querryUniform();
	auto attributeList = querryAttribute();
	LOG_DEBUG("------------------------------ SHADER PROGRAM[%s] ------------------------------", m_name.c_str());
	if (!uniformList.empty())
	{
		for(auto &pUniform: uniformList)
		{
			pUniform->debug();
		}
	}

	if (!attributeList.empty())
	{
		for(auto &pAttribute: attributeList)
		{
			pAttribute->debug();
		}
	}
}

std::vector<IUniformPtr> ShaderProgram::querryUniform()
{
	m_UniformList.clear();

	//<Query uniform from program shader
	GLint maxLen = 0;
	GLint maxElement = 0;
	glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORMS, &maxElement);

	std::vector<char> buffer(maxLen);
	GLenum 	type = 0;
	GLsizei length = 0;
	GLint 	size = 0;
	for(GLint index = 0; index < maxElement; ++index)
	{
		glGetActiveUniform(	m_ProgramID,
						 	index,
						 	maxLen,		//maximum number of characters OpenGL is allowed to write
						 	&length,	//number of characters actually written by OpenGL
						 	&size,		//size of the uniform variable.
						 	&type,		//data type of the uniform variable.
						 	buffer.data());
		if (0 == length)
		{
			//occur if this function is called after a link operation that failed.
			continue;
		}
		else if (!m_UniformList.empty() && m_UniformList.back()->getName() == std::string(buffer.data()))
		{
			//If an error occurs, the return values length, size, type, and name will be unmodified.
			continue;
		}
		else
		{
			IUniformPtr pUniform = ::create(buffer.data(), index, type, size);
			m_UniformList.push_back(pUniform);
			if (GL_SAMPLER_2D == type)
			{
				m_textureUnitCount++;
			}
		}
	}
	return m_UniformList;
}

std::vector<AttributePtr> ShaderProgram::querryAttribute()
{
	m_AttributeList.clear();

	//<Query uniform from program shader
	GLint maxLen = 0;
	GLint maxElement = 0;
	glGetProgramiv(m_ProgramID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLen);
	glGetProgramiv(m_ProgramID, GL_ACTIVE_ATTRIBUTES, &maxElement);

	std::vector<char> buffer(maxLen);
	GLenum 	type = 0;
	GLsizei length = 0;
	GLint 	size = 0;
	for(GLint index = 0; index < maxElement; ++index)
	{
		glGetActiveAttrib(	m_ProgramID,
						 	index,
						 	maxLen,		//maximum number of characters OpenGL is allowed to write
						 	&length,	//number of characters actually written by OpenGL
						 	&size,		//size of the attribute variable.
						 	&type,		//data type of the attribute variable.
						 	buffer.data());
		if (0 == length)
		{
			//occur if this function is called after a link operation that failed.
			continue;
		}
		else if (!m_AttributeList.empty() && m_AttributeList.back()->getName() == std::string(buffer.data()))
		{
			//If an error occurs, the return values length, size, type, and name will be unmodified.
			continue;
		}
		else
		{
			AttributePtr pAttribute = Attribute::create(buffer.data(), index, type, size);
			m_AttributeList.push_back(pAttribute);
		}
	}
	return m_AttributeList;
}

uint32_t ShaderProgram::getID() const
{
	return m_ProgramID;
}

std::string ShaderProgram::getName() const
{
	return m_name;
}

GLint ShaderProgram::getTextureUnitCount() const
{
	return m_textureUnitCount;
}

void ShaderProgram::syncUniform(std::shared_ptr<PropertyTable> pObject)
{
	if(!pObject)
	{
		return;
	}

	if (m_UniformList.empty())
	{
		m_UniformList = querryUniform();
	}

	for(auto &pUniform: m_UniformList)
	{
		if (false == pObject->IsPropertyExist(pUniform->getName()))
		{
			LOG_DEBUG("Uniform Sync: %s", pUniform->getName().c_str());
			pUniform->sync(pObject);
		}
	}
}

void ShaderProgram::setUniform(MaterialPtr pMaterial)
{
	if(!pMaterial)
	{
		return;
	}
	GLenum textureLocation = GL_TEXTURE0;
	GLenum location = 0;

	glEnable(GL_TEXTURE_2D);
	for(auto &pUniform: m_UniformList)
	{
		if (true == pMaterial->IsPropertyExist(pUniform->getName()))
		{
			//<update value to uniform object
			pUniform->set(pMaterial->GetProperty(pUniform->getName()));
		}
		else
		{
			LOG_OFF("Uniform not sync: %s", pUniform->getName().c_str());
		}


		//<set uniform
		location = glGetUniformLocation(m_ProgramID, pUniform->getName().c_str());
		if (-1 != location)
		{
			if (GL_SAMPLER_2D == pUniform->getType())
			{
				GLTexturePtr pTexture = nullptr;
				
				if (pMaterial->GetProperty(pUniform->getName()))
				{
					auto property = pMaterial->GetProperty(pUniform->getName());
					if (property)
					{
						
						auto textureUnit = property->GetValue<glm::u32>();
						pUniform->glUniform(location, (void*)&textureLocation);
						glActiveTexture(textureLocation);
						glBindTexture(GL_TEXTURE_2D, textureUnit);
						textureLocation++;
					}
				}
				else if (pMaterial->hasTextureMap(pUniform->getName()))//<querry texture from material
				{
					pTexture = pMaterial->GetTexture(pUniform->getName());
					pUniform->glUniform(location, (void*)&textureLocation);
					pTexture->setLocation(textureLocation);
					pTexture->active();
					textureLocation++;

				}
			}
			else
			{
				pUniform->glUniform(location, nullptr);
			}
		}
		else
		{
			LOG_DEBUG("Couldn't get uniform location for:%s", pUniform->getName().c_str());
		}
	}

	GLint max_texture_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);

	if (max_texture_units <= GLint(textureLocation - GL_TEXTURE0))
	{
		LOG_DEBUG("Over max texture units: Max[%d] Current[%d]", max_texture_units, textureLocation);
	}
}

void ShaderProgram::setUnifromMatrix(glm::mat4 matrix, const char* uniform)
{
	GLint uniformLocation = glGetUniformLocation(m_ProgramID, uniform);
	if (-1 != uniformLocation)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	else
	{
		LOG_DEBUG("Shader program[%s] No uniform location for [%s]", m_name.c_str(), uniform);
	}
}