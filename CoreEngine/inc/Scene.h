#pragma once
#include <SDL.h>
#include "SingletonHolder.h"
#include "UIObject.h"
typedef std::unordered_map<std::string, UIObjectPtr> UIObjectTableType;
typedef std::list<UIObjectPtr> SceneGraphType;

class Scene : public SingletonHolder<Scene>
{
private:
	friend class SingletonHolder<Scene>;
	UIObjectTableType		m_UIObject_table;
	SceneGraphType			m_scene_graph;

public:
	void LoadResource();

	void AddResource(UIObjectPtr object);
	size_t RemoveResource(std::string url);

	void AddToSceneGraph(UIObjectPtr);
	void RemoveFromSceneGraph(UIObjectPtr);

	void onEvent(SDL_Event&);
	UIObjectPtr GetResource(std::string url, bool isAsync = false);

	UIObjectPtr GetRoot();

	template<class UI>
	std::shared_ptr<UI> LookupUIObject(std::string url)
	{
		static_assert(std::is_base_of<UIObject, UI>::value, "UI must derive from UIObject");
		auto itr = m_UIObject_table.find(url);
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

protected:
	Scene();
	virtual ~Scene();
};