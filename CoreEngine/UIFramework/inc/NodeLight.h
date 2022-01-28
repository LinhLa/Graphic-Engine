#pragma once
#include "creator.h"
#include <UIObject.h>

class NodePointLight final: public UIObject, public creator<NodePointLight>
{
protected:
	NodePointLight() = delete;
	NodePointLight(std::string name);
public:
	virtual ~NodePointLight();
	uint8_t getType() override;
	UIObjectPtr clone() override;
	friend class creator<NodePointLight>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);
};

typedef std::shared_ptr<NodePointLight> NodePointLightPtr;


class NodeSpotLight final : public UIObject, public creator<NodeSpotLight>
{
protected:
	NodeSpotLight() = delete;
	NodeSpotLight(std::string name);
public:
	virtual ~NodeSpotLight();
	uint8_t getType() override;
	UIObjectPtr clone() override;
	friend class creator<NodeSpotLight>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);
};

typedef std::shared_ptr<NodeSpotLight> NodeSpotLightPtr;


class NodeDirectionalLight final : public UIObject, public creator<NodeDirectionalLight>
{
protected:
	NodeDirectionalLight() = delete;
	NodeDirectionalLight(std::string name);
public:
	virtual ~NodeDirectionalLight();
	uint8_t getType() override;
	UIObjectPtr clone() override;
	friend class creator<NodeDirectionalLight>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);
};

typedef std::shared_ptr<NodeDirectionalLight> NodeDirectionalLightPtr;