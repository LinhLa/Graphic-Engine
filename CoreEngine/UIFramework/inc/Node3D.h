#pragma once
#include <UIObject.h>
#include "Mesh.h"
#include "ShaderProgram.h"
#include "GLTexture.h"
class Node3D final: public UIObject, public creator<Node3D>
{
private:
	MeshPtr				m_pMesh = nullptr;
	ShaderProgramPtr	m_pShaderProgram = nullptr;
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

	void SetMesh(const std::string&);
	void SetProgram(const std::string&);
};

typedef std::shared_ptr<Node3D> Node3DPtr;