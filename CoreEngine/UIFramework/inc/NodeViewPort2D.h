#pragma once
#include "creator.h"
#include <UIObject.h>
#include <memory>
#include <Texture.h>

class NodeViewPort2D final: public UIObject, public creator<NodeViewPort2D>
{
private:
	TexturePtr m_pTexture = nullptr;
protected:
	NodeViewPort2D() = delete;
	NodeViewPort2D(std::string name);
public:
	virtual ~NodeViewPort2D();
	uint8_t getType() override;

	friend class creator<NodeViewPort2D>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);
};

typedef std::shared_ptr<NodeViewPort2D> NodeViewPort2DPtr;