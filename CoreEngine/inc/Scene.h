#pragma once
#include <SDL.h>
#include "SingletonHolder.h"
#include "UIObject.h"

extern const int32_t INIT;
extern const int32_t RENDER;

typedef std::unordered_map<std::string, UIObjectPtr> UIObjectTableType;
typedef std::list<UIObjectPtr> SceneGraphType;

class Scene : public SingletonHolder<Scene>
{
private:
	friend class SingletonHolder<Scene>;
	UIObjectTableType		m_UIObject_table;
	SceneGraphType			m_scene_graph;
	UIObjectPtr				m_pNodeRoot = nullptr;
public:
	void LoadResource();

	void AddResource(UIObjectPtr object);
	size_t RemoveResource(std::string url);

	void AddToSceneGraph(UIObjectPtr);
	void RemoveFromSceneGraph(UIObjectPtr);

	void onEvent(SDL_Event&);

	UIObjectPtr GetResource(std::string url, bool isAsync = false);
	std::vector<std::string> GetResource();

	UIObjectPtr GetRoot();

	template<class UI>
	std::shared_ptr<UI> LookupUIObject(std::string name)
	{
		static_assert(std::is_base_of<UIObject, UI>::value, "UI must derive from UIObject");
		auto itr = m_UIObject_table.find(name);
		if (itr != m_UIObject_table.end())
		{
			if(!std::dynamic_pointer_cast<UI>(itr->second))
			{
				throw std::logic_error("invalid argument");
			}
			return std::dynamic_pointer_cast<UI>(itr->second);
		}
		return nullptr;
	}

	template<class UI>
	std::shared_ptr<UI> LookupSceneNode(std::string url)
	{
		if (!m_pNodeRoot)
		{
			return nullptr;
		}

		std::list<std::string> url_stack;
		std::string node;
		while (std::getline(url, node, '/'))
		{
			url_stack.push_front(node);
		}

		if (ROOT == url_stack.front())
		{
			url_stack.pop_front();
			return m_pNodeRoot->lookupNode(url_stack);
		}
		else
		{
			return nullptr;
		}
	}

protected:
	Scene();
	virtual ~Scene();
};