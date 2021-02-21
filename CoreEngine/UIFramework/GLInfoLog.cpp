#pragma once
#include "stdafx.h"
#include "GLInfoLog.h"
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
GLInfoLog::GLInfoLog(uint32_t shader_id)
{
	glGetShaderInfoLog(shader_id, 512, NULL, m_infoLog);
}
GLInfoLog::~GLInfoLog(){}

const char* GLInfoLog::GetInfoLog() const
{
	return m_infoLog;
}