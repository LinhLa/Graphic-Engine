#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"
#include "ShaderProgram.h"
#include "GLTexture.h"

class MaterialProperty final: public IPropertyMethod, public creator<MaterialProperty>
{
private:
	PropertyTable* m_PropertyTable = nullptr;
protected:
	MaterialProperty(PropertyTable*);
public:
	~MaterialProperty();
	friend class creator<MaterialProperty>;

	MaterialProperty& operator=(const MaterialProperty& rhs);
	void init() override;

};

typedef std::shared_ptr<MaterialProperty> MaterialPropertyPtr;