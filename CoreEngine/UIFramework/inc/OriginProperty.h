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

	void SetAngle(double);
	double GetAngle() const;

	void SetBroadCastEvent(bool);
	bool IsBroadCastEvent() const;

	uint8_t GetOpacity() const;
	void 	SetOpacity(uint8_t value);

	void SetCenterPoint(int x, int y);
	void SetCenterPoint(SDL_Point point);
	SDL_Point GetCenterPoint() const;

	void SetFlip(SDL_RendererFlip);
	SDL_RendererFlip GetFlip() const;

	void SetForeGroundColor(SDL_Color);
	SDL_Color GetForeGroundColor() const;

	void SetBackGroundColor(SDL_Color);
	SDL_Color GetBackGroundColor() const;
};

typedef std::shared_ptr<OriginProperty> OriginPropertyPtr;