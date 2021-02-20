#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
	m_ProgramID = glCreateProgram();
}

ShaderProgram::~ShaderProgram(){}

void ShaderProgram::ShaderSource(GLuint shader,	GLsizei count, const GLchar * const * shaderSource, const GLint *length, GLenum shaderType)
{
	GLuint iShader = glCreateShader(shaderType);
	glShaderSource(iShader, count, &shaderSource, length);
}

void ShaderProgram::CompileShader()
{
	for(auto &shader : m_shaderMap)
	{
		glCompileShader(shader.first);
	}
}

void ShaderProgram::AttachShader()
{
	for(auto &shader : m_shaderMap)
	{
		glAttachShader(m_ProgramID, shader.first);
	}
}

void ShaderProgram::LinkProgram()
{
	// link shaders
    glLinkProgram(m_ProgramID);
}