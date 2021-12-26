#pragma once
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "IProperty.h"
#include "Material.h"
#include "SingletonHolder.h"

class ImGuiShader final: public SingletonHolder<ImGuiShader>
{
private:
	std::map<std::string, IPropertyPtr> m_list;

	ImGuiShader();
public:
	friend class SingletonHolder<ImGuiShader>;
	~ImGuiShader();

	void importProperty(std::shared_ptr<PropertyTable> pTable);
	void setProperty(std::shared_ptr<PropertyTable> pTable);
	void ProcessEvent(SDL_Event&);
	void StartNewFrame();
	void ShowDemo();
	void Show();
};