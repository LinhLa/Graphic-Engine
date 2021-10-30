#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "GLInfoLog.h"
#include "log.h"

#include <fstream>
#include <sstream>
#include <iostream>

Mesh::Mesh(std::vector<VertexRecord> vertices, std::vector<unsigned int> indices, std::vector<TextureRecord> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

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
	LOG_DEBUG(".................................................................");
	LOG_DEBUG("VAO[%u]", VAO);
	LOG_DEBUG("VBO[%u]", VBO);
	LOG_DEBUG("EBO[%u]", EBO);
    LOG_DEBUG("vertex count[%u]", vertices.size());
    LOG_DEBUG("index count[%u]", indices.size());
	LOG_DEBUG(".................................................................");
}

void Mesh::Draw(ShaderProgramPtr pShader)
{
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//for(unsigned int i = 0; i < textures.size(); i++)
	//{
 //       glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
 //       // retrieve texture number (the N in diffuse_textureN)
 //       std::string number;
 //       std::string name = textures[i].type;
 //       if(name == "texture_diffuse")
 //       	number = std::to_string(diffuseNr++);
 //       else if(name == "texture_specular")
 //       	number = std::to_string(specularNr++);

	//	glUniform1i(glGetUniformLocation(pShader->getID(), ("material." + name + number).c_str()), i);
 //       glBindTexture(GL_TEXTURE_2D, textures[i].id);
 //   }
 //   glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    if (0U != EBO)
    {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    }
    glBindVertexArray(0);
}