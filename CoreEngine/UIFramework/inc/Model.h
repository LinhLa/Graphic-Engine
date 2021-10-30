#pragma once
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model final : public creator<Model>
{
    public:
        ~Model();
        void Draw(ShaderProgramPtr pShader);  
        std::string getName() const;
        void debug();
    private:
        friend class creator<Model>;
        Model(const char* path);
        Model(std::string, std::vector<MeshPtr>);
        // model data
        std::string m_name;
        std::string directory;
        std::vector<MeshPtr> meshes;
    
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        MeshPtr processMesh(aiMesh *mesh, const aiScene *scene);
        MaterialPtr processMaterial(aiMesh* mesh, const aiScene* scene);
        std::vector<TextureRecord> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

typedef std::shared_ptr<Model> ModelPtr;