#pragma once
#include <base_define.h>
#include <type_define.h>
#include <GameObject.h>
#include <SDL.h>
class SceneConfig;
class SceneService : public IService
{
private:
	friend class ServiceMgr;
	SceneConfig*	m_pSceneConfig = nullptr;

	GameObjectNodeTableType m_game_object_table;
	SceneGraphType			m_scene_graph;

	FunctionBaseSharedPtr m_pInitResourceCallback = nullptr;
	FunctionBaseSharedPtr m_pCleanCallback = nullptr;
	FunctionBaseSharedPtr m_pDrawResourceCallback = nullptr;
	FunctionBaseSharedPtr m_pLoopCallback = nullptr;

	void onInit(RenderInitArgumentType&);
	void onDraw(RenderDrawArgumentType&);
	void onClean(RenderCleanArgumentType&);
	void onLoop(RenderLoopArgumentType&);

	template<class TypeUI>
	TypeUI* GetResource(const std::string &name)
	{
		TypeUI* pTypeUI = nullptr;
		auto itr = m_game_object_table.find(name);
		if (itr != m_game_object_table.end())
		{
			pTypeUI = dynamic_cast<TypeUI*>(itr->second);
		}
		//<Assert on invalid name resource
		_ASSERT(pTypeUI);
		return pTypeUI;
	}

	void LoadResource();

	void AddResource(GameObject* object);
	void RemoveResource(GameObject* object);
	void AddToSceneGraph(GameObject* precede, GameObject* object);
	void RemoveFromSceneGraph(GameObject* object);
public:
	SceneService();
	~SceneService();

	virtual void onStartService() override;
	virtual void onEndService() override;
	virtual void onMessageReceive(const Message&) override;

};

typedef std::shared_ptr<SceneService> SceneServicePtr;