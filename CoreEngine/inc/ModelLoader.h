#pragma once
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "GLTexture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <NodeLight.h>
#include <NodeMesh.h>
#include <Node3D.h>

class ModelLoader
{
    public:
        ModelLoader(const char* _path);
        ~ModelLoader();

        ModelPtr loadModel();

        ModelPtr getModel();
        Node3DPtr getNode();

    private:
        Node3DPtr pObject = nullptr;
        ModelPtr  pModel = nullptr;
        std::string directory;
        std::string name;
        std::string path;
        
        std::vector<MaterialPtr> materials;
        std::vector<MeshPtr> meshes;

        GLTexturePtr TextureFromFile(const char* path, std::string directory);
        void processLight(const aiScene* scene);
        void processNode(aiNode* node, const aiScene* scene);
        MeshPtr processMesh(aiMesh *mesh, const aiScene *scene);
        MaterialPtr processMaterial(aiMesh* mesh, const aiScene* scene);
        std::vector<GLTexturePtr> loadMaterialTextures(aiMaterial *mat, aiTextureType type);

        void LoadPointLight(aiLight* light, UIObjectPtr pLightNode);
        void LoadDirectionalLight(aiLight* light, UIObjectPtr pLightNode);
        void LoadSpotLight(aiLight* light, UIObjectPtr pLightNode);
};