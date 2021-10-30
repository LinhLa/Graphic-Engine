#pragma once
#include <unordered_map>
#include <memory>
#include "creator.h"
#include "ShaderProgram.h"
#include "IPropertyMethod.h"
#include "IProperty.h"
#include "GLTexture.h"

extern const std::string ALBEDO_MAP;
extern const std::string NORMAL_MAP;
extern const std::string ROUGHNESS_MAP;
extern const std::string METALNESS_MAP;
extern const std::string SPECULAR_MAP;
extern const std::string HEIGHT_MAP;
extern const std::string AMBIENT_OCCLUSION_MAP;
extern const std::string OPACITY_MAP;
extern const std::string REFRACTION_MAP;
extern const std::string SELF_ILLUMINATION_MAP;

extern const std::string DIFFUSE_MAP;
extern const std::string BUMP_MAP;
extern const std::string REFLECTION_MAP;

class Material: public creator<Material>, public PropertyTable
{
protected:
	std::unordered_map<std::string, IPropertyPtr> m_propertyTable;
	std::unordered_map<std::string, GLTexturePtr> m_TextureList;
	const std::string m_name;

	Material(std::string name);
public:
	~Material();

	friend class creator<Material>;
public:
	void AddTexture(std::string, GLTexturePtr);
	GLTexturePtr GetTexture(std::string);
	std::vector<GLTexturePtr> GetTextureList();

	std::string getName() const;
	bool hasTextureMap(std::string);
};

typedef std::shared_ptr<Material> MaterialPtr;



