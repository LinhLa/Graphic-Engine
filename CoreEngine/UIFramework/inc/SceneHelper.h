#pragma once
#include <iostream>
#include <picojson.h>
#include "UIObject.h"
#include "Node3D.h"
#include "NodeMesh.h"

typedef std::unordered_map<std::string, UIObjectPtr> UIObjectTableType;
void LoadPropertyTypeList(picojson::value& json_value);
void LoadMaterialList(picojson::value& json_value);

void LoadTextureList(picojson::value& json_value);

void LoadShader(std::string, GLenum);
void LoadShaderProgramList(picojson::value& json_value);

void LoadMeshList(picojson::value& json_value);
void LoadModel(UIObjectTableType& UIObjectTable, picojson::value& json_value);

void LoadResourceList(UIObjectTableType&, picojson::value& json_value);

void LoadLayoutProperty(picojson::object& o_layout, UIObjectPtr pObject);
void LoadGLProperty(picojson::object& o_glProperty, Node3DPtr pObject);

void LoadMaterialProperty(picojson::object& o_glMaterial, NodeMeshPtr pNode);
void LoadLightingProperty(picojson::value& o_Lighting, Node3DPtr pObject);