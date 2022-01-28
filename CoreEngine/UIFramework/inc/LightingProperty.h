#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"
#include <Material.h>

std::string parse_name(GLint index, std::string property_name);

class PointLightProperty final : public IPropertyMethod, public creator<PointLightProperty>
{
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	PointLightProperty(PropertyTable*);
	friend class creator<PointLightProperty>;
public:	
	virtual ~PointLightProperty();

	void init();

	void  SetDiffuseColor(glm::vec4 color);
	glm::vec4  GetDiffuseColor() const;

	void  SetAmbientColor(glm::vec4 color);
	glm::vec4  GetAmbientColor() const;

	void  SetSpecularColor(glm::vec4 color);
	glm::vec4 GetSpecularColor() const;

	glm::vec3 GetPosition() const;

	void SetProperty(MaterialPtr, GLint);
};

typedef std::shared_ptr<PointLightProperty> PointLightPropertyPtr;


class SpotLightProperty final : public IPropertyMethod, public creator<SpotLightProperty>
{
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	SpotLightProperty(PropertyTable*);
	friend class creator<SpotLightProperty>;
public:
	virtual ~SpotLightProperty();

	void init();

	void  SetDiffuseColor(glm::vec4 color);
	glm::vec4  GetDiffuseColor() const;

	void  SetAmbientColor(glm::vec4 color);
	glm::vec4  GetAmbientColor() const;

	void  SetSpecularColor(glm::vec4 color);
	glm::vec4 GetSpecularColor() const;

	glm::vec3 GetPosition() const;
	glm::vec3 GetDirection() const;

	void SetProperty(MaterialPtr, GLint);
};

typedef std::shared_ptr<SpotLightProperty> SpotLightPropertyPtr;



class DirectionalLightProperty final : public IPropertyMethod, public creator<DirectionalLightProperty>
{
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	DirectionalLightProperty(PropertyTable*);
	friend class creator<DirectionalLightProperty>;
public:
	virtual ~DirectionalLightProperty();

	void init();

	void  SetDiffuseColor(glm::vec4 color);
	glm::vec4  GetDiffuseColor() const;

	void  SetAmbientColor(glm::vec4 color);
	glm::vec4  GetAmbientColor() const;

	void  SetSpecularColor(glm::vec4 color);
	glm::vec4 GetSpecularColor() const;

	glm::vec3 GetDirection() const;

	void SetProperty(MaterialPtr, GLint);
};

typedef std::shared_ptr<DirectionalLightProperty> DirectionalLightPropertyPtr;