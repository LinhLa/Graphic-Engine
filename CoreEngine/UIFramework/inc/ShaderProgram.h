#pragma once
#include <iostream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "Shader.h"
#include "creator.h"

class ShaderProgram final: public creator<ShaderProgram>
{
private:
	std::string m_name;
	uint32_t 	m_ProgramID  = 0;
	ShaderProgram(const std::string&);
public:
	friend class creator<ShaderProgram>;
	~ShaderProgram();
	void AttachShader(ShaderPtr);
	void LinkProgram();
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;