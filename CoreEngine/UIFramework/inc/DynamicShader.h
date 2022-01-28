#pragma once
#include <list>
#include <memory>
#include "ShaderProgram.h"

std::string load(const char* path);

class DynamicShader
{
private:
	std::list<std::string>	m_vertexSourceList;
	std::list<std::string>	m_fragmentSourceList;
	std::string m_name;

public:
	DynamicShader(std::string);
	~DynamicShader();

	void addVertexSource(std::initializer_list<std::string>);
	void addFragmentSource(std::initializer_list<std::string>);
	ShaderProgramPtr compile();
};