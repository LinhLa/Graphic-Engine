#pragma once
#include <unordered_map>
#include <memory>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"
#include "GLTexture.h"

extern const std::string SPOT_LIGHT_AMBIENT;
extern const std::string SPOT_LIGHT_DIFFUSE;
extern const std::string SPOT_LIGHT_SPECULAR;
extern const std::string SPOT_LIGHT_POSITION;
extern const std::string SPOT_LIGHT_CUTOFF;
extern const std::string SPOT_LIGHT_INNER_CUTOFF;
extern const std::string SPOT_LIGHT_OUTER_CUTOFF;
extern const std::string SPOT_LIGHT_DIRECTION;

extern const std::string DIRECTIONAL_LIGHT_POSITION;
extern const std::string DIRECTIONAL_LIGHT_COLOR;
extern const std::string DIRECTIONAL_LIGHT_AMBIENT;
extern const std::string DIRECTIONAL_LIGHT_DIFFUSE;
extern const std::string DIRECTIONAL_LIGHT_SPECULAR;
extern const std::string DIRECTIONAL_LIGHT_DIRECTION;

extern const std::string POINT_LIGHT_COLOR;
extern const std::string POINT_LIGHT_AMBIENT;
extern const std::string POINT_LIGHT_DIFFUSE;
extern const std::string POINT_LIGHT_SPECULAR;
extern const std::string POINT_LIGHT_POSITION;
extern const std::string POINT_LIGHT_CONSTANT;
extern const std::string POINT_LIGHT_LINEAR;
extern const std::string POINT_LIGHT_QUADRATIC;

extern const std::string CAST_SHADOW;
extern const std::string LIGHT_DEPTH_MAP;
extern const std::string LIGHT_SPACE_MAXTRIX;
extern const std::string LIGHT_TYPE;

extern const std::string MATERIAL_MODEL;
extern const std::string MATERIAL_OPACITY;
extern const std::string MATERIAL_TRANSPARENCY_FACTOR;
extern const std::string MATERIAL_BUMPSCALING;
extern const std::string MATERIAL_SHININESS;
extern const std::string MATERIAL_REFECTIVITY;
extern const std::string MATERIAL_SHININESS_STRENGTH;
extern const std::string MATERIAL_REFRACTION;
extern const std::string MATERIAL_EMISSIVE;
extern const std::string MATERIAL_AMBIENT;
extern const std::string MATERIAL_DIFFUSE;
extern const std::string MATERIAL_SPECULAR;

extern const std::string MATERIAL_SHININESS;
extern const std::string MATERIAL_TRANSPARENT;
extern const std::string MATERIAL_REFLECTIVE;

extern const std::string MATERIAL_SHININESS_MAP;
extern const std::string MATERIAL_HEIGHT_MAP;
extern const std::string MATERIAL_DIFFUSE_MAP;
extern const std::string MATERIAL_ROUGHNESS_MAP;
extern const std::string MATERIAL_DISPLACEMENT_MAP;
extern const std::string MATERIAL_NORMAL_MAP;
extern const std::string MATERIAL_SPECULAR_MAP;

extern const std::string MATERIAL_OPACITY_MAP;
extern const std::string MATERIAL_LIGHT_MAP;
extern const std::string MATERIAL_REFLECTION_MAP;
extern const std::string MATERIAL_BASECOLOR_MAP;
extern const std::string MATERIAL_NORMAL_CAM_MAP;
extern const std::string MATERIAL_EMISSION_COLOR_MAP;
extern const std::string MATERIAL_METALNESS_MAP;
extern const std::string MATERIAL_AMBIENT_OCCLUSION_MAP;

extern const std::string MATERIAL_BLENDING_MODE;

extern const std::string SHADER_DEFAULT;
extern const std::string MATERIAL_DEFAULT;

extern const std::string TEXTURE_DIFFUSE_DEFAULT;
extern const std::string TEXTURE_SPECULAR_DEFAULT;
extern const std::string TEXTURE_NORMAL_DEFAULT;

extern const std::map<std::string, glm::i32> gLightMap;

enum LIGHT_TYPE_ENUM :glm::i32
{
	UNDEFINE,
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT,
};

#define LIGHTING_SHADER			"LightShading"
#define MULTILIGHT_SHADER		"MultiLight"
#define POSTPROCESSING_SHADER	"PostProcessing"

class Material: public creator<Material>, public PropertyTable, public PropertyMethodTable, public std::enable_shared_from_this<Material>
{
protected:
	std::unordered_map<std::string, std::vector<GLTexturePtr>> m_TextureList;
	const std::string	m_name;
	std::string			m_shader = std::string(SHADER_DEFAULT);

	Material(std::string name);
public:
	~Material();

	friend class creator<Material>;
public:
	void AddTexture(std::string, GLTexturePtr);
	GLTexturePtr GetTexture(std::string texture_map);
	std::vector<GLTexturePtr> GetTextureList(std::string texture_map);

	std::string getName() const;
	bool hasTextureMap(std::string);

	void setShader(std::string);
	std::string getShader() const;

	std::shared_ptr<Material> clone();
};

typedef std::shared_ptr<Material> MaterialPtr;



