#include "stdafx.h"
#include "Material.h"
#include "MaterialProperty.h"

const std::string SPOT_LIGHT_AMBIENT = std::string("spotlight.ambient");
const std::string SPOT_LIGHT_DIFFUSE = std::string("spotlight.diffuse");
const std::string SPOT_LIGHT_SPECULAR = std::string("spotlight.specular");
const std::string SPOT_LIGHT_POSITION = std::string("spotlight.position");
const std::string SPOT_LIGHT_DIRECTION = std::string("spotlight.direction");
const std::string SPOT_LIGHT_CUTOFF = std::string("spotlight.cutOff");
const std::string SPOT_LIGHT_INNER_CUTOFF = std::string("spotlight.innercutOff");
const std::string SPOT_LIGHT_OUTER_CUTOFF = std::string("spotlight.outerCutOff");

const std::string DIRECTIONAL_LIGHT_COLOR = std::string("directionallight.color");
const std::string DIRECTIONAL_LIGHT_AMBIENT = std::string("directionallight.ambient");
const std::string DIRECTIONAL_LIGHT_DIFFUSE = std::string("directionallight.diffuse");
const std::string DIRECTIONAL_LIGHT_SPECULAR = std::string("directionallight.specular");
const std::string DIRECTIONAL_LIGHT_DIRECTION = std::string("directionallight.direction");

const std::string POINT_LIGHT_COLOR = std::string("pointlight.color");
const std::string POINT_LIGHT_AMBIENT = std::string("pointlight.ambient");
const std::string POINT_LIGHT_DIFFUSE = std::string("pointlight.diffuse");
const std::string POINT_LIGHT_SPECULAR = std::string("pointlight.specular");
const std::string POINT_LIGHT_POSITION = std::string("pointlight.position");
const std::string POINT_LIGHT_CONSTANT = std::string("pointlight.constant");
const std::string POINT_LIGHT_LINEAR = std::string("pointlight.linear");
const std::string POINT_LIGHT_QUADRATIC = std::string("pointlight.quadratic");

const std::string LIGHT_TYPE = std::string("light.type");

const std::string MATERIAL_MODEL = std::string("material.shadingModel");

const std::string MATERIAL_OPACITY = std::string("material.opacity");
const std::string MATERIAL_TRANSPARENCY_FACTOR = std::string("material.transparencyFactor");
const std::string MATERIAL_BUMPSCALING = std::string("material.bumpscaling");
const std::string MATERIAL_REFECTIVITY = std::string("material.reflectivity");
const std::string MATERIAL_SHININESS_STRENGTH = std::string("material.shininessStrength");
const std::string MATERIAL_REFRACTION = std::string("material.refraction");

const std::string MATERIAL_EMISSIVE = std::string("material.emissive");
const std::string MATERIAL_AMBIENT = std::string("material.ambient");
const std::string MATERIAL_DIFFUSE = std::string("material.diffuse");
const std::string MATERIAL_SPECULAR = std::string("material.specular");

const std::string MATERIAL_HEIGHT_MAP = std::string("material.heightMap");
const std::string MATERIAL_DIFFUSE_MAP = std::string("material.diffuseMap");
const std::string MATERIAL_ROUGHNESS_MAP = std::string("material.rounghnessMap");
const std::string MATERIAL_DISPLACEMENT_MAP = std::string("material.displacementMap");
const std::string MATERIAL_NORMAL_MAP = std::string("material.normalMap");

const std::string MATERIAL_SPECULAR_MAP = std::string("material.specularMap");
const std::string MATERIAL_SHININESS = std::string("material.shininess");
const std::string MATERIAL_SHININESS_MAP = std::string("material.shininessMap");
const std::string MATERIAL_TRANSPARENT = std::string("material.transparent");
const std::string MATERIAL_REFLECTIVE = std::string("material.reflective");

const std::string MATERIAL_OPACITY_MAP = std::string("material.opacityMap");
const std::string MATERIAL_LIGHT_MAP = std::string("material.lightMap");
const std::string MATERIAL_REFLECTION_MAP = std::string("material.reflectMap");
const std::string MATERIAL_BASECOLOR_MAP = std::string("material.baseColorMap");
const std::string MATERIAL_NORMAL_CAM_MAP = std::string("material.normalCamMap");
const std::string MATERIAL_EMISSION_COLOR_MAP = std::string("material.emissionColorMap");
const std::string MATERIAL_METALNESS_MAP = std::string("material.metalnessMap");
const std::string MATERIAL_AMBIENT_OCCLUSION_MAP = std::string("material.ambientOcclusionMap");

const std::string MATERIAL_BLENDING_MODE = std::string("blendmode");

const std::string SHADER_DEFAULT = std::string("LightShading");
const std::string MATERIAL_DEFAULT = std::string("LightShading");

const std::string TEXTURE_DIFFUSE_DEFAULT = std::string("default_diffuse");
const std::string TEXTURE_SPECULAR_DEFAULT = std::string("default_specular");
const std::string TEXTURE_NORMAL_DEFAULT = std::string("default_normal");

const std::map<std::string, glm::i32> gLightMap =
{
	{"point", POINT_LIGHT},
	{"directional", DIRECTIONAL_LIGHT},
	{"spot", SPOT_LIGHT}
};

Material::Material(std::string name):m_name(name)
{
	AddPropertyMethodObj(MaterialProperty::create(this));
}

Material::~Material(){}

std::string Material::getName() const
{
	return m_name;
}

void Material::AddTexture(std::string texture_map, GLTexturePtr pTexture)
{
	if (nullptr != pTexture)
	{
		m_TextureList[texture_map].push_back(pTexture);
	}
}

GLTexturePtr Material::GetTexture(std::string texture_map)
{
	GLTexturePtr pResult = nullptr;
	auto itr = m_TextureList.find(texture_map);
	if ((m_TextureList.end() != itr) && (itr->second.size() > 0))
	{
		//Material may have many for each texture map, only take the 1st one
		pResult = *(itr->second.begin());
	}
	return pResult;
}

std::vector<GLTexturePtr> Material::GetTextureList(std::string texture_map)
{
	return m_TextureList[texture_map];
}

bool Material::hasTextureMap(std::string texture_map)
{
	bool bResult = false;
	auto itr = m_TextureList.find(texture_map);
	if((m_TextureList.end() != itr) && (itr->second.size() > 0))
	{
		bResult = true;
	}
	return bResult;
}

void Material::setShader(std::string shader)
{
	m_shader = shader;
}

std::string Material::getShader() const
{
	return m_shader;
}

MaterialPtr Material::clone()
{
	auto pMaterial = Material::create(m_name.c_str());
	pMaterial->m_shader = this->m_shader;
	pMaterial->m_TextureList = this->m_TextureList;
	this->setProperty(pMaterial);
	return pMaterial;
}