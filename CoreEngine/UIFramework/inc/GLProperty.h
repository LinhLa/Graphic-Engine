#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"
#include "ShaderProgram.h"
#include "GLTexture.h"
class GLProperty final: public IPropertyMethod, public creator<GLProperty>
{
private:
	PropertyTable* m_PropertyTable = nullptr;
protected:
	GLProperty(PropertyTable*);
	std::vector<GLTexturePtr> m_TextureList;
public:
	~GLProperty();
	friend class creator<GLProperty>;

	GLProperty& operator=(const GLProperty& rhs);
	void init() override;

	void AddTexture(GLTexturePtr);
	std::vector<GLTexturePtr> GetTextureList();

	float 	GetBlendIndensity() const;
	void 	SetBlendIndensity(float value);
};

typedef std::shared_ptr<GLProperty> GLPropertyPtr;