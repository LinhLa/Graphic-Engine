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
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	LayoutProperty(PropertyTable*);
public:
	~LayoutProperty();
	friend class creator<LayoutProperty>;

	LayoutProperty& operator=(const LayoutProperty& rhs) = delete;
	void init() override;

#ifdef OPENGL_RENDERING
	void onMoveX(float&& x);
	void onMoveY(float&& y);

	void onScaleX(float&& x);
	void onScaleY(float&& y);

	void SetLayoutSize(glm::vec2);
	glm::vec2 GetLayoutSize() const;

	void SetLayoutTransform(glm::vec3 pos);
	glm::vec3 GetLayoutTransform() const;

	void SetRotation(glm::vec3);
	glm::vec3 GetRotation() const;

	void SetLayoutScale(glm::vec3);
	glm::vec3 GetLayoutScale() const;
#else

	void SetLayoutScaleX(float);
	float GetLayoutScaleX() const;

	void SetLayoutScaleY(float);
	float GetLayoutScaleY() const;

	int 	GetLayoutWidth() const;
	void 	SetLayoutWidth(int value);

	int 	GetLayoutHeight() const;
	void 	SetLayoutHeight(int value);

	void SetLayoutPosition(int x, int y);
	SDL_Point GetLayoutPosition() const;

	void SetLayoutInformation(const SDL_Rect& r);
	SDL_Rect GetLayoutInformation() const;
#endif

};

typedef std::shared_ptr<LayoutProperty> LayoutPropertyPtr;