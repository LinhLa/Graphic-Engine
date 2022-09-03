#pragma once
#include "creator.h"
#include <UIObject.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<class L>
class LightHelper
{
private:
	std::shared_ptr<L> m_pLight = nullptr;
public:
	LightHelper(UIObjectPtr p)
	{
		assert(p);
		m_pLight = std::dynamic_pointer_cast<L>(p);
	}
	~LightHelper() {}

	glm::mat4 getViewMatrix(glm::vec3 target, glm::vec3 up)
	{
		return glm::lookAt(getPosition(), target, up);
	}


	glm::vec3 getPosition()
	{
		glm::vec3 position;
		switch (m_pLight->GetPropertyValue<glm::i32>(LIGHT_TYPE))
		{
		case POINT_LIGHT:
			position = m_pLight->GetPropertyValue<glm::vec3>(POINT_LIGHT_POSITION);
			break;
		case DIRECTIONAL_LIGHT:
			position = m_pLight->GetPropertyValue<glm::vec3>(DIRECTIONAL_LIGHT_POSITION);
			break;
		case SPOT_LIGHT:
			position = m_pLight->GetPropertyValue<glm::vec3>(SPOT_LIGHT_POSITION);
			break;
		default:
			break;
		}
		//<get light position in world
		return position;
	}
};

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