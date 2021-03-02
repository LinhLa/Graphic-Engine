#include "stdafx.h"
#include "Library.h"
#include "log.h"

Library::Library(){}

Library::~Library(){}

void Library::addTexture(const std::string& name,TexturePtr pTexture)
{
	if (pTexture)
	{
		m_textureTable[name] = pTexture;
	}
}

TexturePtr Library::getTexture(const std::string& name)
{
	auto itr_result = m_textureTable.find(name);
	if (itr_result != m_textureTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Texture not found [%s]", name.c_str());
	}
	return nullptr;
}

void Library::addProperty(const std::string& name, IPropertyPtr pProperty)
{
	if (pProperty)
	{
		m_propertyTable[name] = pProperty;
	}
}

IPropertyPtr Library::getProperty(const std::string& name)
{
	auto itr_result = m_propertyTable.find(name);
	if (itr_result != m_propertyTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Property not found [%s]", name.c_str());
	}
	return nullptr;
}

void Library::addShader(const std::string& name, ShaderPtr pShader)
{
	if (pShader)
	{
		m_shaderTable[name] = pShader;
	}
}

ShaderPtr Library::getShader(const std::string& name)
{
	auto itr_result = m_shaderTable.find(name);
	if (itr_result != m_shaderTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Shader not found [%s]", name.c_str());
	}
	return nullptr;
}

void Library::addShaderProgram(const std::string& name, ShaderProgramPtr pProgramShader)
{
	if (pProgramShader)
	{
		m_shaderProgramTable[name] = pProgramShader;
	}
}

ShaderProgramPtr Library::getShaderProgram(const std::string& name)
{
	auto itr_result = m_shaderProgramTable.find(name);
	if (itr_result != m_shaderProgramTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Shader program not found [%s]", name.c_str());
	}
	return nullptr;
}

void Library::addVertexData(const std::string& name, VertexDataPtr pData)
{
	if (pData)
	{
		m_vertexDataTable[name] = pData;
	}
}

VertexDataPtr Library::getVertexData(const std::string& name)
{
	auto itr_result = m_vertexDataTable.find(name);
	if (itr_result != m_vertexDataTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Vertex data not found [%s]", name.c_str());
	}
	return nullptr;
}
