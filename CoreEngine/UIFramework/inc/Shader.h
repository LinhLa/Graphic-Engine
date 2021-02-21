#pragma once
#include <iostream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "creator.h"
class Shader final: public creator<Shader>
{
private:
	std::string m_name;

	GLenum	m_Type = 0;
	uint32_t m_ShaderID  = 0;
protected:
public:
	Shader(const std::string& name, GLenum type);
	friend class creator<Shader>;
	~Shader();

	void ShaderSource(GLsizei count, const GLchar* const * shaderSource, const GLint *length);
	void CompileShader();

	uint32_t GetShaderId() const;
	GLenum Type() const;
};

typedef std::shared_ptr<Shader> ShaderPtr;