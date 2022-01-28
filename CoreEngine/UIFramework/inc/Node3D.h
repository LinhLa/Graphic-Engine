#pragma once
#include <UIObject.h>
#include <NodeMesh.h>
#include <NodeLight.h>
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "GLTexture.h"

class Node3D final: public UIObject, public creator<Node3D>
{
private:
	std::map<std::string, ModelPtr>		m_modelStack;
	std::vector<UIObjectPtr>			m_pointLights;
	std::vector<UIObjectPtr>			m_spotLights;
	std::vector<UIObjectPtr>	m_directionalLights;
protected:
	Node3D(std::string name);
public:
	virtual ~Node3D();
	uint8_t getType() override;
	UIObjectPtr clone() override;
	friend class creator<Node3D>;

	void onDraw(VoidType&&);
	void onDrawDone(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);

	void SetModel(const std::string& name);

	template<class T>
	void SetNodeLight(std::vector<UIObjectPtr> lights)
	{
		if (typeid(T).hash_code() == typeid(NodePointLight).hash_code())
		{
			m_pointLights = std::move(lights);
			for (auto& node : m_pointLights)
			{
				this->m_childList.push_front(node);
			}
		}
		if (typeid(T).hash_code() == typeid(NodeSpotLight).hash_code())
		{
			m_spotLights = std::move(lights);
			for (auto& node : m_spotLights)
			{
				this->m_childList.push_front(node);
			}
		}
		if (typeid(T).hash_code() == typeid(NodeDirectionalLight).hash_code())
		{
			m_directionalLights = std::move(lights);
			for (auto& node : m_directionalLights)
			{
				this->m_childList.push_front(node);
				node->setParent(shared_from_this());
			}
		}
	}

	template<class T>
	std::vector<UIObjectPtr> getNodeLight()
	{
		if (typeid(T).hash_code() == typeid(NodePointLight).hash_code())
		{
			return m_pointLights;
		}
		if (typeid(T).hash_code() == typeid(NodeSpotLight).hash_code())
		{
			return m_spotLights;
		}
		if (typeid(T).hash_code() == typeid(NodeDirectionalLight).hash_code())
		{
			return m_directionalLights;
		}
		return {};
	}
};

typedef std::shared_ptr<Node3D> Node3DPtr;