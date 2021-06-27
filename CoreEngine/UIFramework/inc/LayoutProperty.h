#pragma once
#ifndef OPENGL_RENDERING
#include <SDL.h>
#else
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm/vec3.hpp>
#endif
#include <string>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"

class LayoutProperty final: public IPropertyMethod, public creator<LayoutProperty>
{
private:
	PropertyTable* m_PropertyTable = nullptr;
protected:
	LayoutProperty(PropertyTable*);
public:
	~LayoutProperty();
	friend class creator<LayoutProperty>;

	LayoutProperty& operator=(const LayoutProperty& rhs);
	void init() override;

	int 	GetLayoutWidth() const;
	void 	SetLayoutWidth(int value);

	int 	GetLayoutHeight() const;
	void 	SetLayoutHeight(int value);

	void SetLayoutScaleX(float);
	float GetLayoutScaleX() const;

	void SetLayoutScaleY(float);
	float GetLayoutScaleY() const;

	void SetLayoutPosition(int x, int y);
	SDL_Point GetLayoutPosition() const;

	void SetLayoutInformation(const SDL_Rect& r);
	SDL_Rect GetLayoutInformation() const;

	void SetLayoutTransform(glm::vec3 pos);
	glm::vec3 GetLayoutTransform() const;

	void SetRotation(glm::vec3);
	glm::vec3 GetRotation() const;

	void SetLayoutScale(glm::vec3);
	glm::vec3 GetLayoutScale() const;
};

typedef std::shared_ptr<LayoutProperty> LayoutPropertyPtr;