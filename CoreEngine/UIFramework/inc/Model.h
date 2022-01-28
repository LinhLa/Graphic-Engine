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
        std::vector<MeshPtr> getMeshes();
        std::string getName() const;
        void debug();
    private:
        friend class creator<Model>;
        Model(std::string, std::vector<MeshPtr>);
        // model data
        std::string m_name;
        std::vector<MeshPtr> meshes;
};

typedef std::shared_ptr<Model> ModelPtr;