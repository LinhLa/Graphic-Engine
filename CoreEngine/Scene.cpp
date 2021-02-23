#include "stdafx.h"
#include "Scene.h"
#include "Configuration.h"
#include "Library.h"
#include "Render.h"

#include <fstream>
#include <picojson.h>
#include <math.h>

#include "ResourceAcquire.h"
#include "macro_define.h"
#include "Node2DImage.h"
#include "NodeButton2D.h"
#include "NodeText2D.h"
#include "NodeViewPort2D.h"
#include "EmptyNode.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "TextProperty.h"

#include "Shader.h"
#include "ShaderProgram.h"

#define INIT 	int32_t(0x01)
#define RENDER 	int32_t(0x02)
#define ROOT 	"root"

#define TEXTURE_LIST		"texture_list"
#define RESOURCES_LIST		"resources_list"
#define STATE_MANAGER_LIST	"state_manager_list"
#define STATE_GROUP_LIST	"state_group_list"
#define STATE_LIST			"state_list"

#define TEXTURE_PATH		"texture_path"

#define RESOURCE_NAME		"resource_name"
#define TEXTURE_NAME		"texture_name"

#define STATE_NAME			"name"
#define STATE_MANAGER_NAME	"name"
#define STATE_GROUP_NAME	"name"

#define TYPE				"type"
#define LAYOUT				"layout"
#define HEIGHT				"h"
#define WIDTH				"w"
#define X_COODINATOR		"x"
#define Y_COODINATOR		"y"

#define SCALE_X				"scale_x"
#define SCALE_Y				"scale_y"

#define ORIGINAL_PROPERTY	"origin_property"
#define VISIBLE				"visible"
#define LOAD_TO_GPU			"load_to_gpu"
#define ALPHA				"alpha"
#define ANGLE				"angle"
#define CENTER_POINT		"center_point"
#define FLIP				"flip"
#define OPACITY				"opacity"
#define ISCLIP				"isclip"

#define TEXT_PROPERTY		"text_property"
#define TEXT				"text"
#define FONT_NAME			"font_name"
#define FONT_SIZE			"point_size"
#define FONT_COLOR			"font_color"

#define FOREGROUND_COLOR	"fore_ground_color"
#define BACKGROUND_COLOR	"back_ground_color"
#define RED 				"r"
#define GREEN 				"g"
#define BLUE				"b"

#define PROPERTY_LIST			"property_list"
#define PROPERTY_NAME			"property_name"
#define PROPERTY_TYPE			"type"
#define PROPERTY_VALUE			"value"
#define PROPERTY_UPPER			"upper"
#define PROPERTY_LOWER			"lower"

#define STATE_MANAGER 			"state_manager"

#define SHADER_PROGRAM_LIST		"ShaderProgram_list"
#define VEX_SHADER_LIST			"VertexShader_list"
#define FRAG_SHADER_LIST		"FragmentShader_list"

#define SHADER_NAME				"name"
#define SHADER_SOURCE			"source"
#define SHADER					"shader"

#define DOUBLE2INT(X) int(rint(X))
#define DOUBLE2UINT8(X) uint8_t(rint(X))

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
					object->onInit();
					break;
					case RENDER:
					object->onDraw();
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

	picojson::value snull;

	//< get property list
	picojson::array o_property_list = json_value.get(PROPERTY_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_property_list.begin(); iter != o_property_list.end(); ++iter)
	{
		std::string name = iter->get(PROPERTY_NAME).get<std::string>();
		std::string type = iter->get(PROPERTY_TYPE).get<std::string>();
		std::string value = iter->get(PROPERTY_VALUE).get<std::string>();
		std::string upper = iter->get(PROPERTY_UPPER).get<std::string>();
		std::string lower = iter->get(PROPERTY_LOWER).get<std::string>();

		uint8_t uType = std::stoi(type) & 0xFF;
		IPropertyPtr pProperty = nullptr;
		switch(uType)
		{
			case INT:
				pProperty = Property<int>::create(name, ConvertToType<int>(value));
				break;
			case BOOL:
				pProperty = Property<bool>::create(name, ConvertToType<bool>(value));
				break;
			case FLOAT:
				pProperty = Property<float>::create(name, ConvertToType<float>(value));
				break;
			case COLOR:
				break;
			default:
				break;
		}
		Library::GetInstance()->addProperty(name, pProperty);
	}

	//<get fragment shader list
	picojson::array o_fragment_list = json_value.get(FRAG_SHADER_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_fragment_list.begin(); iter != o_fragment_list.end(); ++iter)
	{
		std::string shader_name = iter->get(SHADER_NAME).get<std::string>();
		std::string shader_source = Configuration::GetInstance()->shader_path + iter->get(SHADER_SOURCE).get<std::string>();
		auto pShader = Shader::create(shader_name, GL_FRAGMENT_SHADER);

		//<Open shader source file
		std::ifstream inShaderStream(shader_source.c_str());

		//<Validate ifstream
		_ASSERT(inShaderStream);

		//<Read to string
		std::string inShaderString((std::istreambuf_iterator<char>(inShaderStream)), std::istreambuf_iterator<char>());

		//<Attach source
		GLint len = static_cast<GLint>(inShaderString.length());
		const char* const pChar = inShaderString.c_str();
		pShader->ShaderSource(1, &pChar, &len);

		//<Compile shader
		pShader->CompileShader();

		//<Add to library
		Library::GetInstance()->addShader(shader_name, pShader);
	}

	//<get vertex shader list
	picojson::array o_vertex_list = json_value.get(VEX_SHADER_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_vertex_list.begin(); iter != o_vertex_list.end(); ++iter)
	{
		std::string shader_name = iter->get(SHADER_NAME).get<std::string>();
		std::string shader_source = Configuration::GetInstance()->shader_path + iter->get(SHADER_SOURCE).get<std::string>();

		auto pShader = Shader::create(shader_name, GL_VERTEX_SHADER);

		//<Open shader source file
		std::ifstream inShaderStream(shader_source.c_str());

		//<Validate ifstream
		_ASSERT(inShaderStream);

		//<Read to string
		std::string inShaderString((std::istreambuf_iterator<char>(inShaderStream)), std::istreambuf_iterator<char>());

		//<Attach source
		GLint len = static_cast<GLint>(inShaderString.length());
		const char* const pChar = inShaderString.c_str();
		pShader->ShaderSource(1, &pChar, &len);

		//<Compile shader
		pShader->CompileShader();

		//<Add to library
		Library::GetInstance()->addShader(shader_name, pShader);
	}

	//<get shader program list
	picojson::array o_shaderProgram_list = json_value.get(SHADER_PROGRAM_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_shaderProgram_list.begin(); iter != o_shaderProgram_list.end(); ++iter)
	{
		std::string program_name = iter->get(SHADER_NAME).get<std::string>();
		auto pShaderProgram = ShaderProgram::create(program_name);

		picojson::array shader_list = iter->get(SHADER).get<picojson::array>();

		//<get shader name list
		for (picojson::value item : shader_list)
		{
			std::string shader_name = item.get<std::string>();
			//<Attach shader to program
			pShaderProgram->AttachShader(Library::GetInstance()->getShader(shader_name));
		}

		//<Link to program
		pShaderProgram->LinkProgram();

		//<Add to library
		Library::GetInstance()->addShaderProgram(program_name, pShaderProgram);
	}

	//<get texture list
	picojson::array o_texture_list = json_value.get(TEXTURE_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_texture_list.begin(); iter != o_texture_list.end(); ++iter)
	{
		std::string texture_name = iter->get(TEXTURE_NAME).get<std::string>();
		std::string texture_path = Configuration::GetInstance()->asset_path + iter->get(TEXTURE_PATH).get<std::string>();
		bool loadtoGPU = iter->get(LOAD_TO_GPU).get<bool>();

		auto pTexture = Texture::create(Render::GetInstance()->getRenderer(), texture_path);
		pTexture->LoadToGPU(loadtoGPU);
		Library::GetInstance()->addTexture(texture_name, pTexture);
	}

	//<get resource list
	picojson::array o_resource_list = json_value.get(RESOURCES_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_resource_list.begin(); iter != o_resource_list.end(); ++iter)
	{
		//<get resource name
		if (snull == iter->get(RESOURCE_NAME))
		{
			continue;
		}

		std::string resource_name = iter->get(RESOURCE_NAME).get<std::string>();
		UIObjectPtr pObject = nullptr;
		uint8_t type = std::stoi(iter->get(TYPE).get<std::string>()) & 0xFF;
		switch (type)
		{
		case NODE_2D_IMAGE_TYPE:
			pObject = Node2DImage::create(resource_name);
			break;
		case NODE_2D_BUTTON_TYPE:
			pObject = NodeButton2D::create(resource_name);
			break;
		case NODE_2D_TEXT_TYPE:
			pObject = NodeText2D::create(resource_name);
			break;
		case NODE_2D_VIEWPORT_TYPE:
			pObject = NodeViewPort2D::create(resource_name);
			break;
		default:
			continue;
			break;
		}

		//<set image
		if (NODE_2D_IMAGE_TYPE == type)
		{
			std::string texture_name = iter->get(TEXTURE_NAME).get<std::string>();
			std::dynamic_pointer_cast<Node2DImage>(pObject)->SetTexture(texture_name);
		}

		//set layout property
		if (snull != iter->get(LAYOUT))
		{
			picojson::object o_layout = iter->get(LAYOUT).get<picojson::object>();
			auto layoutMethod = pObject->GetPropertyMethodObj<LayoutProperty>();

			//<Set layout width and height
			layoutMethod->SetLayoutWidth(DOUBLE2INT(o_layout[WIDTH].get<double>()));
			layoutMethod->SetLayoutHeight(DOUBLE2INT(o_layout[HEIGHT].get<double>()));

			//<Set layout coordinator (X,Y)
			layoutMethod->SetLayoutPosition(
				DOUBLE2INT(o_layout[X_COODINATOR].get<double>()),
				DOUBLE2INT(o_layout[Y_COODINATOR].get<double>()));
		}

		//<set origin property
		if (snull != iter->get(ORIGINAL_PROPERTY))
		{
			OriginPropertyPtr originMethod = pObject->GetPropertyMethodObj<OriginProperty>();

			//<Set origin property
			picojson::object o_origin_property = iter->get(ORIGINAL_PROPERTY).get<picojson::object>();
			picojson::object o_center_point = o_origin_property[CENTER_POINT].get<picojson::object>();
			originMethod->SetVisible(o_origin_property[VISIBLE].get<bool>());

			originMethod->SetCenterPoint(
				DOUBLE2INT(o_center_point[X_COODINATOR].get<double>()),
				DOUBLE2INT(o_center_point[Y_COODINATOR].get<double>()));

			originMethod->SetFlip(SDL_RendererFlip(DOUBLE2INT(o_origin_property[FLIP].get<double>())));
			originMethod->SetOpacity(DOUBLE2INT(o_origin_property[OPACITY].get<double>()));
			originMethod->SetAngle(o_origin_property[ANGLE].get<double>());
			originMethod->SetClip(o_origin_property[ISCLIP].get<bool>());

			if (o_origin_property.end() != o_origin_property.find(FOREGROUND_COLOR))
			{
				picojson::object o_foreground = o_origin_property[FOREGROUND_COLOR].get<picojson::object>();
				SDL_Color color = {
					DOUBLE2UINT8(o_foreground[RED].get<double>()),
					DOUBLE2UINT8(o_foreground[GREEN].get<double>()),
					DOUBLE2UINT8(o_foreground[BLUE].get<double>()),
					DOUBLE2UINT8(o_foreground[ALPHA].get<double>())
				};
				originMethod->SetForeGroundColor(color);
			}

			if (o_origin_property.end() != o_origin_property.find(BACKGROUND_COLOR))
			{
				picojson::object o_background = o_origin_property[BACKGROUND_COLOR].get<picojson::object>();
				SDL_Color color = {
					DOUBLE2UINT8(o_background[RED].get<double>()),
					DOUBLE2UINT8(o_background[GREEN].get<double>()),
					DOUBLE2UINT8(o_background[BLUE].get<double>()),
					DOUBLE2UINT8(o_background[ALPHA].get<double>())
				};
				originMethod->SetBackGroundColor(color);
			}
		}

		//<Set text property
		if (NODE_2D_TEXT_TYPE == type && snull != iter->get(TEXT_PROPERTY))
		{
			NodeText2DPtr pNodeText2D = std::dynamic_pointer_cast<NodeText2D>(pObject);
			TextPropertyPtr textMethod = pNodeText2D->GetPropertyMethodObj<TextProperty>();

			picojson::object o_text_property = iter->get(TEXT_PROPERTY).get<picojson::object>();
			textMethod->SetText(o_text_property[TEXT].get<std::string>().c_str());
			textMethod->SetFontName(o_text_property[FONT_NAME].get<std::string>().c_str());
			textMethod->SetFontSize(DOUBLE2INT(o_text_property[FONT_SIZE].get<double>()));

			picojson::object o_fontColor = o_text_property[FONT_COLOR].get<picojson::object>();
			textMethod->SetColor(
					DOUBLE2INT(o_fontColor[RED].get<double>()),
					DOUBLE2INT(o_fontColor[GREEN].get<double>()),
					DOUBLE2INT(o_fontColor[BLUE].get<double>()),
					DOUBLE2INT(o_fontColor[ALPHA].get<double>()));
		}

		//<Set user define property list
		if (snull != iter->get(PROPERTY_LIST))
		{
			picojson::array o_user_define_property_list = iter->get(PROPERTY_LIST).get<picojson::array>();
			for (picojson::array::iterator iter = o_user_define_property_list.begin(); iter != o_user_define_property_list.end(); ++iter)
			{
				std::string name = iter->get(PROPERTY_NAME).get<std::string>();
				std::string type = iter->get(PROPERTY_TYPE).get<std::string>();
				std::string value = iter->get(PROPERTY_VALUE).get<std::string>();
				std::string upper = iter->get(PROPERTY_UPPER).get<std::string>();
				std::string lower = iter->get(PROPERTY_LOWER).get<std::string>();

				uint8_t uType = std::stoi(type) & 0xFF;
				switch (uType)
				{
				case INT:
					pObject->AddProperty(name, Property<int>::create(name, ConvertToType<int>(value)));
					break;
				case BOOL:
					pObject->AddProperty(name, Property<bool>::create(name, ConvertToType<bool>(value)));
					break;
				case FLOAT:
					pObject->AddProperty(name, Property<float>::create(name, ConvertToType<float>(value)));
					break;
				case COLOR:
					break;
				default:
					break;
				}
			}
		}
		//<add to ui object table
		m_UIObject_table[resource_name] = pObject;
		LOG_DEBUG("Resource added[%s]", resource_name.c_str());
	}
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