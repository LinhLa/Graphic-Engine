#include "stdafx.h"
#include "utils.h"
using namespace util;

#include "SceneHelper.h"
#include "Configuration.h"
#include "log.h"
#include "Render.h"
#include "GLRenderManipulator.h"

#include <fstream>
#include <math.h>

#include "Library.h"

#include "Node2DImage.h"
#include "NodeButton2D.h"
#include "NodeText2D.h"
#include "EmptyNode.h"
#include "Node3D.h"
#include "NodeMesh.h"
#include "NodeLight.h"
#include "NodeCamera.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "TextProperty.h"
#include "GLProperty.h"
#include "MaterialProperty.h"
#include "CameraProperty.h"

#include "Shader.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Model.h"
#include "Material.h"

#include "GLTypeDictionary.h"
#include "colladainterface.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "PropertyDefine.h"
#include "ModelLoader.h"

int32_t DOUBLE2INT(double X) {return int(rint(X));}
uint8_t DOUBLE2UINT8(double X) {return uint8_t(rint(X));}
uint32_t DOUBLE2UINT32(double X) {return uint32_t(rint(X));}
float DOUBLE2FLOAT(double X) {return static_cast<float>(X);}

static const std::string PROPERTY_TYPE_LIST = std::string("property_type_list");
static const std::string PROPERTY_LIST = std::string("property_list");

static const std::string TEXTURE_LIST = std::string("texture_list");
static const std::string RESOURCES_LIST = std::string("resources_list");

static const std::string TEXTURE_PATH = std::string("texture_path");

static const std::string RESOURCE_NAME = std::string("resource_name");
static const std::string TEXTURE_NAME = std::string("texture_name");

static const std::string TYPE = std::string("type");
static const std::string LAYOUT = std::string("layout");

static const std::string TRANSFORM = std::string("transform");
static const std::string ROTATE = std::string("rotate");
static const std::string SCALE = std::string("scale");

static const std::string ORIGINAL_PROPERTY = std::string("origin_property");
static const std::string LOAD_TO_GPU = std::string("load_to_gpu");

static const std::string ISCLIP = std::string("isclip");

static const std::string TEXT_PROPERTY = std::string("text_property");

static const std::string MATERIAL_PROPERTY_LIST = std::string("material_property");
static const std::string LIGHTING_LIST = std::string("lighting");
static const std::string MATERIAL_LIST = std::string("Material_list");

static const std::string PROPERTY_NAME = std::string("property_name");
static const std::string PROPERTY_TYPE = std::string("type");
static const std::string PROPERTY_VALUE = std::string("value");
static const std::string PROPERTY_UPPER = std::string("upper");
static const std::string PROPERTY_LOWER = std::string("lower");

static const std::string THREE_D_ASSET_LIST = std::string("3D_asset_list");
static const std::string NAME = std::string("name");

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

static const std::string TARGET = std::string("target");
static const std::string TEXTURE_PARAM = std::string("texture_param");
static const std::string FUNC = std::string("func");
static const std::string VALUE = std::string("value");

static const std::string WRAP = std::string("wrap");
static const std::string FILTER = std::string("filter");

static const std::string CAMERA = std::string("Camera");

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

glm::vec2 GLMVec2(picojson::array& o_vec2_list)
{
	glm::vec2 value;
	std::vector<float> vec2;
	for (picojson::value item : o_vec2_list)
	{
		vec2.push_back(DOUBLE2FLOAT(item.get<double>()));
	}
	value.x = vec2[0];
	value.y = vec2[1];
	return value;
}

glm::vec3 GLMVec3(picojson::array& o_vec3_list)
{
	glm::vec3 value;
	std::vector<float> vec3;
	for (picojson::value item : o_vec3_list)
	{
		vec3.push_back(DOUBLE2FLOAT(item.get<double>()));
	}
	value.x = vec3[0];
	value.y = vec3[1];
	value.z = vec3[2];
	return value;
}

//glm::vec4 GLMVec4(picojson::array& o_vec4_list)
//{
//	glm::vec4 value;
//	std::vector<float> vec4;
//	for (picojson::value item : o_vec4_list)
//	{
//		vec4.push_back(DOUBLE2FLOAT(item.get<double>()));
//	}
//	value.x = vec4[0];
//	value.y = vec4[1];
//	value.z = vec4[2];
//	value.w = vec4[3];
//	return value;
//}

template<class T = float, class GLM_TYPE = glm::vec4>
GLM_TYPE GLMVec4(picojson::array& o_vec4_list, std::function<T(double)> converter = DOUBLE2FLOAT)
{
	GLM_TYPE value;
	std::vector<T> vec4;
	for (picojson::value item : o_vec4_list)
	{
		vec4.push_back(converter(item.get<double>()));
	}
	value.x = vec4[0];
	value.y = vec4[1];
	value.z = vec4[2];
	value.w = vec4[3];
	return value;
}

IPropertyPtr GetMaterialProperty(picojson::value& json_value, std::string name)
{
	GLenum uType = GL_INVALID_ENUM;
	IPropertyPtr pReturn = nullptr;
	std::string type = json_value.get(TYPE).get<std::string>();
	if (GLSL_TYPE_MAP.find(type) != GLSL_TYPE_MAP.end())
	{
		uType = GLSL_TYPE_MAP.at(type);
	}
	else
	{
		return  pReturn;
	}

	if (GL_INT == uType)
	{
		std::string value = json_value.get(PROPERTY_VALUE).get<std::string>();
		int iUpper = 0;
		int iLower = 100;

		if (snull != json_value.get(PROPERTY_UPPER))
		{
			iUpper = ConvertToType<float>(json_value.get(PROPERTY_UPPER).get<std::string>());
		}
		if (snull != json_value.get(PROPERTY_LOWER))
		{
			iLower = ConvertToType<float>(json_value.get(PROPERTY_LOWER).get<std::string>());
		}
		auto PropertyPtr = Property<int>::create(name, ConvertToType<int>(value), uType);
		PropertyPtr->setUpper(iUpper);
		PropertyPtr->setLower(iLower);
		pReturn = PropertyPtr;
	}

	if (GL_BOOL == uType)
	{
		std::string value = json_value.get(PROPERTY_VALUE).get<std::string>();
		pReturn = Property<bool>::create(name, ConvertToType<bool>(value), uType);
	}

	if (GL_FLOAT == uType)
	{
		std::string value = json_value.get(PROPERTY_VALUE).get<std::string>();
		float iUpper = 0.0f;
		float iLower = 1.0f;

		if (snull != json_value.get(PROPERTY_UPPER))
		{
			iUpper = ConvertToType<float>(json_value.get(PROPERTY_UPPER).get<std::string>());
		}
		if (snull != json_value.get(PROPERTY_LOWER))
		{
			iLower = ConvertToType<float>(json_value.get(PROPERTY_LOWER).get<std::string>());
		}

		auto PropertyPtr = Property<float>::create(name, ConvertToType<float>(value), uType);
		PropertyPtr->setUpper(iUpper);
		PropertyPtr->setLower(iLower);
		pReturn = PropertyPtr;
	}

	if (GL_FLOAT_VEC4 == uType)
	{
		picojson::array o_vec4_list = json_value.get(PROPERTY_VALUE).get<picojson::array>();
		pReturn = Property<glm::vec4>::create(name, std::move(GLMVec4(o_vec4_list)), uType);
	}

	if (GL_FLOAT_VEC3 == uType)
	{
		picojson::array o_vec3_list = json_value.get(PROPERTY_VALUE).get<picojson::array>();
		pReturn = Property<glm::vec3>::create(name, std::move(GLMVec3(o_vec3_list)), uType);
	}

	if (GL_FLOAT_VEC2 == uType)
	{
		picojson::array o_vec2_list = json_value.get(PROPERTY_VALUE).get<picojson::array>();
		pReturn = Property<glm::vec2>::create(name, std::move(GLMVec2(o_vec2_list)), uType);
	}

	if (GL_SAMPLER_2D == uType)
	{
		auto pTexture = Library::GetInstance()->get<GLTexture>(json_value.get(PROPERTY_VALUE).get<std::string>());
		pReturn = Property<glm::u32>::create(name, pTexture->getID(), uType);
	}
	return pReturn;
}

void LoadMaterialList(picojson::value& json_value)
{
	picojson::array o_list = json_value.get(MATERIAL_LIST).get<picojson::array>();
	for (auto& value : o_list)
	{
		auto pMaterial = Material::create(value.get(NAME).get<std::string>());
		picojson::array o_property_list = value.get(PROPERTY_LIST).get<picojson::array>();
		for (auto& value : o_property_list)
		{
			auto name = value.get<std::string>();
			auto property = Library::GetInstance()->get<IProperty>(name);
			if (property)
			{
				pMaterial->AddProperty(name, property);
			}
		}
		auto pShader = Library::GetInstance()->get<ShaderProgram>(value.get(PROGRAM).get<std::string>());
		if (!pShader)
		{
			pShader = Library::GetInstance()->get<ShaderProgram>(pMaterial->getName());
		}
		pMaterial->setShader(pShader->getName());
		Library::GetInstance()->add<Material>(pMaterial->getName(), pMaterial);
	}

}

void LoadPropertyTypeList(picojson::value& json_value)
{
	picojson::array o_list = json_value.get(PROPERTY_TYPE_LIST).get<picojson::array>();
	for (auto& value : o_list)
	{
		std::string name = value.get(NAME).get<std::string>();
		auto property = GetMaterialProperty(value, name);
		if (property)
		{
			Library::GetInstance()->add<IProperty>(name, property);
		}
	}
}

void LoadGLProperty(picojson::object& o_glProperty, Node3DPtr pObject)
{
	auto glMethod = pObject->GetPropertyMethodObj<GLProperty>();
	//<set indensity
	if (o_glProperty.end() != o_glProperty.find(BLEND_INDENSITY))
	{
		double indensity = o_glProperty[BLEND_INDENSITY].get<double>();
		glMethod->SetBlendIndensity(static_cast<float>(indensity));
	}

	//<Set viewport
	if (o_glProperty.end() != o_glProperty.find(VIEW_PORT))
	{
		picojson::array o_vec4_list = o_glProperty[VIEW_PORT].get<picojson::array>();
		glMethod->SetViewPort(GLMVec4<glm::i32, glm::i32vec4>(o_vec4_list, &DOUBLE2UINT32));
	}
}

void LoadCameraProperty(picojson::array& o_camera_list, Node3DPtr pObject)
{
	std::vector<UIObjectPtr> cameras;
	for (auto& o_camera : o_camera_list)
	{
		auto name = o_camera.get(NAME).get<std::string>();
		UIObjectPtr pCamNode = NodeCamera::create(name);	
		auto camMethod = pCamNode->GetPropertyMethodObj<CameraProperty>();
		
		//<set Cam position
		if (snull != o_camera.get(CAM_POSITION))
		{
			picojson::array o_vec3_list = o_camera.get(CAM_POSITION).get<picojson::array>();
			camMethod->SetCamPos(GLMVec3(o_vec3_list));
		}

		//<set Cam direction
		if (snull != o_camera.get(CAM_FRONT))
		{
			picojson::array o_vec3_list = o_camera.get(CAM_FRONT).get<picojson::array>();
			camMethod->SetCamFront(GLMVec3(o_vec3_list));
		}

		//<Set Cam target
		if (snull != o_camera.get(CAM_TARGET))
		{
			picojson::array o_vec3_list = o_camera.get(CAM_TARGET).get<picojson::array>();
			camMethod->SetCamTarget(GLMVec3(o_vec3_list));
		}

		//<set Cam up
		if (snull != o_camera.get(CAM_UP))
		{
			picojson::array o_vec3_list = o_camera.get(CAM_UP).get<picojson::array>();
			camMethod->SetCamUp(GLMVec3(o_vec3_list));
		}
		//<set Camera type
		if (snull != o_camera.get(CAMERA_TYPE))
		{
			std::string cam_type = o_camera.get(CAMERA_TYPE).get<std::string>();
			camMethod->SetCameraType(cameraMap.at(cam_type));
		}

		//<set FOV
		if (snull != o_camera.get(FOV))
		{
			camMethod->SetFOV(DOUBLE2FLOAT(o_camera.get(FOV).get<double>()));
		}

		//<set near/far plane
		if (snull != o_camera.get(NEAR_PLANE))
		{
			camMethod->SetNearPlane(DOUBLE2FLOAT(o_camera.get(NEAR_PLANE).get<double>()));
		}
		if (snull != o_camera.get(FAR_PLANE))
		{
			camMethod->SetFarPlane(DOUBLE2FLOAT(o_camera.get(FAR_PLANE).get<double>()));
		}
		cameras.push_back(pCamNode);
	}

	for (auto& node : cameras)
	{
		pObject->addChild(node);
	}

	//pObject->SetNodeCamera(cameras);
}

void LoadMaterialProperty(picojson::object& o_material, NodeMeshPtr pObject)
{
	auto material = pObject->getMesh()->getMaterial();
	auto pMaterial = Library::GetInstance()->get<Material>(material);
	if (!pMaterial)
	{
		pMaterial = Material::create(material);
		Library::GetInstance()->add<Material>(material, pMaterial);
		pObject->SetMaterial(material);
	}

	//<set material Specular
	if (o_material.end() != o_material.find(MATERIAL_SPECULAR))
	{
		picojson::array o_vec4_list = o_material[MATERIAL_SPECULAR].get<picojson::array>();
		pMaterial->AddProperty(MATERIAL_SPECULAR, Property<glm::vec4>::create(MATERIAL_SPECULAR, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set material Diffuse
	if (o_material.end() != o_material.find(MATERIAL_DIFFUSE))
	{
		picojson::array o_vec4_list = o_material[MATERIAL_DIFFUSE].get<picojson::array>();
		pMaterial->AddProperty(MATERIAL_DIFFUSE, Property<glm::vec4>::create(MATERIAL_DIFFUSE, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set material Ambient
	if (o_material.end() != o_material.find(MATERIAL_AMBIENT))
	{
		picojson::array o_vec4_list = o_material[MATERIAL_AMBIENT].get<picojson::array>();
		pMaterial->AddProperty(MATERIAL_AMBIENT, Property<glm::vec4>::create(MATERIAL_AMBIENT, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<material diffuse map
	if (o_material.end() != o_material.find(MATERIAL_DIFFUSE_MAP))
	{
		std::string value = get(o_material[MATERIAL_DIFFUSE_MAP]);
		//<set texture unit
		auto pTexture = Library::GetInstance()->get<GLTexture>(value);
		pMaterial->AddProperty(MATERIAL_DIFFUSE_MAP, Property<glm::u32>::create(MATERIAL_DIFFUSE_MAP, pTexture->getID(), GL_SAMPLER_2D));
		pMaterial->AddTexture(MATERIAL_DIFFUSE_MAP, Library::GetInstance()->get<GLTexture>(value));
	}

	//<material specular map
	if (o_material.end() != o_material.find(MATERIAL_SPECULAR_MAP))
	{
		std::string value = get(o_material[MATERIAL_SPECULAR_MAP]);
		//<set texture unit
		auto pTexture = Library::GetInstance()->get<GLTexture>(value);
		pMaterial->AddProperty(MATERIAL_SPECULAR_MAP, Property<glm::u32>::create(MATERIAL_SPECULAR_MAP, pTexture->getID(), GL_SAMPLER_2D));
		pMaterial->AddTexture(MATERIAL_SPECULAR_MAP, Library::GetInstance()->get<GLTexture>(value));
	}

	//<set material shininess
	if (o_material.end() != o_material.find(MATERIAL_SHININESS))
	{
		float value = DOUBLE2FLOAT(o_material[MATERIAL_SHININESS].get<double>());
		pMaterial->AddProperty(MATERIAL_SHININESS, Property<float>::create(MATERIAL_SHININESS, std::move(value), GL_FLOAT));
	}

	pObject->importProperty(pMaterial);
}

void LoadPointLight(picojson::value& o_Lighting, UIObjectPtr pLightNode)
{
	//<set light Pos
	if (snull != o_Lighting.get(POINT_LIGHT_POSITION))
	{
		picojson::array o_vec3_list = o_Lighting.get(POINT_LIGHT_POSITION).get<picojson::array>();
		pLightNode->AddProperty(POINT_LIGHT_POSITION, Property<glm::vec3>::create(POINT_LIGHT_POSITION, GLMVec3(o_vec3_list), GL_FLOAT_VEC3));
	}

	//<set light Specular
	if (snull != o_Lighting.get(POINT_LIGHT_SPECULAR))
	{
		picojson::array o_vec4_list = o_Lighting.get(POINT_LIGHT_SPECULAR).get<picojson::array>();
		pLightNode->AddProperty(POINT_LIGHT_SPECULAR, Property<glm::vec4>::create(POINT_LIGHT_SPECULAR, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Diffuse
	if (snull != o_Lighting.get(POINT_LIGHT_DIFFUSE))
	{
		picojson::array o_vec4_list = o_Lighting.get(POINT_LIGHT_DIFFUSE).get<picojson::array>();
		pLightNode->AddProperty(POINT_LIGHT_DIFFUSE, Property<glm::vec4>::create(POINT_LIGHT_DIFFUSE, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Ambient
	if (snull != o_Lighting.get(POINT_LIGHT_AMBIENT))
	{
		picojson::array o_vec4_list = o_Lighting.get(POINT_LIGHT_AMBIENT).get<picojson::array>();
		pLightNode->AddProperty(POINT_LIGHT_AMBIENT, Property<glm::vec4>::create(POINT_LIGHT_AMBIENT, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Color
	if (snull != o_Lighting.get(POINT_LIGHT_COLOR))
	{
		picojson::array o_vec4_list = o_Lighting.get(POINT_LIGHT_COLOR).get<picojson::array>();
		pLightNode->AddProperty(POINT_LIGHT_COLOR, Property<glm::vec4>::create(POINT_LIGHT_COLOR, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light constant, liner, quadratic
	if (snull != o_Lighting.get(POINT_LIGHT_CONSTANT))
	{
		auto value = DOUBLE2FLOAT(o_Lighting.get(POINT_LIGHT_CONSTANT).get<double>());
		auto property = Property<float>::create(POINT_LIGHT_CONSTANT, std::move(value), GL_FLOAT);
		property->setUpper(1.0f);
		property->setLower(0.0f);
		pLightNode->AddProperty(POINT_LIGHT_CONSTANT, property);
	}

	if (snull != o_Lighting.get(POINT_LIGHT_LINEAR))
	{
		auto value = DOUBLE2FLOAT(o_Lighting.get(POINT_LIGHT_LINEAR).get<double>());
		auto property = Property<float>::create(POINT_LIGHT_LINEAR, std::move(value), GL_FLOAT);
		property->setUpper(1.0f);
		property->setLower(0.0f);
		pLightNode->AddProperty(POINT_LIGHT_LINEAR, property);
	}

	if (snull != o_Lighting.get(POINT_LIGHT_QUADRATIC))
	{
		auto value = DOUBLE2FLOAT(o_Lighting.get(POINT_LIGHT_QUADRATIC).get<double>());
		auto property = Property<float>::create(POINT_LIGHT_QUADRATIC, std::move(value), GL_FLOAT);
		property->setUpper(1.0f);
		property->setLower(0.0f);
		pLightNode->AddProperty(POINT_LIGHT_QUADRATIC, property);
	}
}

void LoadDirectionalLight(picojson::value& o_Lighting, UIObjectPtr pLightNode)
{
	//<set light Color
	if (snull != o_Lighting.get(DIRECTIONAL_LIGHT_COLOR))
	{
		picojson::array o_vec4_list = o_Lighting.get(DIRECTIONAL_LIGHT_COLOR).get<picojson::array>();
		pLightNode->AddProperty(DIRECTIONAL_LIGHT_COLOR, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_COLOR, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Specular
	if (snull != o_Lighting.get(DIRECTIONAL_LIGHT_SPECULAR))
	{
		picojson::array o_vec4_list = o_Lighting.get(DIRECTIONAL_LIGHT_SPECULAR).get<picojson::array>();
		pLightNode->AddProperty(DIRECTIONAL_LIGHT_SPECULAR, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_SPECULAR, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Diffuse
	if (snull != o_Lighting.get(DIRECTIONAL_LIGHT_DIFFUSE))
	{
		picojson::array o_vec4_list = o_Lighting.get(DIRECTIONAL_LIGHT_DIFFUSE).get<picojson::array>();
		pLightNode->AddProperty(DIRECTIONAL_LIGHT_DIFFUSE, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_DIFFUSE, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Ambient
	if (snull != o_Lighting.get(DIRECTIONAL_LIGHT_AMBIENT))
	{
		picojson::array o_vec4_list = o_Lighting.get(DIRECTIONAL_LIGHT_AMBIENT).get<picojson::array>();
		pLightNode->AddProperty(DIRECTIONAL_LIGHT_AMBIENT, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_AMBIENT, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Direction
	if (snull != o_Lighting.get(DIRECTIONAL_LIGHT_DIRECTION))
	{
		picojson::array o_vec3_list = o_Lighting.get(DIRECTIONAL_LIGHT_DIRECTION).get<picojson::array>();
		pLightNode->AddProperty(DIRECTIONAL_LIGHT_DIRECTION, Property<glm::vec3>::create(DIRECTIONAL_LIGHT_DIRECTION, GLMVec3(o_vec3_list), GL_FLOAT_VEC3));
	}
}

void LoadSpotLight(picojson::value& o_Lighting, UIObjectPtr pLightNode)
{
	//<set light Pos
	if (snull != o_Lighting.get(SPOT_LIGHT_POSITION))
	{
		picojson::array o_vec3_list = o_Lighting.get(SPOT_LIGHT_POSITION).get<picojson::array>();
		pLightNode->AddProperty(SPOT_LIGHT_POSITION, Property<glm::vec3>::create(SPOT_LIGHT_POSITION, GLMVec3(o_vec3_list), GL_FLOAT_VEC3));
	}

	//<set light Direction
	if (snull != o_Lighting.get(SPOT_LIGHT_DIRECTION))
	{
		picojson::array o_vec3_list = o_Lighting.get(SPOT_LIGHT_DIRECTION).get<picojson::array>();
		pLightNode->AddProperty(SPOT_LIGHT_DIRECTION, Property<glm::vec3>::create(SPOT_LIGHT_DIRECTION, GLMVec3(o_vec3_list), GL_FLOAT_VEC3));
	}

	//<set light Specular
	if (snull != o_Lighting.get(SPOT_LIGHT_SPECULAR))
	{
		picojson::array o_vec4_list = o_Lighting.get(SPOT_LIGHT_SPECULAR).get<picojson::array>();
		pLightNode->AddProperty(SPOT_LIGHT_SPECULAR, Property<glm::vec4>::create(SPOT_LIGHT_SPECULAR, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Diffuse
	if (snull != o_Lighting.get(SPOT_LIGHT_DIFFUSE))
	{
		picojson::array o_vec4_list = o_Lighting.get(SPOT_LIGHT_DIFFUSE).get<picojson::array>();
		pLightNode->AddProperty(SPOT_LIGHT_DIFFUSE, Property<glm::vec4>::create(SPOT_LIGHT_DIFFUSE, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light Ambient
	if (snull != o_Lighting.get(SPOT_LIGHT_AMBIENT))
	{
		picojson::array o_vec4_list = o_Lighting.get(SPOT_LIGHT_AMBIENT).get<picojson::array>();
		pLightNode->AddProperty(SPOT_LIGHT_AMBIENT, Property<glm::vec4>::create(SPOT_LIGHT_AMBIENT, GLMVec4(o_vec4_list), GL_FLOAT_VEC4));
	}

	//<set light cut off
	if (snull != o_Lighting.get(SPOT_LIGHT_CUTOFF))
	{
		auto value = DOUBLE2FLOAT(o_Lighting.get(SPOT_LIGHT_CUTOFF).get<double>());
		auto property = Property<float>::create(SPOT_LIGHT_CUTOFF, glm::cos(glm::radians(value)), GL_FLOAT);
		property->setUpper(1.0f);
		property->setLower(0.0f);
		pLightNode->AddProperty(SPOT_LIGHT_CUTOFF, property);
	}

	if (snull != o_Lighting.get(SPOT_LIGHT_OUTER_CUTOFF))
	{
		auto value = DOUBLE2FLOAT(o_Lighting.get(SPOT_LIGHT_OUTER_CUTOFF).get<double>());
		auto property = Property<float>::create(SPOT_LIGHT_OUTER_CUTOFF, glm::cos(glm::radians(value)), GL_FLOAT);
		property->setUpper(1.0f);
		property->setLower(0.0f);
		pLightNode->AddProperty(SPOT_LIGHT_OUTER_CUTOFF, property);
	}

	if (snull != o_Lighting.get(SPOT_LIGHT_INNER_CUTOFF))
	{
		auto value = DOUBLE2FLOAT(o_Lighting.get(SPOT_LIGHT_INNER_CUTOFF).get<double>());
		auto property = Property<float>::create(SPOT_LIGHT_INNER_CUTOFF, glm::cos(glm::radians(value)), GL_FLOAT);
		property->setUpper(1.0f);
		property->setLower(0.0f);
		pLightNode->AddProperty(SPOT_LIGHT_INNER_CUTOFF, property);
	}
}

void LoadLightingProperty(picojson::array& o_lighting_list, Node3DPtr pObject)
{
	std::vector<UIObjectPtr> pointLights;
	std::vector<UIObjectPtr> spotLights;
	std::vector<UIObjectPtr> directionalLights;
	for (auto& o_Lighting : o_lighting_list)
	{
		//<set light type
		if (snull != o_Lighting.get(TYPE))
		{
			auto type = gLightMap.at(o_Lighting.get(TYPE).get<std::string>());
			UIObjectPtr pLightNode = nullptr;			
			switch (type)
			{
			case POINT_LIGHT:
				pLightNode = NodePointLight::create(o_Lighting.get(NAME).get<std::string>());
				LoadPointLight(o_Lighting, pLightNode);
				pointLights.push_back(pLightNode);
				break;
			case DIRECTIONAL_LIGHT:
				pLightNode = NodeDirectionalLight::create(o_Lighting.get(NAME).get<std::string>());
				LoadDirectionalLight(o_Lighting, pLightNode);
				directionalLights.push_back(pLightNode);
				break;
			case SPOT_LIGHT:
				pLightNode = NodeSpotLight::create(o_Lighting.get(NAME).get<std::string>());
				LoadSpotLight(o_Lighting, pLightNode);
				spotLights.push_back(pLightNode);
				break;
			default:
				continue;
				break;
			}
			pLightNode->AddProperty(LIGHT_TYPE, Property<glm::i32>::create(LIGHT_TYPE, std::move(type), GL_INT));

			//<set light Layout
			if (snull != o_Lighting.get(LAYOUT))
			{
				auto o_layout = o_Lighting.get(LAYOUT).get<picojson::object>();
				LoadLayoutProperty(o_layout, pLightNode);
			}
			else {}
		}
	}

	for (auto& node : pointLights)
	{
		pObject->addChild(node);
	}

	for (auto& node : spotLights)
	{
		pObject->addChild(node);
	}

	for (auto& node : directionalLights)
	{
		pObject->addChild(node);
	}

	//pObject->SetNodeLight<NodePointLight>(pointLights);
	//pObject->SetNodeLight<NodeSpotLight>(spotLights);
	//pObject->SetNodeLight<NodeDirectionalLight>(directionalLights);
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
			GLenum param = GL_TEXTURE_PVALUE_MAP.at(iter->get(WRAP).get<std::string>());
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_S, param);
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_T, param);
		}
		else
		{
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_S, "GL_CLAMP_TO_EDGE");
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_T, "GL_CLAMP_TO_EDGE");
		}

		if (snull != iter->get(FILTER))
		{
			GLenum param = GL_TEXTURE_PVALUE_MAP.at(iter->get(FILTER).get<std::string>());
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MIN_FILTER, param);
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MAG_FILTER, param);
		}
		else
		{
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MIN_FILTER, "GL_LINEAR");
			pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MAG_FILTER, "GL_LINEAR");
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

void LoadLayoutProperty(picojson::object& o_layout, UIObjectPtr pObject)
{
	auto layoutMethod = pObject->GetPropertyMethodObj<LayoutProperty>();

	//<Set layout width and height
#ifdef OPENGL_RENDERING
	glm::vec2 size(o_layout[LAYOUT_WIDTH].get<double>(), o_layout[LAYOUT_HEIGHT].get<double>());
	layoutMethod->SetLayoutSize(size);
#else
	layoutMethod->SetLayoutWidth(DOUBLE2INT(o_layout[LAYOUT_WIDTH].get<double>()));
	layoutMethod->SetLayoutHeight(DOUBLE2INT(o_layout[LAYOUT_HEIGHT].get<double>()));
#endif

	//<Set layout coordinator (X,Y)
#ifndef OPENGL_RENDERING
	layoutMethod->SetLayoutPosition(
		DOUBLE2INT(o_layout[X_COORDINATOR].get<double>()),
		DOUBLE2INT(o_layout[Y_COORDINATOR].get<double>()));
#endif

#ifdef OPENGL_RENDERING
	//<Set layout transform
	if (o_layout.end() != o_layout.find(TRANSFORM))
	{
		picojson::array o_vec3_list = o_layout[TRANSFORM].get<picojson::array>();
		layoutMethod->SetLayoutTransform(GLMVec3(o_vec3_list));
	}

	//<Set layout rotation
	if (o_layout.end() != o_layout.find(ROTATE))
	{
		picojson::array o_vec3_list = o_layout[ROTATE].get<picojson::array>();
		layoutMethod->SetRotation(GLMVec3(o_vec3_list));
	}

	//<Set layout scale
	if (o_layout.end() != o_layout.find(SCALE))
	{
		picojson::array o_vec3_list = o_layout[SCALE].get<picojson::array>();
		layoutMethod->SetLayoutScale(GLMVec3(o_vec3_list));
	}
#endif
}

void LoadOriginProperty(picojson::object& o_origin_property, UIObjectPtr pObject)
{
	auto originMethod = pObject->GetPropertyMethodObj<OriginProperty>();

	//Visible
	if (snull != o_origin_property[VISIBLE])
	{
		originMethod->SetVisible(o_origin_property[VISIBLE].get<bool>());
	}

	//Center point
	if (snull != o_origin_property[CENTER_POINT])
	{
		picojson::array o_vec2_list = o_origin_property[CENTER_POINT].get<picojson::array>();
#ifdef OPENGL_RENDERING
		originMethod->SetCenterPoint(GLMVec2(o_vec2_list));
#else
		auto center_point = GLMVec2(o_vec2_list);
		originMethod->SetCenterPoint(center_point.x, center_point.y);
#endif
	}
	//Pivot point
	if (snull != o_origin_property[PIVOT_POINT])
	{
		picojson::array o_vec3_list = o_origin_property[PIVOT_POINT].get<picojson::array>();
		originMethod->SetPivotPoint(GLMVec3(o_vec3_list));
	}

	//Flip, Opacity, Angle, Clip, Aligment
	originMethod->SetFlip(SDL_RendererFlip(DOUBLE2INT(o_origin_property[FLIP].get<double>())));
#ifdef OPENGL_RENDERING
	originMethod->SetOpacity(o_origin_property[OPACITY].get<double>());
#else
	originMethod->SetOpacity(DOUBLE2INT(o_origin_property[OPACITY].get<double>()));
#endif
	if (snull != o_origin_property[ANGLE])
	{
		originMethod->SetAngle(o_origin_property[ANGLE].get<double>());
	}

	if (snull != o_origin_property[ISCLIP])
	{
		originMethod->SetClip(o_origin_property[ISCLIP].get<bool>());
	}

	if (snull != o_origin_property[ALIGNMENT_HORIZONTAL])
	{
		auto aligment = gMapAligment.at(o_origin_property[ALIGNMENT_HORIZONTAL].get<std::string>());
		originMethod->SetAlignHorizontal(aligment);
	}

	if (snull != o_origin_property[ALIGNMENT_VERIZONTAL])
	{
		auto aligment = gMapAligment.at(o_origin_property[ALIGNMENT_VERIZONTAL].get<std::string>());
		originMethod->SetAlignVerizontal(aligment);
	}
	//Foreground color
	if (o_origin_property.end() != o_origin_property.find(FORE_GROUND_COLOR))
	{
		picojson::array o_vec4_list = o_origin_property[FORE_GROUND_COLOR].get<picojson::array>();
#ifdef OPENGL_RENDERING	
		originMethod->SetForeGroundColor(GLMVec4(o_vec4_list));
#else
		SDL_Color color = {
			DOUBLE2UINT8(o_vec4_list[0]),
			DOUBLE2UINT8(o_vec4_list[1]),
			DOUBLE2UINT8(o_vec4_list[2]),
			DOUBLE2UINT8(o_vec4_list[3])
		};
		originMethod->SetForeGroundColor(color);
#endif
	}

	//Background color
	if (o_origin_property.end() != o_origin_property.find(BACK_GROUND_COLOR))
	{
		picojson::array o_vec4_list = o_origin_property[BACK_GROUND_COLOR].get<picojson::array>();
#ifdef OPENGL_RENDERING
		originMethod->SetBackGroundColor(GLMVec4(o_vec4_list));
#else
		SDL_Color color = {
			DOUBLE2UINT8(o_vec4_list[0]),
			DOUBLE2UINT8(o_vec4_list[1]),
			DOUBLE2UINT8(o_vec4_list[2]),
			DOUBLE2UINT8(o_vec4_list[3])
		};
		originMethod->SetBackGroundColor(color);
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
			LoadLayoutProperty(o_layout, pObject);
		}
		else {}


		//<set origin property
		if (snull != iter->get(ORIGINAL_PROPERTY))
		{
			picojson::object o_origin = iter->get(ORIGINAL_PROPERTY).get<picojson::object>();
			LoadOriginProperty(o_origin, pObject);
		}
		else {}

		//<Set text property
		if (NODE_2D_TEXT_TYPE == type && snull != iter->get(TEXT_PROPERTY))
		{
			NodeText2DPtr pNodeText2D = std::dynamic_pointer_cast<NodeText2D>(pObject);
			TextPropertyPtr textMethod = pObject->GetPropertyMethodObj<TextProperty>();

			picojson::object o_text_property = iter->get(TEXT_PROPERTY).get<picojson::object>();
			textMethod->SetText(o_text_property[TEXT].get<std::string>().c_str());
			textMethod->SetFontName(o_text_property[FONT_NAME].get<std::string>().c_str());
			textMethod->SetFontSize(DOUBLE2INT(o_text_property[FONT_SIZE].get<double>()));

			picojson::array o_vec3_list = o_text_property[FONT_COLOR].get<picojson::array>();
#ifdef OPENGL_RENDERING
			textMethod->SetColor(GLMVec3(o_vec3_list));
#else
			textMethod->SetColor(
				DOUBLE2INT(o_vec3_list[0]),
				DOUBLE2INT(o_vec3_list[1]),
				DOUBLE2INT(o_vec3_list[2]),
				DOUBLE2INT(255));
#endif
		}

		if (NODE_3D == type)
		{

			//<set model
			if (snull != iter->get(MODEL))
			{
				picojson::array o_model_list = iter->get(MODEL).get<picojson::array>();
				for (auto& value : o_model_list)
				{
					std::dynamic_pointer_cast<Node3D>(pObject)->SetModel(value.get<std::string>());
				}
			}

			//<set mesh material
			if (snull != iter->get(MESH))
			{
				picojson::array o_mesh_list = iter->get(MESH).get<picojson::array>();
				for (auto& o_mesh : o_mesh_list)
				{
					auto name = o_mesh.get(NAME).get<std::string>();
					auto mesh = Library::GetInstance()->get<Mesh>(o_mesh.get(MESH).get<std::string>());
					auto node = pObject->getChild<NodeMesh>(name);
					if (!node)
					{
						node = NodeMesh::create(name);
						node->SetMesh(mesh);
						pObject->addChild(node);
					}

					//set layout property
					if (snull != o_mesh.get(LAYOUT))
					{
						picojson::object o_layout = o_mesh.get(LAYOUT).get<picojson::object>();
						LoadLayoutProperty(o_layout, node);
					}
					else {}

					//set Material property
					if (snull != o_mesh.get(MATERIAL_PROPERTY_LIST))
					{
						picojson::object o_material = o_mesh.get(MATERIAL_PROPERTY_LIST).get<picojson::object>();
						LoadMaterialProperty(o_material, node);
					}

				}
			}

			//set gl property
			if (snull != iter->get(GLPROPERTY))
			{
				picojson::object o_glProperty = iter->get(GLPROPERTY).get<picojson::object>();
				LoadGLProperty(o_glProperty, std::dynamic_pointer_cast<Node3D>(pObject));
			}
			else {}

			//set camera property
			if (snull != iter->get(CAMERA))
			{
				picojson::array o_camera_list = iter->get(CAMERA).get<picojson::array>();
				LoadCameraProperty(o_camera_list, std::dynamic_pointer_cast<Node3D>(pObject));
			}
			else {}

			//<Set lighting
			if (snull != iter->get(LIGHTING_LIST))
			{
				picojson::array o_lighting_list = iter->get(LIGHTING_LIST).get<picojson::array>();
				LoadLightingProperty(o_lighting_list, std::dynamic_pointer_cast<Node3D>(pObject));
			}
		}
#ifdef OPENGL_RENDERING
		//set render pass
		if (snull != iter->get(RENDER_PASS))
		{
			picojson::array o_render_pass = iter->get(RENDER_PASS).get<picojson::array>();
			GLint render_pass = DEFAULT_RENDER_PASS;
			for (auto& o_value : o_render_pass)
			{
				render_pass |= gRenderPassMap.at(o_value.get<std::string>());

			}
			pObject->SetPropertyValue<GLint>(RENDER_PASS, render_pass);
		}
		else
		{
			pObject->SetPropertyValue<GLint>(RENDER_PASS, DEFAULT_RENDER_PASS | MULTISAMPLE_RENDER_PASS);
		}

#endif
		//<add to ui object table
		UIObjectTable[resource_name] = pObject;
		LOG_DEBUG("Resource added[%s]", resource_name.c_str());
	}
}

void LoadShader(std::string shader_name, GLenum type)
{
	std::string shader_path = Configuration::GetInstance()->shader_path + shader_name;

	auto pShader = Shader::create(shader_name, type);

	//<load and attach source
	pShader->load(shader_path);

	pShader->setSource();

	//<Compile shader
	pShader->compile();

	//<Add to library
	Library::GetInstance()->add<Shader>(shader_name, pShader);
}

void LoadShaderProgramList(picojson::value& json_value)
{
	//<get shader program list
	picojson::array o_shaderProgram_list = json_value.get(SHADER_PROGRAM_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_shaderProgram_list.begin(); iter != o_shaderProgram_list.end(); ++iter)
	{
		std::string program_name = iter->get<std::string>();
		auto pShaderProgram = ShaderProgram::create(program_name);

		//<load vertex + fragment shader
		std::string vertex_shader_name = program_name + "Vex.glsl";
		std::string fragment_shader_name = program_name + "Frag.glsl";
		LoadShader(vertex_shader_name, GL_VERTEX_SHADER);
		LoadShader(fragment_shader_name, GL_FRAGMENT_SHADER);

		//<Attach shader to program
		pShaderProgram->attachShader(Library::GetInstance()->get<Shader>(vertex_shader_name));
		pShaderProgram->attachShader(Library::GetInstance()->get<Shader>(fragment_shader_name));

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

void LoadModel(UIObjectTableType& UIObjectTable, picojson::value& json_value)
{
	std::string obj_file;
	picojson::array o_model_list = json_value.get(THREE_D_ASSET_LIST).get<picojson::array>();
	for (picojson::value item : o_model_list)
	{
		obj_file = Configuration::GetInstance()->asset_path + item.get<std::string>();
		ModelLoader loader(obj_file.c_str());
		if (loader.loadModel())
		{
			auto pNode3D = loader.getNode();
			UIObjectTable[pNode3D->getName()] = pNode3D;
		}
	}
}

void LoadMeshList(picojson::value& json_value)
{
	//<get model data
	picojson::array o_model_data_list = json_value.get(MODEL_LIST).get<picojson::array>();
	for (picojson::array::iterator iter = o_model_data_list.begin(); iter != o_model_data_list.end(); ++iter)
	{
		std::string name = iter->get(NAME).get<std::string>();
		//<get mesh data
		std::vector<MeshPtr> meshes;
		picojson::array o_mesh_data_list = iter->get(MESH).get<picojson::array>();
		for (picojson::array::iterator iter = o_mesh_data_list.begin(); iter != o_mesh_data_list.end(); ++iter)
		{
			std::string name = iter->get(NAME).get<std::string>();
			std::string material = iter->get(MATERIAL).get<std::string>();

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
			auto pMesh = Mesh::create(name, vertices, indices);
			pMesh->setMaterial(material);
			meshes.push_back(pMesh);

			//<Add to library
			Library::GetInstance()->add<Mesh>(name, pMesh);
		}
		auto pModel = Model::create(name, meshes);
		pModel->debug();

		//<Add to library
		Library::GetInstance()->add<Model>(name, pModel);
	}

}