#pragma once
#include "stdafx.h"
#include "DynamicShader.h"

std::string load(const char* path)
{
	//<Open shader source file
	std::ifstream inShaderStream(path);

	//<Validate ifstream
	_ASSERT(inShaderStream);

	//<Read to string
	return std::string((std::istreambuf_iterator<char>(inShaderStream)), std::istreambuf_iterator<char>());
}

DynamicShader::DynamicShader(std::string name):m_name(name){}
DynamicShader::~DynamicShader() {}

void DynamicShader::addVertexSource(std::initializer_list<std::string> list)
{
	for (auto& source : list)
	{
		if (!source.empty())
		{
			m_vertexSourceList.push_back(source);
		}
	}
}

void DynamicShader::addFragmentSource(std::initializer_list<std::string> list)
{
	for (auto &source : list)
	{
		if (!source.empty())
		{
			m_fragmentSourceList.push_back(source);
		}
	}
	
}

ShaderProgramPtr DynamicShader::compile()
{
	auto pShaderProgram = Library::GetInstance()->get<ShaderProgram>(m_name);
	if (!pShaderProgram)
	{
		//Prepare lighting shader
		pShaderProgram = ShaderProgram::create(m_name);

		//<load vertex + fragment shader
		auto pFragmentShader = Shader::create(m_name + "Frag", GL_FRAGMENT_SHADER);
		auto pVertexShader = Shader::create(m_name + "Vex", GL_VERTEX_SHADER);

		//<load and attach source
		for (auto& src : m_fragmentSourceList)
		{
			pFragmentShader->addSource(src.c_str(), src.length());
		}

		for (auto& src : m_vertexSourceList)
		{
			pVertexShader->addSource(src.c_str(), src.length());
		}

		pFragmentShader->setSource();
		pVertexShader->setSource();

		//<Compile shader
		pFragmentShader->compile();
		pVertexShader->compile();

		//<Attach shader to program
		pShaderProgram->attachShader(pFragmentShader);
		pShaderProgram->attachShader(pVertexShader);

		//<Link to program
		pShaderProgram->linkProgram();

		//<Debug program
		pShaderProgram->debug();
	}
	return pShaderProgram;
}
