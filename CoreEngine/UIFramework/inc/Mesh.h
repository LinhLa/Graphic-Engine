#pragma once
#include <iostream>
#include <unordered_map>
#include <set>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "creator.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct VertexRecord {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct TextureRecord {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh final: public creator<Mesh>
{
private:
	std::string m_name;
	std::string m_material;

	// mesh data
	std::vector<VertexRecord> vertices;
	std::vector<unsigned int> indices;
	
	//  render data
    uint32_t VAO = 0U;
    uint32_t VBO = 0U;
    uint32_t EBO = 0U;

protected:
	Mesh(std::string name, std::vector<VertexRecord> vertices, std::vector<unsigned int> indices);
public:
	friend class creator<Mesh>;
	~Mesh();

    void setupMesh();
	void Mesh::debug();

	uint32_t vao() const;
	uint32_t vbo() const;
	uint32_t ebo() const;

	size_t vertexCount() const;
	size_t indexCount() const;

	std::string getName() const;
	std::vector<glm::vec3> getVertex();
	std::string getMaterial();
	void setMaterial(std::string);
};

typedef std::shared_ptr<Mesh> MeshPtr;