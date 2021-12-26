#pragma once
#include <SDL.h>
#include <string>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"

class TextProperty final: public IPropertyMethod, public creator<TextProperty>
{
private:
	PropertyTable* m_PropertyTable = nullptr;
protected:
	TextProperty() = delete;
	TextProperty(PropertyTable*);
public:
	virtual ~TextProperty();
	friend class creator<TextProperty>;

	TextProperty& operator=(const TextProperty & rhs);

	template<typename ... Args>
	static std::shared_ptr<TextProperty> create(Args&& ... args)
	{
		return std::shared_ptr<TextProperty>(new TextProperty(std::forward<Args>(args)...));
	}

	void init() override;

	void SetText(const char*);
	std::string GetText() const;

	void SetFontName(const char*);
	std::string GetFontName() const;

	void SetFontSize(int32_t);
	int32_t GetFontSize() const;
#ifndef OPENGL_RENDERING 
	void SetColor(uint8_t, uint8_t, uint8_t, uint8_t);
	void SetColor(SDL_Color);
	SDL_Color GetColor() const;
#else
	void SetColor(glm::vec3);
	glm::vec3 GetColor() const;
#endif
	
};

typedef std::shared_ptr<TextProperty> TextPropertyPtr;