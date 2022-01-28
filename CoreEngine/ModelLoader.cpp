#include "stdafx.h"
#include "ModelLoader.h"
#include "Library.h"
#include "Material.h"
#include "log.h"

#include "PropertyDefine.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp >

static const std::map<aiTextureType, std::string> sAiTExtureTypeMap =
{
	{ aiTextureType_DIFFUSE, "material.diffuseMap"},
	{ aiTextureType_SPECULAR , "material.specularMap"},
	{ aiTextureType_HEIGHT , "material.heightMap"},
	{ aiTextureType_NORMALS , "material.normalMap" },
	{ aiTextureType_SHININESS , "material.shininessMap" },
	{ aiTextureType_OPACITY , "material.opacityMap" },
	{ aiTextureType_DISPLACEMENT , "material.displacementMap" },
	{ aiTextureType_LIGHTMAP , "material.lightMap" },
	{ aiTextureType_REFLECTION , "material.reflectMap" },
	{ aiTextureType_NORMAL_CAMERA , "material.normalCamMap" },
	{ aiTextureType_METALNESS ,  "material.metalnessMap" },
	{ aiTextureType_DIFFUSE_ROUGHNESS , "material.rounghnessMap" },
	{ aiTextureType_AMBIENT_OCCLUSION , "material.ambientOcclusionMap"}
};

static void decompose(
						glm::mat4 &transformation, // your transformation matrix.
						glm::vec3 &scale,
						glm::vec3& translation,
						glm::vec3& euler)
{
	scale = glm::vec3(
		glm::length(glm::vec3(transformation[0][0], transformation[1][0], transformation[2][0])),
		glm::length(glm::vec3(transformation[0][1], transformation[1][1], transformation[2][1])),
		glm::length(glm::vec3(transformation[0][2], transformation[1][2], transformation[2][2]))
	);

	translation = glm::vec3(transformation[0][3], transformation[1][3], transformation[2][3]);

	euler.y = asin(-transformation[0][2]);
	if (cos(euler.y) != 0) {
		euler.x = atan2(transformation[1][2], transformation[2][2]);
		euler.z = atan2(transformation[0][1], transformation[0][0]);
	}
	else
	{
		euler.x = atan2(-transformation[2][0], transformation[1][1]);
		euler.z = 0;
	}
	euler = euler * (180.f / glm::pi<float>());
}

static glm::vec3 GLMVec3(aiVector3D& in)
{
	glm::vec3 v3;
	std::memcpy(&v3, &in, sizeof(aiVector3D));
	return v3;
}

static glm::vec3 GLMVec3(aiColor3D& in)
{
	glm::vec3 v3;
	std::memcpy(&v3, &in, sizeof(aiColor3D));
	return v3;
}

static glm::vec4 GLMVec4(aiColor4D& in)
{
	glm::vec4 v4;
	std::memcpy(&v4, &in, sizeof(aiColor4D));
	return v4;
}

static glm::vec4 GLMVec4(aiColor3D& in)
{
	glm::vec4 v4(1.0f);
	std::memcpy(&v4, &in, sizeof(aiColor3D));
	return v4;
}

static glm::mat4 GLMMat4(aiMatrix4x4& in)
{
	glm::mat4 m4(1.0f);
	std::memcpy(&m4, &in, sizeof(aiMatrix4x4));
	return m4;
}

static bool ASSIMP_CHECK(int result, std::string comment)
{
	bool bResult = true;
	if (AI_SUCCESS != result)
	{
		bResult = false;
		LOG_DEBUG("ASSIMP error :%s", comment.c_str());
	}
	return bResult;
}

GLTexturePtr ModelLoader::TextureFromFile(const char* path, std::string directory)
{
	std::string texture_name = std::string(path);
	texture_name = texture_name.substr(texture_name.find_last_of("/\\") + 1);

	std::string texture_path = directory + '/' + path;

	auto pGLTexture = GLTexture::create(texture_name, texture_path, GL_TEXTURE_2D);

	//<add texture param
	pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_S, "GL_REPEAT");
	pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_WRAP_T, "GL_REPEAT");

	pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MIN_FILTER, "GL_LINEAR_MIPMAP_LINEAR");
	pGLTexture->addTextureParam(TEXT_PARAM_I, GL_TEXTURE_MAG_FILTER, "GL_LINEAR");

	pGLTexture->loadToGPU(true);
	pGLTexture->init();

	return pGLTexture;
}

ModelLoader::ModelLoader(const char* _path) :path(_path)
{
	path = _path;
	directory = path.substr(0, path.find_last_of('/'));
	name = path.substr(path.find_last_of('/') + 1, path.find_last_of('.') - path.find_last_of('/') - 1);
}

ModelLoader::~ModelLoader() {}

ModelPtr ModelLoader::loadModel()
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_DEBUG("ERROR::ASSIMP::%s", import.GetErrorString());
		_ASSERT(false);
	}

	processNode(scene->mRootNode, scene);
	pModel = Model::create(name, meshes);
	if (pModel)
	{
		pModel->debug();
		Library::GetInstance()->add<Model>(pModel->getName(), pModel);

		pObject = Node3D::create(name);
		pObject->SetModel(name);

		processLight(scene);
	}

	for (auto& material : materials)
	{
		Library::GetInstance()->add<Material>(material->getName(), material);
	}
	return pModel;
}

ModelPtr ModelLoader::getModel()
{
	return pModel;
}

Node3DPtr ModelLoader::getNode()
{
	return pObject;
}

void ModelLoader::processLight(const aiScene* scene)
{
	std::vector<UIObjectPtr> pointLights;
	std::vector<UIObjectPtr> spotLights;
	std::vector<UIObjectPtr> directionalLights;

	for (unsigned int i = 0; i < scene->mNumLights; i++)
	{
		aiLight* light = scene->mLights[i];
		UIObjectPtr pLight = nullptr;
		
		switch (light->mType)
		{
		case  aiLightSource_POINT:
			pLight = NodePointLight::create(light->mName.C_Str());
			LoadPointLight(light, pLight);
			pointLights.push_back(pLight);
			break;
		case  aiLightSource_SPOT:
			pLight = NodeSpotLight::create(light->mName.C_Str());
			LoadSpotLight(light, pLight);
			spotLights.push_back(pLight);
			break;
		case  aiLightSource_DIRECTIONAL:
			pLight = NodeDirectionalLight::create(light->mName.C_Str());
			LoadDirectionalLight(light, pLight);
			directionalLights.push_back(pLight);
			break;
		default:
			continue;
			break;
		}

		auto node = scene->mRootNode->FindNode(pLight->getName().c_str());
		if (node)
		{
			auto world_matrix = GLMMat4(node->mTransformation);
			//<do something with node->mTransformation;
			glm::vec3 scale;
			glm::vec3 translation;
			glm::vec3 rotation;
			decompose(world_matrix, scale, translation, rotation);

			pLight->SetPropertyValue<glm::vec3>(TRANS_VECTOR, translation);
			pLight->SetPropertyValue<glm::vec3>(SCALE_VECTOR, scale);
			pLight->SetPropertyValue<glm::vec3>(ROTATE_VECTOR, rotation);
		}
	}
	pObject->SetNodeLight<NodePointLight>(pointLights);
	pObject->SetNodeLight<NodeSpotLight>(spotLights);
	pObject->SetNodeLight<NodeDirectionalLight>(directionalLights);
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		auto pMesh = processMesh(mesh, scene);
		meshes.push_back(pMesh);

		// process material
		auto pMaterial = processMaterial(mesh, scene);
		materials.push_back(pMaterial);


		// link mesh with material
		pMesh->setMaterial(pMaterial->getName());


		//<Add to library
		Library::GetInstance()->add<Mesh>(pMesh->getName(), pMesh);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

MeshPtr ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexRecord> vertices;
	std::vector<unsigned int> indices;
	std::vector<GLTexturePtr> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexRecord vertex;
		glm::vec3 vector;
		// process vertex positions, normals and texture coordinates 
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh::create(mesh->mName.C_Str(), vertices, indices);
}

MaterialPtr ModelLoader::processMaterial(aiMesh* mesh, const aiScene* scene)
{
	MaterialPtr pMaterial = nullptr;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		pMaterial = Material::create(material->GetName().C_Str());

		for (auto& aiTextureMapType : sAiTExtureTypeMap)
		{
			std::vector<GLTexturePtr> texturelist = loadMaterialTextures(material, aiTextureMapType.first);
			for (auto& pTexture : texturelist)
			{
				Library::GetInstance()->add<GLTexture>(pTexture->getName(), pTexture);
				auto property = Property<glm::u32>::create(aiTextureMapType.second, pTexture->getID(), GL_SAMPLER_2D);
				pMaterial->AddProperty(aiTextureMapType.second, property);
				pMaterial->AddTexture(aiTextureMapType.second, pTexture);
			}
		}

		aiShadingMode shading;
		if (ASSIMP_CHECK(material->Get(AI_MATKEY_SHADING_MODEL, shading), "no shading name"))
		{
			pMaterial->AddProperty(MATERIAL_MODEL, Property<glm::i32>::create(MATERIAL_MODEL, std::move(shading), GL_INT));
		}

		float value;
		if (ASSIMP_CHECK(material->Get(AI_MATKEY_SHININESS_STRENGTH, value), "no shininess strength"))
		{
			pMaterial->AddProperty(MATERIAL_SHININESS_STRENGTH, Property<float>::create(MATERIAL_SHININESS_STRENGTH, std::move(value), GL_FLOAT));
		}

		if (ASSIMP_CHECK(material->Get(AI_MATKEY_BUMPSCALING, value), "no bump scale"))
		{
			pMaterial->AddProperty(MATERIAL_BUMPSCALING, Property<float>::create(MATERIAL_BUMPSCALING, std::move(value), GL_FLOAT));
		}

		if (ASSIMP_CHECK(material->Get(AI_MATKEY_REFLECTIVITY, value), "no reflectivity"))
		{
			pMaterial->AddProperty(MATERIAL_REFECTIVITY, Property<float>::create(MATERIAL_REFECTIVITY, std::move(value), GL_FLOAT));
		}

		if (ASSIMP_CHECK(material->Get(AI_MATKEY_OPACITY, value), "no opacity"))
		{
			pMaterial->AddProperty(MATERIAL_OPACITY, Property<float>::create(MATERIAL_OPACITY, std::move(value), GL_FLOAT));
		}

		aiColor3D color(1.0f, 1.0f, 1.0f);
		if (ASSIMP_CHECK(material->Get(AI_MATKEY_COLOR_DIFFUSE, color), "no diffuse color"))
		{
			auto property = Property<glm::vec4>::create(MATERIAL_DIFFUSE, GLMVec4(color), GL_FLOAT_VEC4);
			pMaterial->AddProperty(MATERIAL_SPECULAR, property);
		}

		if (ASSIMP_CHECK(material->Get(AI_MATKEY_COLOR_AMBIENT, color), "no ambient color"))
		{
			auto property = Property<glm::vec4>::create(MATERIAL_AMBIENT, GLMVec4(color), GL_FLOAT_VEC4);
			pMaterial->AddProperty(MATERIAL_SPECULAR, property);
		}

		if (ASSIMP_CHECK(material->Get(AI_MATKEY_COLOR_SPECULAR, color), "no specular color"))
		{
			auto property = Property<glm::vec4>::create(MATERIAL_SPECULAR, GLMVec4(color), GL_FLOAT_VEC4);
			pMaterial->AddProperty(MATERIAL_SPECULAR, property);
		}

		if (ASSIMP_CHECK(material->Get(AI_MATKEY_COLOR_EMISSIVE, color), "no emissive color"))
		{
			auto property = Property<glm::vec4>::create(MATERIAL_EMISSIVE, GLMVec4(color), GL_FLOAT_VEC4);
			pMaterial->AddProperty(MATERIAL_EMISSIVE, property);
		}

		if (ASSIMP_CHECK(material->Get(AI_MATKEY_SHININESS, value), "no shininess"))
		{
			auto property = Property<float>::create(MATERIAL_SHININESS, std::move(value), GL_FLOAT);
			pMaterial->AddProperty(MATERIAL_SHININESS, property);
		}
	}
	return pMaterial;
}

std::vector<GLTexturePtr> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<GLTexturePtr> textures_loaded;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j]->getName().data(), str.C_Str()) == 0)
			{
				textures_loaded.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			auto pTexture = TextureFromFile(str.C_Str(), directory);
			textures_loaded.push_back(pTexture); // add to loaded textures
		}
	}
	return textures_loaded;
}


void ModelLoader::LoadPointLight(aiLight* light, UIObjectPtr pLightNode)
{
	pLightNode->SetPropertyValue<glm::i32>(LIGHT_TYPE, POINT_LIGHT);

	//<set light Pos
	pLightNode->AddProperty(name, Property<glm::vec3>::create(name, GLMVec3(light->mPosition), GL_FLOAT_VEC3));

	//<set light Specular
	pLightNode->AddProperty(name, Property<glm::vec4>::create(name, GLMVec4(light->mColorSpecular), GL_FLOAT_VEC4));


	//<set light Diffuse
	pLightNode->AddProperty(name, Property<glm::vec4>::create(name, GLMVec4(light->mColorDiffuse), GL_FLOAT_VEC4));


	//<set light Ambient
	pLightNode->AddProperty(name, Property<glm::vec4>::create(name, GLMVec4(light->mColorDiffuse), GL_FLOAT_VEC4));

	//<set light Color
	pLightNode->AddProperty(name, Property<glm::vec4>::create(name, glm::vec4(1.0f), GL_FLOAT_VEC4));


	//<set light constant, liner, quadratic
	auto value = light->mAttenuationConstant;
	auto property = Property<float>::create(POINT_LIGHT_CONSTANT, std::move(value), GL_FLOAT);
	property->setUpper(1.0f);
	property->setLower(0.0f);
	pLightNode->AddProperty(POINT_LIGHT_CONSTANT, property);


	value = light->mAttenuationLinear;
	property = Property<float>::create(POINT_LIGHT_LINEAR, std::move(value), GL_FLOAT);
	property->setUpper(1.0f);
	property->setLower(0.0f);
	pLightNode->AddProperty(POINT_LIGHT_LINEAR, property);


	value = light->mAttenuationQuadratic;
	property = Property<float>::create(POINT_LIGHT_QUADRATIC, std::move(value), GL_FLOAT);
	property->setUpper(1.0f);
	property->setLower(0.0f);
	pLightNode->AddProperty(POINT_LIGHT_QUADRATIC, property);
}

void ModelLoader::LoadDirectionalLight(aiLight* light, UIObjectPtr pLightNode)
{
	pLightNode->SetPropertyValue<glm::i32>(LIGHT_TYPE, DIRECTIONAL_LIGHT);
	//<set light Color
	pLightNode->AddProperty(DIRECTIONAL_LIGHT_COLOR, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_COLOR, glm::vec4(1.0f), GL_FLOAT_VEC4));

	//<set light Specular
	pLightNode->AddProperty(DIRECTIONAL_LIGHT_SPECULAR, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_SPECULAR, GLMVec4(light->mColorSpecular), GL_FLOAT_VEC4));


	//<set light Diffuse
	pLightNode->AddProperty(DIRECTIONAL_LIGHT_DIFFUSE, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_DIFFUSE, GLMVec4(light->mColorDiffuse), GL_FLOAT_VEC4));

	//<set light Ambient
	pLightNode->AddProperty(DIRECTIONAL_LIGHT_AMBIENT, Property<glm::vec4>::create(DIRECTIONAL_LIGHT_AMBIENT, GLMVec4(light->mColorAmbient), GL_FLOAT_VEC4));


	//<set light Direction
	pLightNode->AddProperty(DIRECTIONAL_LIGHT_DIRECTION, Property<glm::vec3>::create(DIRECTIONAL_LIGHT_DIRECTION, GLMVec3(light->mDirection), GL_FLOAT_VEC3));
}

void ModelLoader::LoadSpotLight(aiLight* light, UIObjectPtr pLightNode)
{
	pLightNode->SetPropertyValue<glm::i32>(LIGHT_TYPE, SPOT_LIGHT);
	//<set light Pos
	pLightNode->AddProperty(SPOT_LIGHT_POSITION, Property<glm::vec3>::create(SPOT_LIGHT_POSITION, GLMVec3(light->mPosition), GL_FLOAT_VEC3));

	//<set light Direction
	pLightNode->AddProperty(SPOT_LIGHT_DIRECTION, Property<glm::vec3>::create(SPOT_LIGHT_DIRECTION, GLMVec3(light->mDirection), GL_FLOAT_VEC3));

	//<set light Specular
	pLightNode->AddProperty(SPOT_LIGHT_SPECULAR, Property<glm::vec4>::create(SPOT_LIGHT_SPECULAR, GLMVec4(light->mColorSpecular), GL_FLOAT_VEC4));


	//<set light Diffuse
	pLightNode->AddProperty(SPOT_LIGHT_DIFFUSE, Property<glm::vec4>::create(SPOT_LIGHT_DIFFUSE, GLMVec4(light->mColorDiffuse), GL_FLOAT_VEC4));


	//<set light Ambient
	pLightNode->AddProperty(SPOT_LIGHT_AMBIENT, Property<glm::vec4>::create(SPOT_LIGHT_AMBIENT, GLMVec4(light->mColorAmbient), GL_FLOAT_VEC4));


	//<set light cut off
	auto value = light->mAngleInnerCone;
	auto property = Property<float>::create(SPOT_LIGHT_CUTOFF, glm::cos(glm::radians(value)), GL_FLOAT);
	property->setUpper(1.0f);
	property->setLower(0.0f);
	pLightNode->AddProperty(SPOT_LIGHT_CUTOFF, property);

	value = light->mAngleOuterCone;
	property = Property<float>::create(SPOT_LIGHT_OUTER_CUTOFF, glm::cos(glm::radians(value)), GL_FLOAT);
	property->setUpper(1.0f);
	property->setLower(0.0f);
	pLightNode->AddProperty(SPOT_LIGHT_OUTER_CUTOFF, property);

	value = light->mAngleInnerCone;
	property = Property<float>::create(SPOT_LIGHT_INNER_CUTOFF, glm::cos(glm::radians(value)), GL_FLOAT);
	property->setUpper(1.0f);
	property->setLower(0.0f);
	pLightNode->AddProperty(SPOT_LIGHT_INNER_CUTOFF, property);
}