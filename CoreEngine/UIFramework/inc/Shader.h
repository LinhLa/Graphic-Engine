#pragma once
#include <iostream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "creator.h"
#include <vector>
class Shader final: public creator<Shader>
{
private:
	std::string m_name;

	GLenum	m_Type = 0;
	GLuint  m_ShaderID  = 0;

	std::vector<char*>	m_shaderSource;
	std::vector<GLint>  m_lenght;
protected:
	Shader(const std::string& name, GLenum type);
public:
	friend class creator<Shader>;
	~Shader();

	void load(std::string);
	void addSource(const GLchar* shaderSource, GLint length);
	void setSource(/*GLsizei count, const GLchar* const* shaderSource, const GLint* length*/);
	void compile();

	GLuint  getId() const;
	std::string getName() const;
	GLenum type() const;
};

typedef std::shared_ptr<Shader> ShaderPtr;