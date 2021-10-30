#pragma once
#include <iostream>
#include <picojson.h>
#include "UIObject.h"
#include "Node3D.h"
typedef std::unordered_map<std::string, UIObjectPtr> UIObjectTableType;
void LoadPropertyTypeList(picojson::value& json_value);
void LoadGLProperty(picojson::object& o_glProperty, Node3DPtr pObject);
void LoadMaterialPropertyList(picojson::array& o_array, Node3DPtr pObject);
void LoadTextureList(picojson::value& json_value);
void LoadFragmentShaderList(picojson::value& json_value);
void LoadVertexShaderList(picojson::value& json_value);
void LoadShaderProgramList(picojson::value& json_value);
void LoadMeshList(picojson::value& json_value);
void LoadModel(picojson::value& json_value);
void LoadResourceList(UIObjectTableType&, picojson::value& json_value);