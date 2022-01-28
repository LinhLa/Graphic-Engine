#pragma once
#include <UIObject.h>
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "GLTexture.h"

class NodeMesh final: public UIObject, public creator<NodeMesh>
{
private:
	MeshPtr				m_pMesh = nullptr;
	ShaderProgramPtr	m_pShaderProgram = nullptr;
	MaterialPtr 		m_pMaterial = nullptr;
protected:
	NodeMesh(std::string name);

	ShaderProgramPtr prepareDynamicShader();
public:
	virtual ~NodeMesh();
	uint8_t getType() override;
	UIObjectPtr clone() override;
	friend class creator<NodeMesh>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);

	void SetMesh(MeshPtr);
	void SetProgram(const std::string& nam);
	void SetMaterial(const std::string& nam);

	MeshPtr getMesh();
	MaterialPtr getMaterial();
};

typedef std::shared_ptr<NodeMesh> NodeMeshPtr;