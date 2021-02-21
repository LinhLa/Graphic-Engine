#include "stdafx.h"
#include "Library.h"
#include "log.h"

Library::Library(){}

Library::~Library(){}

void Library::addTexture(const std::string& texture_name,TexturePtr pTexture)
{
	if (pTexture)
	{
		m_textureTable[texture_name] = pTexture;
	}
}

TexturePtr Library::getTexture(const std::string& texture_name)
{
	auto itr_result = m_textureTable.find(texture_name);
	if (itr_result != m_textureTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Texture not found [%s]", texture_name.c_str());
	}
	return nullptr;
}

void Library::addProperty(const std::string& property_name, IPropertyPtr pProperty)
{
	if (pProperty)
	{
		m_propertyTable[property_name] = pProperty;
	}
}

IPropertyPtr Library::getProperty(const std::string& property_name)
{
	auto itr_result = m_propertyTable.find(property_name);
	if (itr_result != m_propertyTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Property not found [%s]", property_name.c_str());
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

ShaderPtr Library::getShader(const std::string& shader_name)
{
	auto itr_result = m_shaderTable.find(shader_name);
	if (itr_result != m_shaderTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Shader not found [%s]", shader_name.c_str());
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

ShaderProgramPtr Library::getShaderProgram(const std::string& program_name)
{
	auto itr_result = m_shaderProgramTable.find(program_name);
	if (itr_result != m_shaderProgramTable.end())
	{
		return itr_result->second;
	}
	else
	{
		LOG_DEBUG("Shader not found [%s]", program_name.c_str());
	}
	return nullptr;
}