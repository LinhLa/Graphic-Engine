#include "stdafx.h"
#include "Material.h"

//PBR texture map
const std::string ALBEDO_MAP = std::string("Albedo");
const std::string NORMAL_MAP = std::string("Normal");
const std::string ROUGHNESS_MAP = std::string("Roughness");
const std::string METALNESS_MAP = std::string("Metalness");
const std::string SPECULAR_MAP = std::string("Specular");
const std::string HEIGHT_MAP = std::string("Height");
const std::string AMBIENT_OCCLUSION_MAP = std::string("Ambient occlusion");
const std::string OPACITY_MAP = std::string("Opacity");
const std::string REFRACTION_MAP = std::string("Refraction");
const std::string SELF_ILLUMINATION_MAP = std::string("Self illumination");

//Non PBR texture map
const std::string DIFFUSE_MAP = std::string("Diffuse");
const std::string BUMP_MAP = std::string("Bump");
const std::string REFLECTION_MAP = std::string("Reflection");

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