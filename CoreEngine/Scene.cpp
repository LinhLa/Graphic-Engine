#include "stdafx.h"
#include "Scene.h"
#include "Configuration.h"
#include "Library.h"

#include <fstream>
#include <picojson.h>

#include "ResourceAcquire.h"
#include "EmptyNode.h"

#include "OriginProperty.h"
#include "SceneHelper.h"

#define ROOT 	"root"

#define INIT 	int32_t(0x01)
#define RENDER 	int32_t(0x02)

Scene::Scene()
{
	UIObjectPtr proot = EmptyNode::create(ROOT);
	OriginPropertyPtr OriginMethod = proot->GetPropertyMethodObj<OriginProperty>();
	OriginMethod->SetBroadCastEvent(true);
	m_scene_graph.push_front(proot);
	m_UIObject_table.insert({ ROOT, proot });
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

	LoadPropertyTypeList(json_value);
	LoadTextureList(json_value);
#ifdef OPENGL_RENDERING
	LoadFragmentShaderList(json_value);
	LoadVertexShaderList(json_value);
	LoadShaderProgramList(json_value);
	LoadMeshList(json_value);
	LoadModel(json_value);
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