#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "GLInfoLog.h"
#include "log.h"

Shader::Shader(const std::string& name, GLenum type):m_name(name), m_Type(type)
{
	m_ShaderID = glCreateShader(m_Type);
}

Shader::~Shader()
{
	glDeleteShader(m_ShaderID);
}

void Shader::ShaderSource(GLsizei count, const GLchar* const * shaderSource, const GLint *length)
{
	glShaderSource(m_ShaderID, count, shaderSource, length);
}

void Shader::CompileShader()
{
	glCompileShader(m_ShaderID);
	int32_t success;
	glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        LOG_DEBUG("Shader[%s] compilation failed: %s", m_name.c_str(), GLInfoLog(m_ShaderID).GetInfoLog());
        _ASSERT(false);
    }
}

uint32_t Shader::GetShaderId() const
{
	return m_ShaderID;
}

GLenum Shader::Type() const
{
	return m_Type;
}