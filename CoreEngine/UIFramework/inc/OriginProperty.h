#pragma once
#include <SDL.h>
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
	//<Gets the value of VisibleProperty.More...

	void 	SetVisible(bool value);
	//<Sets the value of VisibleProperty.More...

	uint8_t GetOpacity() const;
	//<Gets the value of OpacityProperty.More...

	void 	SetOpacity(uint8_t value);
	//<Sets the value of OpacityProperty.More...

	void SetClip(bool isClip);
	bool IsClip() const;

	void SetAngle(double);
	double GetAngle() const;

	void SetCenterPoint(int x, int y);
	void SetCenterPoint(SDL_Point point);
	SDL_Point GetCenterPoint() const;

	void SetFlip(SDL_RendererFlip);
	SDL_RendererFlip GetFlip() const;

	void SetBroadCastEvent(bool);
	bool IsBroadCastEvent() const;

	void SetForeGroundColor(SDL_Color);
	SDL_Color GetForeGroundColor() const;

	void SetBackGroundColor(SDL_Color);
	SDL_Color GetBackGroundColor() const;
};

typedef std::shared_ptr<OriginProperty> OriginPropertyPtr;