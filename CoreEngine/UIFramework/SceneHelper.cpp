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
#include "VertexAttribute.h"

#include "GLTypeDictionary.h"

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

static const std::string CUSTOM_PROPERTY_LIST = std::string("custom_property");
static const std::string PROPERTY_NAME = std::string("property_name");
static const std::string PROPERTY_TYPE = std::string("type");
static const std::string PROPERTY_VALUE = std::string("value");
static const std::string PROPERTY_UPPER = std::string("upper");
static const std::string PROPERTY_LOWER = std::string("lower");

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

static const std::string MESH_LIST = std::string("Mesh_list");
static const std::string VERTICES = std::string("vertices");
static const std::string INDICES = std::string("indices");
static const std::string USAGE = std::string("usage");

static const std::string GLPROPERTY = std::string("gl_property");
static const std::string MATERIAL = std::string("material");
static const std::string MESH = std::string("mesh");
static const std::string PROGRAM = std::string("program");
static const std::string BLEND_INDENSITY = std::string("blend_indensity");

static const std::string ATTRIBUTE = std::string("attribute");
static const std::string INDEX = std::string("index");
static const std::string TYPE = std::string("type");
static const std::string NORMALIZED = std::string("normalized");
static const std::string SIZE = std::string("size");
static const std::string STRIDE = std::string("stride");
static const std::string OFFSET = std::string("offset");

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
	for (auto &item : o_list)
	{
		std::string name = item.first;
		std::string type = item.second.get<std::string>();
		if (property_type_map.find(type) != property_type_map.end())
		{
			Library::GetInstance()->add<PropertyType>(name, PropertyType::create(property_type_map.at(type)));
		}
	}
}

void LoadCustomPropertyList(picojson::array& o_array, UIObjectPtr pObject)
{
	auto glMethod = pObject->GetPropertyMethodObj<GLProperty>();
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
			pObject->AddProperty(name, PropertyPtr);
		}

		if (BOOL == uType)
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			pObject->AddProperty(name, Property<bool>::create(name, ConvertToType<bool>(value), uType));
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
			pObject->AddProperty(name, PropertyPtr);
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
			pObject->AddProperty(name, Property<glm::vec4>::create(name, std::move(value), uType));
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
			pObject->AddProperty(name, Property<glm::vec3>::create(name, std::move(value), uType));
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
			pObject->AddProperty(name, Property<glm::vec2>::create(name, std::move(value), uType));
		}

		if (URL_TEXTURE == uType)
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			pObject->AddProperty(name, Property<std::string>::create(name, std::move(value), uType));
			//<set texture unit
			glMethod->AddTexture(Library::GetInstance()->get<GLTexture>(value));
		}

		if ((STRING == uType) ||
			(URL_SHADER == uType) ||
			(URL_PROGRAM == uType) ||
			(URL_MESH == uType))
		{
			std::string value = get(iter->get(PROPERTY_VALUE));
			pObject->AddProperty(name, Property<std::string>::create(name, std::move(value), uType));
		}

		if (ARRAY_INT == uType)
		{
			std::vector<int> value;
			picojson::array o_list = iter->get(PROPERTY_VALUE).get<picojson::array>();
			for (picojson::value item : o_list)
			{
				value.push_back(DOUBLE2INT(item.get<double>()));
			}
			pObject->AddProperty(name, Property<std::vector<int>>::create(name, std::move(value), uType));
		}
	}
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
			if (snull != o_wrap.at(WRAP_S))
			{
				GLenum param = GL_TEXTURE_PVALUE_MAP.at(o_wrap[WRAP_S].get<std::string>());
				pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_S, param);
			}
			if (snull != o_wrap.at(WRAP_T))
			{
				GLenum param = GL_TEXTURE_PVALUE_MAP.at(o_wrap[WRAP_T].get<std::string>());
				pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_T, param);
			}
		}
	
		if (snull != iter->get(FILTER))
		{
			picojson::object o_filter = iter->get(FILTER).get<picojson::object>();
			if (snull != o_filter.at(MINIFYING))
			{
				GLenum param = GL_TEXTURE_PVALUE_MAP.at(o_filter[MINIFYING].get<std::string>());
				pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MIN_FILTER, param);
			}
			if (snull != o_filter.at(MAGNIFYING))
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
#ifndef OPENGL_RENDERING
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
#else
		case NODE_3D:
			pObject = Node3D::create(resource_name);
			break;
#endif
		default:
			continue;
			break;
		}
#ifndef OPENGL_RENDERING
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
#else
		if (NODE_3D == type)
		{
			//set layout property
			if (snull != iter->get(LAYOUT))
			{
				picojson::object o_layout = iter->get(LAYOUT).get<picojson::object>();
				auto layoutMethod = pObject->GetPropertyMethodObj<LayoutProperty>();

				//<Set layout width and height
				layoutMethod->SetLayoutWidth(DOUBLE2INT(o_layout[WIDTH].get<double>()));
				layoutMethod->SetLayoutHeight(DOUBLE2INT(o_layout[HEIGHT].get<double>()));

				//<Set layout transform
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

				//<Set layout rotation
				glm::vec3 vRotate;
				vec3.clear();
				o_vec3_list = o_layout[ROTATE].get<picojson::array>();
				for (picojson::value item : o_vec3_list)
				{
					vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
				}
				vRotate.x = vec3[0];
				vRotate.y = vec3[1];
				vRotate.z = vec3[2];
				layoutMethod->SetRotation(vRotate);

				//<Set layout scale
				glm::vec3 vScale;
				vec3.clear();
				o_vec3_list = o_layout[SCALE].get<picojson::array>();
				for (picojson::value item : o_vec3_list)
				{
					vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
				}
				vScale.x = vec3[0];
				vScale.y = vec3[1];
				vScale.z = vec3[2];
				layoutMethod->SetLayoutScale(vScale);

			}

			//<set origin property
			if (snull != iter->get(ORIGINAL_PROPERTY))
			{
				OriginPropertyPtr originMethod = pObject->GetPropertyMethodObj<OriginProperty>();

				//<Set origin property
				picojson::object o_origin_property = iter->get(ORIGINAL_PROPERTY).get<picojson::object>();
				picojson::object o_center_point = o_origin_property[CENTER_POINT].get<picojson::object>();
				originMethod->SetVisible(o_origin_property[VISIBLE].get<bool>());

				originMethod->SetOpacity(DOUBLE2INT(o_origin_property[OPACITY].get<double>()));
				originMethod->SetAngle(o_origin_property[ANGLE].get<double>());
				originMethod->SetClip(o_origin_property[ISCLIP].get<bool>());
			}

			//set gl property
			if (snull != iter->get(GLPROPERTY))
			{
				picojson::object o_glProperty = iter->get(GLPROPERTY).get<picojson::object>();
				auto glMethod = pObject->GetPropertyMethodObj<GLProperty>();

				//<set program
				std::string program_name = o_glProperty[PROGRAM].get<std::string>();
				std::dynamic_pointer_cast<Node3D>(pObject)->SetProgram(program_name);

				//<set mesh
				std::string mesh_name = o_glProperty[MESH].get<std::string>();
				std::dynamic_pointer_cast<Node3D>(pObject)->SetMesh(mesh_name);

				//<set indensity
				double indensity = o_glProperty[BLEND_INDENSITY].get<double>();
				glMethod->SetBlendIndensity(static_cast<float>(indensity));
			}

			//<Set custome property list
			if (snull != iter->get(CUSTOM_PROPERTY_LIST))
			{
				picojson::array o_array = iter->get(CUSTOM_PROPERTY_LIST).get<picojson::array>();
				LoadCustomPropertyList(o_array, pObject);
			}
		}
#endif
		
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

void LoadMeshList(picojson::value& json_value)
{
	//<get vertex data
	picojson::array o_vertex_data_list = json_value.get(MESH_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_vertex_data_list.begin(); iter != o_vertex_data_list.end(); ++iter)
	{
		std::string name = iter->get(NAME).get<std::string>();
		GLenum usage;
		switch(std::stoi(iter->get(USAGE).get<std::string>()))
		{
			case 0:
			usage = GL_STATIC_DRAW;
			break;
			case 1:
			usage = GL_DYNAMIC_DRAW;
			break;
			default:
			usage = GL_STATIC_DRAW;
			break;
		}
		std::vector<float> vertices;
		picojson::array o_vertices_list = iter->get(VERTICES).get<picojson::array>();
		for (picojson::value item : o_vertices_list)
		{
			vertices.push_back(static_cast<float>(item.get<double>()));
		}

		std::vector<uint32_t> indices;
		if (snull != iter->get(INDICES))
		{
			picojson::array o_indices_list = iter->get(INDICES).get<picojson::array>();
			for (picojson::value item : o_indices_list)
			{
				indices.push_back(DOUBLE2UINT32(item.get<double>()));
			}
		}

		auto pVertexData = VertexData::create(std::move(vertices), std::move(indices), usage);
		auto pMesh = Mesh::create(name, pVertexData);

		picojson::array o_attribute_list = iter->get(ATTRIBUTE).get<picojson::array>();
		for (picojson::array::iterator iter = o_attribute_list.begin(); iter != o_attribute_list.end(); ++iter)
		{
			std::string name = iter->get(NAME).get<std::string>();

			GLuint index = DOUBLE2UINT32(iter->get(INDEX).get<double>());

			GLenum type = ENUM_TYPE_FROMSTRING.at(iter->get(TYPE).get<std::string>());

			size_t sizeOfType = SIZE_TYPE_FROMSTRING.at(iter->get(TYPE).get<std::string>());

			GLboolean isNormalize = iter->get(NORMALIZED).get<bool>() ? GL_TRUE: GL_FALSE;

			GLint size = DOUBLE2INT(iter->get(SIZE).get<double>());

			GLsizei stride = DOUBLE2INT(iter->get(STRIDE).get<double>());
			GLsizei offset = DOUBLE2INT(iter->get(OFFSET).get<double>());
			auto pVertexAttribute = VertexAttribute::create(name, index, size, type, isNormalize, stride * sizeOfType, (void*)(offset*sizeOfType));
			pVertexAttribute->debug();
			pMesh->addAttribute(pVertexAttribute);
		}
		pMesh->init();
		pMesh->debug();
		//<Add to library
		Library::GetInstance()->add<Mesh>(name, pMesh);
	}

}