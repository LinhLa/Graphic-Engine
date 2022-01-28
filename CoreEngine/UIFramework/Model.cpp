#include "stdafx.h"
#include "Model.h"
#include "stb_image.h"

Model::Model(std::string name, std::vector<MeshPtr> list): m_name(name)
{
	meshes = std::move(list);
}

Model::~Model(){}

std::string Model::getName() const
{
    return m_name;
}

std::vector<MeshPtr> Model::getMeshes()
{
	return meshes;
}

void Model::debug()
{
	LOG_DEBUG(".................................................................");
	LOG_DEBUG("Model[%s]", m_name.c_str());
	for (auto &pMesh : meshes)
	{
		pMesh->debug();
	}
	LOG_DEBUG(".................................................................");
}