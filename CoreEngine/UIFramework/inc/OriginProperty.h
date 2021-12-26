#pragma once
#ifndef OPENGL_RENDERING
#include <SDL.h>
#else
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm/vec4.hpp>
#endif
#include <string>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"

enum ALIGNMENT_ENUM: int
{
	ALGIN_CENTER = 0,
	ALGIN_LEFT,
	ALGIN_RIGHT,
	ALGIN_TOP,
	ALGIN_BOTTOM,
};

extern const std::map<std::string, int> gMapAligment;

class OriginProperty final: public IPropertyMethod, public creator<OriginProperty>
{
private:
	PropertyTable* m_PropertyTable = nullptr;
protected:
	OriginProperty(PropertyTable*);
public:
	~OriginProperty();
	friend class creator<OriginProperty>;

	OriginProperty& operator=(const OriginProperty& rhs);
	void init() override;

	bool 	isVisible() const;
	void 	SetVisible(bool value);

	void SetClip(bool isClip);
	bool IsClip() const;

	void SetAngle(float);
	float GetAngle() const;

	void SetBroadCastEvent(bool);
	bool IsBroadCastEvent() const;

	void SetPivotPoint(glm::vec3 pivot);
	glm::vec3 GetPivotPoint() const;

	void SetFlip(SDL_RendererFlip);
	SDL_RendererFlip GetFlip() const;

	void SetAlignHorizontal(int);
	int GetAlignHorizontal();

	void SetAlignVerizontal(int);
	int GetAlignVerizontal();

#ifndef OPENGL_RENDERING
	uint8_t GetOpacity() const;
	void 	SetOpacity(uint8_t value);

	void SetCenterPoint(int x, int y);
	void SetCenterPoint(SDL_Point point);
	SDL_Point GetCenterPoint() const;

	void SetForeGroundColor(SDL_Color);
	SDL_Color GetForeGroundColor() const;

	void SetBackGroundColor(SDL_Color);
	SDL_Color GetBackGroundColor() const;
#else
	float OriginProperty::GetOpacity() const;
	void OriginProperty::SetOpacity(float value);

	void OriginProperty::SetCenterPoint(glm::vec2 point);
	glm::vec2 OriginProperty::GetCenterPoint() const;

	void SetForeGroundColor(glm::vec4);
	glm::vec4 GetForeGroundColor() const;

	void SetBackGroundColor(glm::vec4);
	glm::vec4 GetBackGroundColor() const;
#endif

#ifdef OPENGL_RENDERING
	static bool isValidColor(glm::vec4 color);
#else
	static bool isValidColor(SDL_Color color);
#endif
};

typedef std::shared_ptr<OriginProperty> OriginPropertyPtr;