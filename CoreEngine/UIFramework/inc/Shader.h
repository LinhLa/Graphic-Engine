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
	GLuint  m_ShaderID  = 0;
protected:
	Shader(const std::string& name, GLenum type);
public:
	friend class creator<Shader>;
	~Shader();

	void setSource(GLsizei count, const GLchar* const* shaderSource, const GLint* length);
	void compile();

	GLuint  getId() const;
	std::string getName() const;
	GLenum type() const;
};

typedef std::shared_ptr<Shader> ShaderPtr;