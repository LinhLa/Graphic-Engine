#pragma once
#include <UIObject.h>
#include "GLTexture.h"
#include <Texture.h>
#include "ShaderProgram.h"
class Node2DImage final: public UIObject, public creator<Node2DImage>
{
private:
#ifdef OPENGL_RENDERING
	GLTexturePtr		m_pTexture = nullptr;
#else
	TexturePtr 	m_pTexture = nullptr;
	TexturePtr 	m_pTextureToRender = nullptr;
#endif
	bool		m_bUIChanged = true;
protected:
	Node2DImage(std::string name);
public:
	virtual ~Node2DImage();
	uint8_t getType() override;
	UIObjectPtr clone() override;
	friend class creator<Node2DImage>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);

	int GetActualWidth() const;
	int GetActualHeight() const;

	void SetTexture(const std::string&);

};

typedef std::shared_ptr<Node2DImage> Node2DImagePtr;