#pragma once
#include <SDL.h>
#include <string>


class Property
{
protected:
	SDL_Rect m_position;
	SDL_Rect m_inner_position;

	Property();
	virtual ~Property();

	bool m_bVisible = false;
	bool m_bLoadToGPU = false;

	uint8_t m_uAlpha = 0;

	double           m_angle = 0.0F;
	SDL_Point        m_centerPoint = {0};
	SDL_RendererFlip m_flip = SDL_RendererFlip::SDL_FLIP_NONE;

public:
	int 	getLayoutWidth() const;
	//<Gets the value of WidthProperty.More...

	void 	setLayoutWidth(int value);
	//<Sets the value of WidthProperty.More...

	int 	getLayoutHeight() const;
	//<Gets the value of HeightProperty.More...

	void 	setLayoutHeight(int value);
	//<Sets the value of HeightProperty.More...

	int 	getActualWidth() const;
	//<Gets the value of SourceWidthProperty.More...

	void 	setActualWidth(int value);
	//<Sets the value of SourceWidthProperty.More...

	int 	getActualHeight() const;
	//<Gets the value of SourceHeightProperty.More...

	void 	setActualHeight(int value);
	//<Sets the value of SourceHeightProperty.More...

	bool 	isVisible() const;
	//<Gets the value of VisibleProperty.More...

	void 	setVisible(bool value);
	//<Sets the value of VisibleProperty.More...

	uint8_t 	getOpacity() const;
	//<Gets the value of OpacityProperty.More...

	void 	setOpacity(uint8_t value);
	//<Sets the value of OpacityProperty.More...

	void SetLayoutPosition(int x, int y);
	SDL_Point GetLayoutPosition()  const;

	SDL_Rect GetActualTransformation()  const;
	SDL_Rect GetLayoutTransformation()  const;

	void SetActualTransformation(SDL_Rect&);
	void SetLayoutTransformation(SDL_Rect&);

	void SetLoadToGPU(bool isload);
	bool IsLoadToGPU() const;

	inline	void SetAngle(double);
	double GetAngle() const;

	void SetCenterPoint(int x, int y);
	SDL_Point GetCenterPoint() const;

	void SetFlip(SDL_RendererFlip);
	SDL_RendererFlip GetFlip() const;
};

class TextProperty
{
protected:
	std::string m_text;
	std::string m_font_name = std::string("arial.ttf");
	int32_t		m_point_size = 10;
	SDL_Color	m_fore_ground_color = { 255, 255, 255 };
public:

	TextProperty();
	~TextProperty();

	void SetText(const char*);
	std::string GetText() const;

	void SetFontName(const char*);
	std::string GetFontName() const;

	void SetFontSize(int32_t);
	int32_t GetFontSize() const;

	void SetColor(uint8_t, uint8_t, uint8_t, uint8_t);
	SDL_Color GetColor() const;
};

class NodeResourceProperty
{
protected:
	NodeResourceProperty();
	virtual ~NodeResourceProperty();

	std::string m_resource_name;
public:

	std::string GetResourceName() const;
	void SetResourceName(const char*);
};
