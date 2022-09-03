#include "stdafx.h"
#include "Scene.h"
#include "Configuration.h"
#include "Library.h"

#include <fstream>
#include <picojson.h>

#include "ResourceAcquire.h"
#include "EmptyNode.h"

#include "PropertyDefine.h"
#include "SceneHelper.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"

#include "ScriptFunction.h"

const int32_t INIT = (0x01);
const int32_t RENDER = (0x02);

#define ROOT 	"root"

Scene::Scene()
{
	m_pNodeRoot = EmptyNode::create(ROOT);
	m_pNodeRoot->onInit();
	m_pNodeRoot->SetPropertyValue<bool>(IS_BROADCAST_EVENT, true);
	
	m_scene_graph.push_front(m_pNodeRoot);
	m_UIObject_table.insert({ ROOT, m_pNodeRoot });
}

Scene::~Scene()
{
}

void Scene::onEvent(SDL_Event& event)
{
	for (auto &object : m_scene_graph)
	{
		switch(event.type)
		{
			case SDL_USEREVENT:
			{
				switch(event.user.code)
				{
					case INIT:
					{
						object->onInit();
						break;
					}
					case RENDER:
					{
						object->onDraw();
						break;
					}
					default:
					break;
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				object->onKeyInputEvent(event);
				break;
			}
			case SDL_QUIT:
				object->onClean();
			default:
				break;
		}
	}
}

UIObjectPtr Scene::GetRoot()
{
	return m_UIObject_table[ROOT];
}

void Scene::AddResource(UIObjectPtr object)
{
	//<add object
	m_UIObject_table[object->getUrl()] = object;
}

size_t Scene::RemoveResource(std::string url)
{
	//<remove object
	return m_UIObject_table.erase(url);
}

void Scene::AddToSceneGraph(UIObjectPtr object)
{
	if (!object)
	{
		return;
	}
	GetRoot()->addChild(object);
}

void Scene::RemoveFromSceneGraph(UIObjectPtr object)
{
	m_scene_graph.remove(object);
}

void Scene::LoadResource()
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	Configuration* pConfig = Configuration::GetInstance();

	//<Open asset data file
	std::ifstream inAssetFile(pConfig->data_file);

	//<validate
	_ASSERT(inAssetFile);

	//The easiest way is to use the two - argument parse function.
	picojson::value json_value;
	std::string err = picojson::parse(json_value, inAssetFile);

	//< Should not have error
	_ASSERT(err.empty());

	// check if the type of the value is "object"
	_ASSERT(json_value.is<picojson::object>());

	LoadScript();
	LoadTextureList(json_value);
	LoadPropertyTypeList(json_value);
	
#ifdef OPENGL_RENDERING
	LoadShaderProgramList(json_value);
	LoadMaterialList(json_value);
	
	LoadMeshList(json_value);
	LoadModel(m_UIObject_table, json_value);
#endif
	LoadResourceList(m_UIObject_table, json_value);
}

UIObjectPtr Scene::GetResource(std::string url, bool isAsync)
{
	if (isAsync)
	{
		//<Remove Resource Object from acquire list
		ResourceAcquire::GetInstance()->ResourceAcquired(url);
	}
	return nullptr;
}

std::vector<std::string> Scene::GetResource()
{
	std::vector<std::string> list;
	for (auto& item : m_UIObject_table)
	{
		list.push_back(item.first);
	}
	return list;
}