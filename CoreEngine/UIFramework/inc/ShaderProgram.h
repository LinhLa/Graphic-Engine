#pragma once
#include <iostream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <map>
class ShaderProgram
{
private:
	uint32_t m_ProgramID  = 0;
	std::map<uint32_t, const char*> m_shaderMap;
protected:
	ShaderProgram();
	~ShaderProgram();
	void ShaderSource(GLuint shader,
	 	GLsizei count,
	 	const GLchar * const *string,
	 	const GLint *length);
	void CompileShader();
	void AttachShader();
	void LinkProgram();

};
