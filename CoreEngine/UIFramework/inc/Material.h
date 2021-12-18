#pragma once
#include <unordered_map>
#include <memory>
#include "creator.h"
#include "ShaderProgram.h"
#include "IPropertyMethod.h"
#include "IProperty.h"
#include "GLTexture.h"

extern const std::string LIGHT_COLOR;
extern const std::string LIGHT_AMBIENT;
extern const std::string LIGHT_DIFFUSE;
extern const std::string LIGHT_SPECULAR;
extern const std::string LIGHT_POSITION;
extern const std::string MATERIAL_AMBIENT;
extern const std::string MATERIAL_DIFFUSE;
extern const std::string MATERIAL_SPECULAR;
extern const std::string MATERIAL_DIFFUSE_MAP;
extern const std::string MATERIAL_SPECULAR_MAP;
extern const std::string MATERIAL_SHININESS;

class Material: public creator<Material>, public PropertyTable
{
protected:
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



