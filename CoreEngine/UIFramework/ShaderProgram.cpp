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
	if (!m_UniformList.empty())
	{
		return m_UniformList;
	}

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
	if (!m_AttributeList.empty())
	{
		return m_AttributeList;
	}

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

void ShaderProgram::syncUniformToUIObject(UIObjectPtr pObject)
{
	if(!pObject)
	{
		return;
	}

	m_UniformList = querryUniform();
	for(auto &pUniform: m_UniformList)
	{
		if (false == pObject->IsPropertyExist(pUniform->getName()))
		{
			LOG_DEBUG("Uniform Sync: %s", pUniform->getName().c_str());
			pUniform->sync(pObject);
		}
	}
}

void ShaderProgram::setUniformFromUIObject(UIObjectPtr pObject)
{
	if(!pObject)
	{
		return;
	}

	for(auto &pUniform: m_UniformList)
	{
		if (true == pObject->IsPropertyExist(pUniform->getName()))
		{
			pUniform->set(pObject->GetProperty(pUniform->getName()));
		}
		else
		{
			LOG_DEBUG("Uniform not sync: %s", pUniform->getName().c_str());
		}
	}
}