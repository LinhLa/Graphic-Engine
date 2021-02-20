#pragma once
#include <vector>
#include "creator.h"
#include "Node2DImage.h"
#include "NodeText2D.h"
#include "Signal.h"

class NodeButton2D final: public UIObject, public creator<NodeButton2D>
{
public:
	enum BUTTON_SPIRE
	{
		BUTTON_SPRITE_NORMAL = 0,
		BUTTON_SPRITE_MOUSE_OUT,
		BUTTON_SPRITE_MOUSE_OVER_MOTION,
		BUTTON_SPRITE_MOUSE_DOWN,
		BUTTON_SPRITE_MOUSE_UP,
		BUTTON_SPRITE_TOTAL,
	};
private:
	NodeText2DPtr		m_pCaption = nullptr;
	UIObjectPtr			m_ArrayButtonState[BUTTON_SPRITE_TOTAL];
	BUTTON_SPIRE 		m_CurrentSprite = BUTTON_SPRITE_NORMAL;
protected:
	NodeButton2D(std::string);
public:
	virtual ~NodeButton2D();
	uint8_t getType() override;

	friend class creator<NodeButton2D>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);

	void setButtonSprite(BUTTON_SPIRE, Node2DImagePtr);
	void setButtonCaption(NodeText2DPtr);
};

typedef std::shared_ptr<NodeButton2D> NodeButton2DPtr;