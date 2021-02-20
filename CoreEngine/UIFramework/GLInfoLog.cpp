#pragma once
#include "stdafx.h"
#include "GLInfoLog.h"
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
GLInfoLog::GLInfoLog()
{
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
}
GLInfoLog::~GLInfoLog(){}

const char* GLInfoLog::GetInfoLog() const
{
	return m_infoLog;
}