#pragma once
#include "stdafx.h"
#include "GLInfoLog.h"
#include "log.h"

char GLInfoLog::m_infoLog[512];

GLInfoLog::GLInfoLog(GLuint id, GLenum type):m_id(id),m_type(type)
{
	switch (type)
	{
	case GL_LINK_STATUS:
		glGetProgramiv(m_id, m_type, &m_success);
		break;
	case GL_COMPILE_STATUS:
		glGetShaderiv(m_id, m_type, &m_success);
		break;
	default:
		_ASSERT(false);
	}
}

GLInfoLog::~GLInfoLog(){}

const char* GLInfoLog::GetInfoLog()
{
	return m_infoLog;
}

bool GLInfoLog::CheckGLSuccess()
{
	if (!m_success)
	{
		switch(m_type)
		{
			case GL_COMPILE_STATUS:
			glGetShaderInfoLog(m_id, 512, NULL, m_infoLog);
			break;
			case GL_LINK_STATUS:
			glGetProgramInfoLog(m_id, 512, NULL, m_infoLog);
			break;
			default:
			break;
		}
	}
	return (0 != m_success);
}