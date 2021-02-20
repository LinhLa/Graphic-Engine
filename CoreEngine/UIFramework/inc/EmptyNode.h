#pragma once
#include "creator.h"
#include <UIObject.h>
class EmptyNode final: public UIObject, public creator<EmptyNode>
{
protected:
	EmptyNode() = delete;
	EmptyNode(std::string name);
public:
	virtual ~EmptyNode();
	uint8_t getType() override;

	friend class creator<EmptyNode>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);
};

typedef std::shared_ptr<EmptyNode> EmptyNodePtr;