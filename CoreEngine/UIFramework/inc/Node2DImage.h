#pragma once
#include <UIObject.h>
#include <Texture.h>
class Node2DImage final: public UIObject, public creator<Node2DImage>
{
private:
	TexturePtr 	m_pTexture = nullptr;
	TexturePtr 	m_pTextureToRender = nullptr;
	bool		m_bUIChanged = false;
protected:
	Node2DImage(std::string name);
public:
	virtual ~Node2DImage();
	uint8_t getType() override;

	friend class creator<Node2DImage>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);

	int GetActualWidth() const;
	int GetActualHeight() const;

	void SetTexture(const std::string&);

};

typedef std::shared_ptr<Node2DImage> Node2DImagePtr;