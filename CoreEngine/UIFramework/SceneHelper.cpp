#include "stdafx.h"
#include "utils.h"
using namespace util;

#include "SceneHelper.h"
#include "Configuration.h"
#include "log.h"
#include "Render.h"

#include <fstream>
#include <math.h>

#include "Library.h"

#include "Node2DImage.h"
#include "NodeButton2D.h"
#include "NodeText2D.h"
#include "NodeViewPort2D.h"
#include "EmptyNode.h"
#include "Node3D.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "TextProperty.h"
#include "GLProperty.h"

#include "Shader.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Model.h"

#include "GLTypeDictionary.h"
#include "colladainterface.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

static const std::string PROPERTY_TYPE_LIST = std::string("property_type_list");
static const std::string TEXTURE_LIST = std::string("texture_list");
static const std::string RESOURCES_LIST = std::string("resources_list");

static const std::string TEXTURE_PATH = std::string("texture_path");

static const std::string RESOURCE_NAME = std::string("resource_name");
static const std::string TEXTURE_NAME = std::string("texture_name");

static const std::string UI_TYPE = std::string("type");
static const std::string LAYOUT = std::string("layout");
static const std::string HEIGHT = std::string("h");
static const std::string WIDTH = std::string("w");
static const std::string X_COODINATOR = std::string("x");
static const std::string Y_COODINATOR = std::string("y");

static const std::string SCALE_X = std::string("scale_x");
static const std::string SCALE_Y = std::string("scale_y");

static const std::string TRANSFORM = std::string("transform");
static const std::string ROTATE = std::string("rotate");
static const std::string SCALE = std::string("scale");

static const std::string ORIGINAL_PROPERTY = std::string("origin_property");
static const std::string VISIBLE = std::string("visible");
static const std::string LOAD_TO_GPU = std::string("load_to_gpu");
static const std::string ALPHA = std::string("alpha");
static const std::string ANGLE = std::string("angle");
static const std::string CENTER_POINT = std::string("center_point");
static const std::string FLIP = std::string("flip");
static const std::string OPACITY = std::string("opacity");
static const std::string ISCLIP = std::string("isclip");

static const std::string TEXT_PROPERTY = std::string("text_property");
static const std::string TEXT = std::string("text");
static const std::string FONT_NAME = std::string("font_name");
static const std::string FONT_SIZE = std::string("point_size");
static const std::string FONT_COLOR = std::string("font_color");

static const std::string FOREGROUND_COLOR = std::string("fore_ground_color");
static const std::string BACKGROUND_COLOR = std::string("back_ground_color");
static const std::string RED = std::string("r");
static const std::string GREEN = std::string("g");
static const std::string BLUE = std::string("b");

static const std::string MATERIAL_PROPERTY_LIST = std::string("material_property");
static const std::string PROPERTY_NAME = std::string("property_name");
static const std::string PROPERTY_TYPE = std::string("type");
static const std::string PROPERTY_VALUE = std::string("value");
static const std::string PROPERTY_UPPER = std::string("upper");
static const std::string PROPERTY_LOWER = std::string("lower");

static const std::string THREE_D_ASSET_LIST = std::string("3D_asset_list");

static const std::string NAME = std::string("name");

#define DOUBLE2INT(X)		int(rint(X))
#define DOUBLE2UINT8(X)		uint8_t(rint(X))
#define DOUBLE2UINT32(X)	uint32_t(rint(X))
#define DOUBLE2FLOAT(X)		static_cast<float>(X)

static const std::string SHADER_PROGRAM_LIST = std::string("ShaderProgram_list");
static const std::string VEX_SHADER_LIST = std::string("VertexShader_list");
static const std::string FRAG_SHADER_LIST = std::string("FragmentShader_list");

static const std::string SHADER_SOURCE = std::string("source");
static const std::string SHADER = std::string("shader");

static const std::string MODEL_LIST = std::string("Model_list");
static const std::string POSITION = std::string("position");
static const std::string NORMAL = std::string("normal");
static const std::string INDICES = std::string("indices");
static const std::string UV = std::string("uv");

static const std::string GLPROPERTY = std::string("gl_property");
static const std::string MATERIAL = std::string("material");
static const std::string MODEL = std::string("model");
static const std::string MESH = std::string("mesh");
static const std::string PROGRAM = std::string("program");
static const std::string BLEND_INDENSITY = std::string("blend_indensity");

static const std::string TARGET = std::string("target");
static const std::string TEXTURE_PARAM = std::string("texture_param");
static const std::string FUNC = std::string("func");
static const std::string VALUE = std::string("value");

static const std::string WRAP = std::string("wrap");
static const std::string WRAP_S = std::string("wrap_s");
static const std::string WRAP_T = std::string("wrap_t");

static const std::string FILTER = std::string("filter");

static const std::string MINIFYING = std::string("minifying");
static const std::string MAGNIFYING = std::string("magnifying");

static const std::string CAM_POSITION = std::string("cam position");
static const std::string CAM_FRONT = std::string("cam front");
static const std::string CAM_TARGET = std::string("cam target");
static const std::string CAM_UP = std::string("cam up");
static const std::string FOV = std::string("fov");
static const std::string NEAR_PLANE = std::string("near plane");
static const std::string FAR_PLANE = std::string("far plane");
static const std::string CAMERA = std::string("camera");

static picojson::value snull;

std::string get(const picojson::value& o)
{
	std::string str;
	if (snull != o)
	{
		str = o.get<std::string>();
	}
	return str;
}

void LoadPropertyTypeList(picojson::value& json_value)
{
	picojson::object o_list = json_value.get(PROPERTY_TYPE_LIST).get<picojson::object>();
	for (auto& item : o_list)
	{
		std::string name = item.first;
		std::string type = item.second.get<std::string>();
		if (property_type_map.find(type) != property_type_map.end())
		{
			Library::GetInstance()->add<PropertyType>(name, PropertyType::create(property_type_map.at(type)));
		}
	}
}

void LoadGLProperty(picojson::object& o_glProperty, Node3DPtr pObject)
{
	auto glMethod = pObject->GetPropertyMethodObj<GLProperty>();

	//<set model
	if (o_glProperty.end() != o_glProperty.find(MODEL))
	{
		std::string model_name = o_glProperty[MODEL].get<std::string>();
		pObject->SetModel(model_name);
	}

	//<set program
	if (o_glProperty.end() != o_glProperty.find(PROGRAM))
	{
		std::string program_name = o_glProperty[PROGRAM].get<std::string>();
		pObject->SetProgram(program_name);
	}

	//<set indensity
	if (o_glProperty.end() != o_glProperty.find(BLEND_INDENSITY))
	{
		double indensity = o_glProperty[BLEND_INDENSITY].get<double>();
		glMethod->SetBlendIndensity(static_cast<float>(indensity));
	}

	//<set Cam position
	if (o_glProperty.end() != o_glProperty.find(CAM_POSITION))
	{
		glm::vec3 pos;
		std::vector<float> vec3;
		picojson::array o_vec3_list = o_glProperty[CAM_POSITION].get<picojson::array>();
		for (picojson::value item : o_vec3_list)
		{
			vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
		}
		pos.x = vec3[0];
		pos.y = vec3[1];
		pos.z = vec3[2];
		glMethod->SetCamPos(pos);
	}

	//<set Cam direction
	if (o_glProperty.end() != o_glProperty.find(CAM_FRONT))
	{
		glm::vec3 front;
		std::vector<float> vec3;
		picojson::array o_vec3_list = o_glProperty[CAM_FRONT].get<picojson::array>();
		for (picojson::value item : o_vec3_list)
		{
			vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
		}
		front.x = vec3[0];
		front.y = vec3[1];
		front.z = vec3[2];
		glMethod->SetCamFront(front);
	}


	//<Set Cam target
	if (o_glProperty.end() != o_glProperty.find(CAM_TARGET))
	{
		glm::vec3 target;
		std::vector<float> vec3;
		picojson::array o_vec3_list = o_glProperty[CAM_TARGET].get<picojson::array>();
		for (picojson::value item : o_vec3_list)
		{
			vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
		}
		target.x = vec3[0];
		target.y = vec3[1];
		target.z = vec3[2];
		glMethod->SetCamTarget(target);
	}

	//<set Cam up
	if (o_glProperty.end() != o_glProperty.find(CAM_UP))
	{
		glm::vec3 up;
		std::vector<float> vec3;
		picojson::array o_vec3_list = o_glProperty[CAM_UP].get<picojson::array>();
		for (picojson::value item : o_vec3_list)
		{
			vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
		}
		up.x = vec3[0];
		up.y = vec3[1];
		up.z = vec3[2];
		glMethod->SetCamUp(up);
	}
	//<set Camera type
	if (o_glProperty.end() != o_glProperty.find(CAMERA))
	{
		std::string cam_type = o_glProperty[CAMERA].get<std::string>();
		glMethod->SetCameraType(cameraMap.at(cam_type));
	}

	//<set FOV
	if (o_glProperty.end() != o_glProperty.find(FOV))
	{
		glMethod->SetFOV(DOUBLE2FLOAT(o_glProperty[FOV].get<double>()));
	}

	//<set near/far plane
	if (o_glProperty.end() != o_glProperty.find(NEAR_PLANE))
	{
		glMethod->SetNearPlane(DOUBLE2FLOAT(o_glProperty[NEAR_PLANE].get<double>()));
	}
	if (o_glProperty.end() != o_glProperty.find(FAR_PLANE))
	{
		glMethod->SetFarPlane(DOUBLE2FLOAT(o_glProperty[FAR_PLANE].get<double>()));
	}
}

void LoadMaterialPropertyList(picojson::array& o_array, Node3DPtr pObject)
{
	auto pMaterial = Material::create(pObject->getName());
	for (picojson::array::iterator iter = o_array.begin(); iter != o_array.end(); ++iter)
	{
		std::string name = get(iter->get(PROPERTY_NAME));
		uint8_t uType = UNDEFINE;
		auto pType = Library::GetInstance()->get<PropertyType>(name);
		if (nullptr != pType)
		{
			uType = pType->m_type;
		}

		if (INT == uType)
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			std::string upper = get(iter->get(PROPERTY_UPPER));
			std::string lower = get(iter->get(PROPERTY_LOWER));
			int iUpper = ConvertToType<int>(upper);
			int iLower = ConvertToType<int>(lower);
			auto PropertyPtr = Property<int>::create(name, ConvertToType<int>(value), uType);
			PropertyPtr->setUpper(iUpper);
			PropertyPtr->setLower(iLower);
			pMaterial->AddProperty(name, PropertyPtr);
		}

		if (BOOL == uType)
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			pMaterial->AddProperty(name, Property<bool>::create(name, ConvertToType<bool>(value), uType));
		}

		if (FLOAT == uType)
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			std::string upper = get(iter->get(PROPERTY_UPPER));
			std::string lower = get(iter->get(PROPERTY_LOWER));
			float iUpper = ConvertToType<float>(upper);
			float iLower = ConvertToType<float>(lower);
			auto PropertyPtr = Property<float>::create(name, ConvertToType<float>(value), uType);
			PropertyPtr->setUpper(iUpper);
			PropertyPtr->setLower(iLower);
			pMaterial->AddProperty(name, PropertyPtr);
		}

		if (VEC4 == uType)
		{
			glm::vec4 value;
			std::vector<float> vec4;
			picojson::array o_vec4_list = iter->get(PROPERTY_VALUE).get<picojson::array>();
			for (picojson::value item : o_vec4_list)
			{
				vec4.push_back(DOUBLE2FLOAT(item.get<double>()));
			}
			value.x = vec4[0];
			value.y = vec4[1];
			value.z = vec4[2];
			value.w = vec4[3];
			pMaterial->AddProperty(name, Property<glm::vec4>::create(name, std::move(value), uType));
		}

		if (VEC3 == uType)
		{
			glm::vec3 value;
			std::vector<float> vec3;
			picojson::array o_vec3_list = iter->get(PROPERTY_VALUE).get<picojson::array>();
			for (picojson::value item : o_vec3_list)
			{
				vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
			}
			value.x = vec3[0];
			value.y = vec3[1];
			value.z = vec3[2];
			pMaterial->AddProperty(name, Property<glm::vec3>::create(name, std::move(value), uType));
		}

		if (VEC2 == uType)
		{
			glm::vec2 value;
			std::vector<float> vec2;
			picojson::array o_vec2_list = iter->get(PROPERTY_VALUE).get<picojson::array>();
			for (picojson::value item : o_vec2_list)
			{
				vec2.push_back(DOUBLE2FLOAT(item.get<double>()));
			}
			value.x = vec2[0];
			value.y = vec2[1];
			pMaterial->AddProperty(name, Property<glm::vec2>::create(name, std::move(value), uType));
		}

		if (URL_TEXTURE == uType)
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			//<set texture unit
			pMaterial->AddTexture(name, Library::GetInstance()->get<GLTexture>(value));
		}

		if ((STRING == uType) ||
			(URL_SHADER == uType) ||
			(URL_PROGRAM == uType) ||
			(URL_MESH == uType) ||
			(URL_MODEL == uType))
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			pMaterial->AddProperty(name, Property<std::string>::create(name, std::move(value), uType));
		}

		if (ARRAY_INT == uType)
		{
			std::vector<int> value;
			picojson::array o_list = iter->get(PROPERTY_VALUE).get<picojson::array>();
			for (picojson::value item : o_list)
			{
				value.push_back(DOUBLE2INT(item.get<double>()));
			}
			pMaterial->AddProperty(name, Property<std::vector<int>>::create(name, std::move(value), uType));
		}
	}
	Library::GetInstance()->add<Material>(pMaterial->getName(), pMaterial);
	pObject->SetMaterial(pMaterial->getName());
}

void LoadTextureList(picojson::value& json_value)
{
	//<get texture list
	picojson::array o_texture_list = json_value.get(TEXTURE_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_texture_list.begin(); iter != o_texture_list.end(); ++iter)
	{
		std::string texture_name = iter->get(TEXTURE_NAME).get<std::string>();
		std::string texture_path = Configuration::GetInstance()->asset_path + iter->get(TEXTURE_PATH).get<std::string>();
		bool loadtoGPU = iter->get(LOAD_TO_GPU).get<bool>();
#ifdef OPENGL_RENDERING
		std::string target = iter->get(TARGET).get<std::string>();
		auto pGLTexture = GLTexture::create(texture_name, texture_path, GL_TEXTURE_TARGET_MAP.at(target));

		//<add texture param
		if (snull != iter->get(WRAP))
		{
			picojson::object o_wrap = iter->get(WRAP).get<picojson::object>();
			if (o_wrap.end() != o_wrap.find(WRAP_S))
			{
				GLenum param = GL_TEXTURE_PVALUE_MAP.at(o_wrap[WRAP_S].get<std::string>());
				pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_S, param);
			}
			if (o_wrap.end() != o_wrap.find(WRAP_T))
			{
				GLenum param = GL_TEXTURE_PVALUE_MAP.at(o_wrap[WRAP_T].get<std::string>());
				pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_T, param);
			}
		}

		if (snull != iter->get(FILTER))
		{
			picojson::object o_filter = iter->get(FILTER).get<picojson::object>();
			if (o_filter.end() != o_filter.find(MINIFYING))
			{
				GLenum param = GL_TEXTURE_PVALUE_MAP.at(o_filter[MINIFYING].get<std::string>());
				pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MIN_FILTER, param);
	}
			if (o_filter.end() != o_filter.find(MAGNIFYING))
			{
				GLenum param = GL_TEXTURE_PVALUE_MAP.at(o_filter[MAGNIFYING].get<std::string>());
				pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MAG_FILTER, param);
			}
}

		pGLTexture->loadToGPU(loadtoGPU);
		pGLTexture->init();
		Library::GetInstance()->add<GLTexture>(texture_name, pGLTexture);
#else
		auto pTexture = Texture::create(Render::GetInstance()->getRenderer(), texture_path);
		pTexture->LoadToGPU(loadtoGPU);
		Library::GetInstance()->add<Texture>(texture_name, pTexture);
#endif
	}
}

void LoadResourceList(UIObjectTableType& UIObjectTable, picojson::value& json_value)
{
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
		uint8_t type = std::stoi(iter->get(UI_TYPE).get<std::string>()) & 0xFF;
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
		case NODE_3D:
			pObject = Node3D::create(resource_name);
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

			//<Set layout transform
			if (o_layout.end() != o_layout.find(TRANSFORM))
			{
				glm::vec3 vTransform;
				std::vector<float> vec3;
				picojson::array o_vec3_list = o_layout[TRANSFORM].get<picojson::array>();
				for (picojson::value item : o_vec3_list)
				{
					vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
				}
				vTransform.x = vec3[0];
				vTransform.y = vec3[1];
				vTransform.z = vec3[2];
				layoutMethod->SetLayoutTransform(vTransform);
			}

			//<Set layout rotation
			if (o_layout.end() != o_layout.find(ROTATE))
			{
				glm::vec3 vRotate;
				std::vector<float> vec3;
				picojson::array o_vec3_list = o_layout[ROTATE].get<picojson::array>();
				for (picojson::value item : o_vec3_list)
				{
					vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
				}
				vRotate.x = vec3[0];
				vRotate.y = vec3[1];
				vRotate.z = vec3[2];
				layoutMethod->SetRotation(vRotate);
			}

			//<Set layout scale
			if (o_layout.end() != o_layout.find(SCALE))
			{
				glm::vec3 vScale;
				std::vector<float> vec3;
				picojson::array o_vec3_list = o_layout[SCALE].get<picojson::array>();
				for (picojson::value item : o_vec3_list)
				{
					vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
				}
				vScale.x = vec3[0];
				vScale.y = vec3[1];
				vScale.z = vec3[2];
				layoutMethod->SetLayoutScale(vScale);
			}
		}

		//<set origin property
		if (snull != iter->get(ORIGINAL_PROPERTY))
		{
			OriginPropertyPtr originMethod = pObject->GetPropertyMethodObj<OriginProperty>();

			//<Set origin property
			picojson::object o_origin_property = iter->get(ORIGINAL_PROPERTY).get<picojson::object>();

			//Visible
			originMethod->SetVisible(o_origin_property[VISIBLE].get<bool>());

			//Pivot point
			picojson::object o_center_point = o_origin_property[CENTER_POINT].get<picojson::object>();
			originMethod->SetCenterPoint(
				DOUBLE2INT(o_center_point[X_COODINATOR].get<double>()),
				DOUBLE2INT(o_center_point[Y_COODINATOR].get<double>()));

			//Flip, Opacity, Angle, Clip
			originMethod->SetFlip(SDL_RendererFlip(DOUBLE2INT(o_origin_property[FLIP].get<double>())));
			originMethod->SetOpacity(DOUBLE2INT(o_origin_property[OPACITY].get<double>()));
			originMethod->SetAngle(o_origin_property[ANGLE].get<double>());
			originMethod->SetClip(o_origin_property[ISCLIP].get<bool>());

			//Foreground color
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

			//Background color
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

		if (NODE_3D == type)
		{
			//set gl property
			if (snull != iter->get(GLPROPERTY))
			{
				picojson::object o_glProperty = iter->get(GLPROPERTY).get<picojson::object>();
				LoadGLProperty(o_glProperty, std::dynamic_pointer_cast<Node3D>(pObject));
			}

			//<Set material property list
			if (snull != iter->get(MATERIAL_PROPERTY_LIST))
			{
				picojson::array o_array = iter->get(MATERIAL_PROPERTY_LIST).get<picojson::array>();
				LoadMaterialPropertyList(o_array, std::dynamic_pointer_cast<Node3D>(pObject));
			}
		}

		//<add to ui object table
		UIObjectTable[resource_name] = pObject;
		LOG_DEBUG("Resource added[%s]", resource_name.c_str());
	}
}

void LoadFragmentShaderList(picojson::value& json_value)
{

	//<get fragment shader list
	picojson::array o_fragment_list = json_value.get(FRAG_SHADER_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_fragment_list.begin(); iter != o_fragment_list.end(); ++iter)
	{
		std::string shader_name = iter->get(NAME).get<std::string>();
		std::string shader_source = Configuration::GetInstance()->shader_path + iter->get(SHADER_SOURCE).get<std::string>();
		auto pShader = Shader::create(shader_name, GL_FRAGMENT_SHADER);

		//<Open shader source file
		std::ifstream inShaderStream(shader_source.c_str());

		//<Validate ifstream
		_ASSERT(inShaderStream);

		//<Read to string
		std::string inShaderString((std::istreambuf_iterator<char>(inShaderStream)), std::istreambuf_iterator<char>());

		//<Attach source
		const char* const pChar = inShaderString.c_str();
		pShader->setSource(1, &pChar, NULL);

		//<Compile shader
		pShader->compile();

		//<Add to library
		Library::GetInstance()->add<Shader>(shader_name, pShader);
	}

}

void LoadVertexShaderList(picojson::value& json_value)
{
	//<get vertex shader list
	picojson::array o_vertex_list = json_value.get(VEX_SHADER_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_vertex_list.begin(); iter != o_vertex_list.end(); ++iter)
	{
		std::string shader_name = iter->get(NAME).get<std::string>();
		std::string shader_source = Configuration::GetInstance()->shader_path + iter->get(SHADER_SOURCE).get<std::string>();

		auto pShader = Shader::create(shader_name, GL_VERTEX_SHADER);

		//<Open shader source file
		std::ifstream inShaderStream(shader_source.c_str());

		//<Validate ifstream
		_ASSERT(inShaderStream);

		//<Read to string
		std::string inShaderString((std::istreambuf_iterator<char>(inShaderStream)), std::istreambuf_iterator<char>());

		//<Attach source
		const char* const pChar = inShaderString.c_str();
		pShader->setSource(1, &pChar, NULL);

		//<Compile shader
		pShader->compile();

		//<Add to library
		Library::GetInstance()->add<Shader>(shader_name, pShader);
	}

}

void LoadShaderProgramList(picojson::value& json_value)
{
	//<get shader program list
	picojson::array o_shaderProgram_list = json_value.get(SHADER_PROGRAM_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_shaderProgram_list.begin(); iter != o_shaderProgram_list.end(); ++iter)
	{
		std::string program_name = iter->get(NAME).get<std::string>();
		auto pShaderProgram = ShaderProgram::create(program_name);

		picojson::array shader_list = iter->get(SHADER).get<picojson::array>();

		//<get shader name list
		for (picojson::value item : shader_list)
		{
			std::string shader_name = item.get<std::string>();
			//<Attach shader to program
			pShaderProgram->attachShader(Library::GetInstance()->get<Shader>(shader_name));
		}

		//<Link to program
		pShaderProgram->linkProgram();

		//<Debug program
		pShaderProgram->debug();

		//<Add to library
		Library::GetInstance()->add<ShaderProgram>(program_name, pShaderProgram);

		//<Write shader binary to storage
		pShaderProgram->write();
	}
}

void LoadModel(picojson::value& json_value)
{
	std::string obj_file;
	picojson::array o_model_list = json_value.get(THREE_D_ASSET_LIST).get<picojson::array>();
	for (picojson::value item : o_model_list)
	{
		obj_file = Configuration::GetInstance()->asset_path + item.get<std::string>();

		auto pModel = Model::create(obj_file.c_str());
		Library::GetInstance()->add<Model>(pModel->getName(), pModel);
	}
}

void LoadMeshList(picojson::value& json_value)
{
	//<get vertex data
	picojson::array o_vertex_data_list = json_value.get(MODEL_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_vertex_data_list.begin(); iter != o_vertex_data_list.end(); ++iter)
	{
		std::string name = iter->get(NAME).get<std::string>();

		picojson::array o_position_list;
		picojson::array o_normal_list;
		picojson::array o_uv_list;
		if (snull != iter->get(POSITION))
		{
			o_position_list = iter->get(POSITION).get<picojson::array>();
		}
		if (snull != iter->get(NORMAL))
		{
			o_normal_list = iter->get(NORMAL).get<picojson::array>();
		}
		if (snull != iter->get(UV))
		{
			o_uv_list = iter->get(UV).get<picojson::array>();
		}

		std::vector<VertexRecord> vertices;
		std::vector<TextureRecord> textures;

		size_t pos = 0;
		size_t uv = 0;
		while (pos < o_position_list.size())
		{
			VertexRecord vertex;
			glm::vec3 vector;

			// process vertex positions, normals and texture coordinates 
			vector.x = o_position_list[pos].get<double>();
			vector.y = o_position_list[pos + 1].get<double>();
			vector.z = o_position_list[pos + 2].get<double>();
			vertex.Position = vector;

			if (0 < o_normal_list.size())
			{
				vector.x = o_normal_list[pos].get<double>();
				vector.y = o_normal_list[pos + 1].get<double>();
				vector.z = o_normal_list[pos + 2].get<double>();
				vertex.Normal = vector;
			}
			//does the mesh contain texture coordinates?
			glm::vec2 vec;
			if (0 < o_uv_list.size())
			{
				vec.x = o_uv_list[uv].get<double>();
				vec.y = o_uv_list[uv + 1].get<double>();
				vertex.TexCoords = vec;
			}
			vertices.push_back(vertex);
			pos += 3;
			uv += 2;
		}

		// process indices
		std::vector<unsigned int> indices;
		if (snull != iter->get(INDICES))
		{
			picojson::array o_indices_list = iter->get(INDICES).get<picojson::array>();
			for (picojson::value item : o_indices_list)
			{
				indices.push_back(DOUBLE2UINT32(item.get<double>()));
			}
		}

		// process material

		auto pMesh = Mesh::create(vertices, indices, textures);
		auto pModel = Model::create(name, std::vector<MeshPtr>{pMesh});
		pModel->debug();
		//<Add to library
		Library::GetInstance()->add<Model>(name, pModel);
	}

}