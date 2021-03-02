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

	void attachShader(ShaderPtr);
	void linkProgram();
	void useProgram();

	void read();
	void write();

	uint32_t getID() const;
	std::string getName() const;
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;