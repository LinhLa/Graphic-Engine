#pragma once
#include <UIObject.h>
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "GLTexture.h"

class Node3D final: public UIObject, public creator<Node3D>
{
private:
	ModelPtr 			m_pModel = nullptr;
	ShaderProgramPtr	m_pShaderProgram = nullptr;
	MaterialPtr 		m_pMaterial = nullptr;
	bool				m_bUIChanged = false;
protected:
	Node3D(std::string name);
public:
	virtual ~Node3D();
	uint8_t getType() override;

	friend class creator<Node3D>;

	void onDraw(VoidType&&);
	void onInit(VoidType&&);
	void onClean(VoidType&&);

	float GetActualWidth() const;
	float GetActualHeight() const;

	void SetProgram(const std::string&);
	void SetModel(const std::string& name);
	void SetMaterial(const std::string& name);
};

typedef std::shared_ptr<Node3D> Node3DPtr;