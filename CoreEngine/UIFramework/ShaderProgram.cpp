#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& name):m_name(name)
{
	m_ProgramID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ProgramID);
}

void ShaderProgram::AttachShader(ShaderPtr pShader)
{
	if (pShader)
	{
		glAttachShader(m_ProgramID, pShader->GetShaderId());
	}
}

void ShaderProgram::LinkProgram()
{
	// link shaders
    glLinkProgram(m_ProgramID);
}