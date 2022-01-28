#pragma once
#include "stdafx.h"
#include <fstream>
#include "Shader.h"
#include "GLInfoLog.h"
#include "log.h"

Shader::Shader(const std::string& name, GLenum type):m_name(name), m_Type(type)
{
	m_ShaderID = glCreateShader(m_Type);
	if (0 == m_ShaderID || GL_FALSE == glIsShader(m_ShaderID))
	{
	    LOG_DEBUG("Could not create Shader[%s]", name.c_str());
	    _ASSERT(false);
	}
}

Shader::~Shader()
{
	glDeleteShader(m_ShaderID);
	for (auto& string : m_shaderSource)
	{
		if (string)
		{
			delete[] string;
		}
	}
}

void Shader::load(std::string path)
{
	//<Open shader source file
	std::ifstream inShaderStream(path.c_str());

	//<Validate ifstream
	_ASSERT(inShaderStream);

	//<Read to string
	std::string inShaderString((std::istreambuf_iterator<char>(inShaderStream)), std::istreambuf_iterator<char>());

	//<Attach source
	addSource(inShaderString.c_str(), inShaderString.length());
}

void Shader::addSource(const GLchar* shaderSource, GLint length)
{
	char* source = new char[length];
	std::memcpy(source, shaderSource, length);
	m_lenght.push_back(length);
	m_shaderSource.push_back(source);
}

void Shader::setSource(/*GLsizei count, const GLchar* const * shaderSource, const GLint *length*/)
{
	glShaderSource(m_ShaderID, m_shaderSource.size(), &m_shaderSource[0], &m_lenght[0]);
}

void Shader::compile()
{
	glCompileShader(m_ShaderID);
    if (!GLInfoLog(m_ShaderID, GL_COMPILE_STATUS).CheckGLSuccess())
    {
        LOG_DEBUG("Shader[%s] compilation failed: %s", m_name.c_str(), GLInfoLog::GetInfoLog());
        _ASSERT(false);
    }
}

uint32_t Shader::getId() const
{
	return m_ShaderID;
}

std::string Shader::getName() const
{
	return m_name;
}

GLenum Shader::type() const
{
	return m_Type;
}