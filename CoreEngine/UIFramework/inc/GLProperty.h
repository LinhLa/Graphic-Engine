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

class GLProperty final: public IPropertyMethod ,public creator<GLProperty>
{
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	GLProperty(PropertyTable*);
public:
	~GLProperty();
	friend class creator<GLProperty>;

	GLProperty& operator=(const GLProperty& rhs) = delete;
	void init() override;

	float 	GetBlendIndensity() const;
	void 	SetBlendIndensity(float value);

	glm::i32vec4 GetViewPort() const;
	void SetViewPort(glm::i32vec4 value);
};

typedef std::shared_ptr<GLProperty> GLPropertyPtr;