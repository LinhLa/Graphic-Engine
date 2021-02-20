#pragma once
#include <SDL.h>
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
	//<Gets the value of WidthProperty.More...

	void 	SetLayoutWidth(int value);
	//<Sets the value of WidthProperty.More...

	int 	GetLayoutHeight() const;
	//<Gets the value of HeightProperty.More...

	void 	SetLayoutHeight(int value);
	//<Sets the value of HeightProperty.More...

	void SetLayoutPosition(int x, int y);
	SDL_Point GetLayoutPosition() const;

	void SetLayoutScaleX(float);
	float GetLayoutScaleX() const;

	void SetLayoutScaleY(float);
	float GetLayoutScaleY() const;

	void SetLayoutInformation(const SDL_Rect& r);
	SDL_Rect GetLayoutInformation() const;

};

typedef std::shared_ptr<LayoutProperty> LayoutPropertyPtr;