#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "GLInfoLog.h"
#include "log.h"
#include <fstream>
#include <sstream>
#include <iostream>

Mesh::Mesh(std::string name, std::vector<VertexRecord> vertices, std::vector<unsigned int> indices): m_name(name)
{
    this->vertices = vertices;
    this->indices = indices;

    setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    if (!indices.empty())
    {
        glGenBuffers(1, &EBO);
    }
    
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexRecord), &vertices[0], GL_STATIC_DRAW);  

    if (!indices.empty())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexRecord), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexRecord), (void*)(sizeof(VertexRecord::Position)));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexRecord), (void*)(sizeof(VertexRecord::Position) + sizeof(VertexRecord::Normal)));

    glBindVertexArray(0);
}

void Mesh::debug()
{
	LOG_DEBUG("Mesh[%s]:VAO[%u] VBO[%u] EBO[%u], vertex count[%u] index count[%u] material[%s]", m_name.c_str(), VAO, VBO, EBO, vertices.size(), indices.size(), m_material.c_str());
}

uint32_t Mesh::vao() const
{
    return VAO;
}

uint32_t Mesh::vbo() const
{
    return VBO;
}
uint32_t Mesh::ebo() const
{
    return EBO;
}

size_t Mesh::vertexCount() const
{
    return vertices.size();
}

size_t Mesh::indexCount() const
{
    return indices.size();
}

std::string Mesh::getName() const
{
    return m_name;
}

std::string Mesh::getMaterial()
{
    return m_material;
}

std::vector<glm::vec3> Mesh::getVertex()
{
    std::vector<glm::vec3> list;
    for (auto& record : vertices)
    {
        list.push_back(record.Position);
    }
    return list;
}

void Mesh::setMaterial(std::string pMaterial)
{
    m_material = pMaterial;
}