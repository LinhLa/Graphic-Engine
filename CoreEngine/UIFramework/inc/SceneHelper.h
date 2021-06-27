#pragma once
#include <iostream>
#include <picojson.h>
#include "UIObject.h"

typedef std::unordered_map<std::string, UIObjectPtr> UIObjectTableType;
void LoadPropertyTypeList(picojson::value& json_value);
void LoadCustomPropertyList(picojson::array& o_array, UIObjectPtr pObject);
void LoadTextureList(picojson::value& json_value);
void LoadFragmentShaderList(picojson::value& json_value);
void LoadVertexShaderList(picojson::value& json_value);
void LoadShaderProgramList(picojson::value& json_value);
void LoadMeshList(picojson::value& json_value);
void LoadResourceList(UIObjectTableType&, picojson::value& json_value);