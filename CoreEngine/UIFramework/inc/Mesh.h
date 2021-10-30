#pragma once
#include <iostream>
#include <unordered_map>

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
	// mesh data
	std::vector<VertexRecord>       vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureRecord>      textures;
	
	//  render data
    uint32_t VAO = 0U;
    uint32_t VBO = 0U;
    uint32_t EBO = 0U;

protected:
	Mesh(std::vector<VertexRecord> vertices, std::vector<unsigned int> indices, std::vector<TextureRecord> textures);
public:
	friend class creator<Mesh>;
	~Mesh();

    void setupMesh();
	void Mesh::debug();
	std::string getName() const;
	void Draw(ShaderProgramPtr pShader);
};

typedef std::shared_ptr<Mesh> MeshPtr;