#pragma once
#include "stdafx.h"
#include <vector>
#include <fstream>
#include "ShaderProgram.h"
#include "GLInfoLog.h"
#include "log.h"
#include "Configuration.h"
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


uint32_t ShaderProgram::getID() const
{
	return m_ProgramID;
}

std::string ShaderProgram::getName() const
{
	return m_name;
}
