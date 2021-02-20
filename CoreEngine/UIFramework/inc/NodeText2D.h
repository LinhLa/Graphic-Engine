#pragma once
#include "creator.h"
#include <UIObject.h>
#include <Font.h>
#include "Texture.h"
class NodeText2D final: public UIObject, public creator<NodeText2D>
{
private:
	FontPtr	m_pFont = nullptr;
	bool	m_bFontChanged = false;
	bool	m_bUIChanged = false;

	void UpdateFont();
protected:
	NodeText2D() = delete;
	NodeText2D(std::string);
public:
	virtual ~NodeText2D();
	uint8_t getType() override;

	friend class creator<NodeText2D>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);
};

typedef std::shared_ptr<NodeText2D> NodeText2DPtr;