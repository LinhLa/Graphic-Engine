#pragma once
#include "creator.h"
#include <UIObject.h>

class NodeCamera final: public UIObject, public creator<NodeCamera>
{
protected:
	NodeCamera() = delete;
	NodeCamera(std::string name);
public:
	virtual ~NodeCamera();
	uint8_t getType() override;
	UIObjectPtr clone() override;
	friend class creator<NodeCamera>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);
};

typedef std::shared_ptr<NodeCamera> NodeCameraPtr;