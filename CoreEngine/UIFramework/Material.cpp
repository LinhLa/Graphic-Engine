#include "stdafx.h"
#include "Material.h"

const std::string LIGHT_COLOR = std::string("light.color");
const std::string LIGHT_AMBIENT = std::string("light.ambient");
const std::string LIGHT_DIFFUSE = std::string("light.diffuse");
const std::string LIGHT_SPECULAR = std::string("light.specular");
const std::string LIGHT_POSITION = std::string("light.position");
const std::string MATERIAL_AMBIENT = std::string("material.ambient");
const std::string MATERIAL_DIFFUSE = std::string("material.diffuse");
const std::string MATERIAL_SPECULAR = std::string("material.specular");
const std::string MATERIAL_DIFFUSE_MAP = std::string("material.diffuseMap");
const std::string MATERIAL_SPECULAR_MAP = std::string("material.specularMap");
const std::string MATERIAL_SHININESS = std::string("material.shininess");

Material::Material(std::string name):m_name(name)
{
}

Material::~Material(){}

std::string Material::getName() const
{
	return m_name;
}

void Material::AddTexture(std::string texture_map, GLTexturePtr pTexture)
{
	if ((nullptr != pTexture) && (m_TextureList.find(texture_map) == m_TextureList.end()))
	{
		m_TextureList[texture_map] = pTexture;
	}
}

GLTexturePtr Material::GetTexture(std::string texture_map)
{
	GLTexturePtr pTexture = nullptr;
	auto itr = m_TextureList.find(texture_map);
	if (itr != m_TextureList.end())
	{
		pTexture = itr->second;
	}
	return pTexture;
}
std::vector<GLTexturePtr> Material::GetTextureList()
{
	std::vector<GLTexturePtr> list;
	for(auto &item : m_TextureList)
	{
		list.push_back(item.second);
	}
	return list;
}

bool Material::hasTextureMap(std::string texture_map)
{
	bool bResult = false;
	auto itr = m_TextureList.find(texture_map);
	if((m_TextureList.end() != itr) && (itr->second != nullptr))
	{
		bResult = true;
	}
	return bResult;
}