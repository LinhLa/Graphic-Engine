#include "stdafx.h"
#include "SceneService.h"
#include <ServiceMgr.h>
#include <SceneConfig.h>
#include <ResourceAcquire.h>

#include <fstream>
#include <picojson.h>
#include <math.h>

#include <Property.h>
#include <Node2DImage.h>
#include <NodeButton2D.h>
#include <NodeText2D.h>
#include <NodeViewPort2D.h>
#include <IEntity.h>

#define FRAMES				"frames"

#define RESOURCE_NAME		"filename"
#define FRAME				"frame"
#define TYPE				"type"
#define SPRITE_SOURCE_SIZE	"spriteSourceSize"
#define SOURCE_SIZE			"sourceSize"
#define HEIGHT			"h"
#define WIDTH			"w"
#define X_COODINATOR	"x"
#define Y_COODINATOR	"y"


#define DOUBLE2INT(X) int(rint(X))

SceneService::SceneService() :
	m_pSceneConfig(SceneConfig::GetInstance())
{
	_ASSERT(m_pServiceMgr && m_pSceneConfig);
	m_pInitResourceCallback.reset(new function_member<SceneService, RenderInitArgumentType>(this, &SceneService::onInit));
	m_pDrawResourceCallback.reset(new function_member<SceneService, RenderDrawArgumentType>(this, &SceneService::onDraw));
	m_pCleanCallback.reset(new function_member<SceneService, RenderCleanArgumentType>(this, &SceneService::onClean));
	m_pLoopCallback.reset(new function_member<SceneService, RenderLoopArgumentType>(this, &SceneService::onLoop));
}

SceneService::~SceneService()
{
}

void SceneService::onStartService()
{

	//<Add trigger event to service manager
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_SCREEN_HEIGHT);
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_SCREEN_WIDTH);
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_ASSET_PATH);
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_DATA_FILE);

	m_pServiceMgr->registerOnEvent(this, RENDER_INSTANCE_UPDATE);
	m_pServiceMgr->registerOnEvent(this, SCREEN_SURFACE_UPDATE);

	m_pServiceMgr->registerOnEvent(this, SCENE_QUERRY_RESOURCE_SYNC);
	m_pServiceMgr->registerOnEvent(this, SCENE_QUERRY_RESOURCE_ASYNC);

	m_pServiceMgr->registerOnEvent(this, SCENE_ADD_RESOURCE);
	m_pServiceMgr->registerOnEvent(this, SCENE_REMOVE_RESOURCE);

	m_pServiceMgr->registerOnEvent(this, SCENE_LOAD_RESOURCE);
	m_pServiceMgr->registerOnEvent(this, SCENE_UNLOAD_RESOURCE);


	RENDER_REGISTER_CALLBACK(m_pServiceMgr, m_pInitResourceCallback.get(), RENDER_EVENT_INIT);
	RENDER_REGISTER_CALLBACK(m_pServiceMgr, m_pDrawResourceCallback.get(), RENDER_EVENT_DRAW);
	RENDER_REGISTER_CALLBACK(m_pServiceMgr, m_pCleanCallback.get(), RENDER_EVENT_CLEAN);
	RENDER_REGISTER_CALLBACK(m_pServiceMgr, m_pLoopCallback.get(), RENDER_EVENT_LOOP);
}

void SceneService::onEndService()
{
	RENDER_UNREGISTER_CALLBACK(m_pServiceMgr, m_pInitResourceCallback.get(), RENDER_EVENT_INIT);
	RENDER_UNREGISTER_CALLBACK(m_pServiceMgr, m_pDrawResourceCallback.get(), RENDER_EVENT_DRAW);
	RENDER_UNREGISTER_CALLBACK(m_pServiceMgr, m_pCleanCallback.get(), RENDER_EVENT_CLEAN);
	RENDER_UNREGISTER_CALLBACK(m_pServiceMgr, m_pLoopCallback.get(), RENDER_EVENT_LOOP);

	//<Remove trigger event to service manager
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_SCREEN_HEIGHT);
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_SCREEN_WIDTH);
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_ASSET_PATH);
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_DATA_FILE);

	m_pServiceMgr->unregisterOnEvent(this, RENDER_INSTANCE_UPDATE);
	m_pServiceMgr->unregisterOnEvent(this, SCREEN_SURFACE_UPDATE);

	m_pServiceMgr->unregisterOnEvent(this, SCENE_QUERRY_RESOURCE_SYNC);
	m_pServiceMgr->unregisterOnEvent(this, SCENE_QUERRY_RESOURCE_ASYNC);

	m_pServiceMgr->unregisterOnEvent(this, SCENE_ADD_RESOURCE);
	m_pServiceMgr->unregisterOnEvent(this, SCENE_REMOVE_RESOURCE);

	m_pServiceMgr->unregisterOnEvent(this, SCENE_LOAD_RESOURCE);
	m_pServiceMgr->unregisterOnEvent(this, SCENE_UNLOAD_RESOURCE);


}

void SceneService::onInit(RenderInitArgumentType& arg)
{
	for (auto &object : m_game_object_table)
	{
		object.second->onInit(arg);
	}
}

void SceneService::onDraw(RenderDrawArgumentType& arg)
{
	for (auto &object : m_scene_graph)
	{
		object->onDraw(arg);
	}
}

void SceneService::onClean(RenderCleanArgumentType &arg)
{
	for (auto &object : m_game_object_table)
	{
		object.second->onClean(arg);
	}
}

void SceneService::onLoop(RenderLoopArgumentType &arg)
{
	for (auto &object : m_scene_graph)
	{
		object->onLoop(arg);
	}
}

void SceneService::onMessageReceive(const Message &msg)
{
	uint64_t* address = nullptr;
	switch (msg.m_id)
	{
	case CONFIG_UPDATE_SCREEN_HEIGHT:
		m_pSceneConfig->SetScreenHeight(msg.m_iValue & 0xFFFFFFFF);
		break;
	case CONFIG_UPDATE_SCREEN_WIDTH:
		m_pSceneConfig->SetScreenWidth(msg.m_iValue & 0xFFFFFFFF);
		break;
	case CONFIG_UPDATE_ASSET_PATH:
		m_pSceneConfig->SetAssetPath(reinterpret_cast<char*>(msg.m_msg_data.m_pData), msg.m_msg_data.m_len);
		break;
	case CONFIG_UPDATE_DATA_FILE:
		m_pSceneConfig->SetDataFile(reinterpret_cast<char*>(msg.m_msg_data.m_pData), msg.m_msg_data.m_len);
		//<load resource
		m_pServiceMgr->addBackgroundTask(std::bind(&SceneService::LoadResource, this));
		break;
	case RENDER_INSTANCE_UPDATE:
		_ASSERT(msg.m_msg_data.m_pData && msg.m_msg_data.m_len == (2*sizeof(uint64_t)));
		address = reinterpret_cast<uint64_t*>(msg.m_msg_data.m_pData);
		//<set render instance member
		m_pSceneConfig->SetRenderer(reinterpret_cast<SDL_Renderer*>(address[0]));
		m_pSceneConfig->SetScreenSurface(reinterpret_cast<SDL_Surface*>(address[1]));

		//<request init resource to render service
		m_pServiceMgr->sendMessage({ SCENE_INIT_DONE , 0ULL });
		break;
	case SCENE_QUERRY_RESOURCE_SYNC:
	case SCENE_QUERRY_RESOURCE_ASYNC:
		_ASSERT(msg.m_msg_object.m_pObject);
		switch (msg.m_msg_object.m_type)
		{
		case NODE_2D_IMAGE_TYPE:
			*msg.m_msg_object.m_pObject = GetResource<Node2DImage>(msg.m_msg_object.m_pResourceName);
			break;
		case NODE_2D_BUTTON_TYPE:
			*msg.m_msg_object.m_pObject = GetResource<NodeButton2D>(msg.m_msg_object.m_pResourceName);
			break;
		case NODE_2D_TEXT_TYPE:
			*msg.m_msg_object.m_pObject = GetResource<NodeButton2D>(msg.m_msg_object.m_pResourceName);
			break;
		case NODE_2D_VIEWPORT_TYPE:
			*msg.m_msg_object.m_pObject = GetResource<NodeViewPort2D>(msg.m_msg_object.m_pResourceName);
			break;
		case ENTITY_TYPE:
			*msg.m_msg_object.m_pObject = GetResource<IEntity>(msg.m_msg_object.m_pResourceName);
			break;
		default:
			_ASSERT(false);
			break;
		}
		//<Remove Resource Object from acquire list
		ResourceAcquire::GetInstance()->ResourceAcquired(msg.m_msg_object);
		break;
	case SCENE_ADD_RESOURCE:
		_ASSERT(msg.m_msg_data.m_pData);
		AddResource(reinterpret_cast<GameObject*>(msg.m_msg_data.m_pData));
		break;
	case SCENE_REMOVE_RESOURCE:
		_ASSERT(msg.m_msg_data.m_pData);
		RemoveResource(reinterpret_cast<GameObject*>(msg.m_msg_data.m_pData));
		break;
	case SCENE_LOAD_RESOURCE:
		break;
	case SCENE_UNLOAD_RESOURCE:
		break;
	default:
		break;
	}
}

void SceneService::AddResource(GameObject* object)
{
	//<add game object
	m_game_object_table[object->GetResourceName()] = object;
}

void SceneService::RemoveResource(GameObject* object)
{
	//<remove game object
	size_t number_item = m_game_object_table.erase(object->GetResourceName());

	//<clean up
	if (0 < number_item)
	{
		delete object;
	}
}

void SceneService::AddToSceneGraph(GameObject* precede, GameObject* object)
{
	_ASSERT(object);
	if (!precede)
	{
		m_scene_graph.push_back(object);
		return;
	}
	else
	{
		auto itr = std::find(m_scene_graph.begin(), m_scene_graph.end(), precede);
		if (itr != m_scene_graph.end())
		{
			m_scene_graph.insert(++itr, object);
		}
	}
}

void SceneService::RemoveFromSceneGraph(GameObject* object)
{
	m_scene_graph.remove(object);
}

void SceneService::LoadResource()
{
	char buffer[512];

	memset(buffer, 0, sizeof(buffer));

	//<Open asset data file
	std::ifstream inAssetFile(m_pSceneConfig->GetDataFile());

	//<validate
	_ASSERT(inAssetFile);

	//The easiest way is to use the two - argument parse function.
	picojson::value json_value;
	std::string err = picojson::parse(json_value, inAssetFile);

	//< Should not have error
	_ASSERT(err.empty());

	// check if the type of the value is "object"
	_ASSERT(json_value.is<picojson::object>());

	picojson::array o_frames_list = json_value.get(FRAMES).get<picojson::array>();
	for (picojson::array::iterator iter = o_frames_list.begin(); iter != o_frames_list.end(); ++iter)
	{
		picojson::object o_frame = iter->get(FRAME).get<picojson::object>();

		//picojson::object o_sourceSize = iter->get(SOURCE_SIZE).get<picojson::object>();

		//<Get Object type
		std::string type = iter->get(TYPE).get<std::string>();
		GameObject* pObject = GameObject::GetObject<GameObject>(UI_TYPE(std::stoi(type)));

		//<set resource name
		std::string filename = iter->get(RESOURCE_NAME).get<std::string>();
		pObject->SetResourceName(filename.c_str());

		picojson::object o_spriteSourceSize = iter->get(SPRITE_SOURCE_SIZE).get<picojson::object>();

		//<Set source width and height
		pObject->setSourceWidth(DOUBLE2INT(o_spriteSourceSize[WIDTH].get<double>()));
		pObject->setSourceHeight(DOUBLE2INT(o_spriteSourceSize[HEIGHT].get<double>()));

		//<Set source coordinator (X,Y)
		pObject->SetPosition(
			DOUBLE2INT(o_spriteSourceSize[X_COODINATOR].get<double>()),
			DOUBLE2INT(o_spriteSourceSize[Y_COODINATOR].get<double>()));

		//<Set destination width and height
		pObject->setDestinationWidth(DOUBLE2INT(o_frame[WIDTH].get<double>()));
		pObject->setDestinationHeight(DOUBLE2INT(o_frame[HEIGHT].get<double>()));

		//<Set destination coordinator (X,Y)
		pObject->SetPosition(
			DOUBLE2INT(o_frame[X_COODINATOR].get<double>()),
			DOUBLE2INT(o_frame[Y_COODINATOR].get<double>()));

		//<add to scene graph
		m_game_object_table[filename] = pObject;
	}
	m_pServiceMgr->sendMessage({ SCENE_LOAD_RESOURCE_DONE , 0ULL});
}