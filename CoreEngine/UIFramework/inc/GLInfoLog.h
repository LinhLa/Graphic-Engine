#pragma once
#include <iostream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

class GLInfoLog
{
private:
	GLint		m_success = 0;
	GLenum		m_type = 0;
	GLuint		m_id = 0;
	static char m_infoLog[512];
public:
	GLInfoLog(GLuint id, GLenum type);
	~GLInfoLog();
	static const char* GetInfoLog();
	bool CheckGLSuccess();
};
