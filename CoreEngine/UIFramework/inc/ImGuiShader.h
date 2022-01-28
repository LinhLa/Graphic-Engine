#pragma once
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "UIObject.h"
#include "IProperty.h"
#include "Material.h"
#include "SingletonHolder.h"

class ImGuiShader final: public SingletonHolder<ImGuiShader>, public PropertyTable
{
private:
	ImGuiShader();

	std::vector<std::string> m_materials;
	std::vector<std::string> m_shaders;
	std::vector<std::string> m_textures;
	std::vector<std::string> m_models;
	std::vector<std::string> m_resources;


	std::string m_material_selected;
	std::string m_shader_selected;
	std::string m_texture_selected;
	std::string m_model_selected;
	std::string m_resource_selected;

	bool m_show_property_window = false;

	UIObjectPtr m_node_clicked = nullptr;

	void onMaterialUpdate(VoidType&&);
	void onShaderUpdate(VoidType&&);
	void onTextureUpdate(VoidType&&);
	void onModelUpdate(VoidType&&);

	bool DrawImageControl(std::string title, glm::u32 &texture_unit);
	void DrawImControl(std::unordered_map<std::string, IPropertyPtr> propertyTable);
public:
	friend class SingletonHolder<ImGuiShader>;
	~ImGuiShader();
	void ProcessEvent(SDL_Event&);
	void StartNewFrame();
	void Show();
	void ShowPropertyWindow();
};