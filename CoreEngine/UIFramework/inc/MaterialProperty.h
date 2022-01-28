#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"

class MaterialProperty final: public IPropertyMethod, public creator<MaterialProperty>
{
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	MaterialProperty(PropertyTable*);
public:
	~MaterialProperty();
	friend class creator<MaterialProperty>;

	MaterialProperty& operator=(const MaterialProperty& rhs) = delete;
	void init() override;


	void  SetDiffuseColor(glm::vec4 color);
	glm::vec4  GetDiffuseColor() const;

	void  SetAmbientColor(glm::vec4 color);
	glm::vec4  GetAmbientColor() const;

	void  SetSpecularColor(glm::vec4 color);
	glm::vec4 GetSpecularColor() const;

	void  SetDiffuseMap(glm::u32 texture);
	glm::u32  GetDiffuseMap() const;


	void  SetSpecularMap(glm::u32 texture);

	glm::u32  GetSpecularMap() const;

	void  SetShininess(float value);
	float  GetShininess() const;
};

typedef std::shared_ptr<MaterialProperty> MaterialPropertyPtr;