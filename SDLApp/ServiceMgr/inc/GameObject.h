#pragma once
#include <IMessageHandler.h>
#include <SceneConfig.h>
#include <asset_define.h>
#include <Property.h>
#include <Animation.h>
#include <MyCustomDynamicAlloc.h>

class GameObject;
typedef std::unordered_map<std::string, GameObject*> GameObjectNodeTableType;

typedef std::list<GameObject*> SceneGraphType;
enum UI_TYPE : int8_t
{
	NODE_2D_IMAGE_TYPE = 0,
	NODE_2D_BUTTON_TYPE,
	NODE_2D_TEXT_TYPE,
	NODE_2D_VIEWPORT_TYPE,

	ANIMATION_TYPE,
	ENTITY_TYPE,
};


class GameObject: public Property, public NodeResourceProperty, public PropertyTableType
{
protected:
	static const SceneConfig* m_pSceneConfig;
	GameObject();
public:
	virtual ~GameObject();

	virtual void onInit(RenderInitArgumentType&) = 0;
	virtual void onDraw(RenderDrawArgumentType&) = 0;
	virtual void onClean(RenderCleanArgumentType&) = 0;
	virtual void onKeyInputEvent(KeyInputArgumentType&) = 0;
	virtual void onLoop(RenderLoopArgumentType&) = 0;

	template<class Type>
	static Type* GetObject(UI_TYPE type)
	{
		Type *pInstance = nullptr;
		switch (type)
		{
		case NODE_2D_IMAGE_TYPE:
			pInstance = new Node2DImage();
			break;
		case NODE_2D_BUTTON_TYPE:
			pInstance = new NodeButton2D();
			break;
		case NODE_2D_TEXT_TYPE:
			pInstance = new NodeText2D();
			break;
		case NODE_2D_VIEWPORT_TYPE:
			pInstance = new NodeViewPort2D();
			break;
		//case ANIMATION_TYPE:
			//pInstance = new Animation();
		default:
			_ASSERT(false);
			break;
		}
		return pInstance;
	}
};