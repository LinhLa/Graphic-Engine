#pragma once
#include <SDL.h>
#include <unordered_map>
#include "Texture.h"
#include "IProperty.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <SingletonHolder.h>
class Library : public SingletonHolder<Library>
{
private:
	std::unordered_map<std::string, TexturePtr> 	m_textureTable;
	std::unordered_map<std::string, IPropertyPtr> 	m_propertyTable;
	std::unordered_map<std::string, ShaderPtr> 		m_shaderTable;
	std::unordered_map<std::string, ShaderProgramPtr> m_shaderProgramTable;
protected:
	Library();
	~Library();
public:
	void addTexture(const std::string&,TexturePtr);
	TexturePtr getTexture(const std::string&);

	void addProperty(const std::string&,IPropertyPtr);
	IPropertyPtr getProperty(const std::string&);

	void addShader(const std::string&,ShaderPtr);
	ShaderPtr getShader(const std::string&);

	void addShaderProgram(const std::string&, ShaderProgramPtr);
	ShaderProgramPtr getShaderProgram(const std::string&);

	friend class SingletonHolder<Library>;
};