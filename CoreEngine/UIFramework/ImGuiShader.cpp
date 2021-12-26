#include "stdafx.h"
#include "ImGuiShader.h"
#include "log.h"

#include "GLProperty.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Our state
bool open = false;
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

ImGuiShader::ImGuiShader() {}

ImGuiShader::~ImGuiShader() {}

void ImGuiShader::importProperty(std::shared_ptr<PropertyTable> pTable)
{
	for (auto& item : pTable->m_propertyTable)
	{
		m_list.insert(std::make_pair(item.first,  item.second->clone()));
	}
}

void ImGuiShader::setProperty(std::shared_ptr<PropertyTable> pTable)
{
	for (auto& item : m_list)
	{
		auto pProperty = pTable->GetProperty(item.first);
		if (pProperty)
		{
			item.second->SetValue(pProperty);
		}
	}
}

void ImGuiShader::ProcessEvent(SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void ImGuiShader::StartNewFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}
void ImGuiShader::Show()
{
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (!ImGui::Begin("Shader input"))
	{
		return;
	}

	for (auto& item : m_list)
	{
		auto pProperty = item.second;
		switch (pProperty->GetType())
		{
			case BOOL:
			{
				ImGui::Checkbox(item.first.c_str(), std::dynamic_pointer_cast<Property<bool>>(pProperty)->getPointer());
				break;
			}
			case INT:
			{
				ImGui::SliderInt(item.first.c_str(), std::dynamic_pointer_cast<Property<glm::i32>>(pProperty)->getPointer(), 0.0f, 100.0f);
				break;
			}
			case FLOAT:
			{
				ImGui::SliderFloat(item.first.c_str(), std::dynamic_pointer_cast<Property<glm::f32>>(pProperty)->getPointer(), 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.000f
				break;
			}
			case VEC4_COLOR:
			{
				ImGui::ColorEdit4(item.first.c_str(), glm::value_ptr(std::dynamic_pointer_cast<Property<glm::vec4>>(pProperty)->getRef())); // Edit 4 floats representing a color
				break;
			}
			case VEC3_COLOR:
			{
				ImGui::ColorEdit3(item.first.c_str(), glm::value_ptr(std::dynamic_pointer_cast<Property<glm::vec3>>(pProperty)->getRef())); // Edit 3 floats representing a color
				break;
			}
			case VEC3:
			{
				ImGui::SliderFloat3(item.first.c_str(), glm::value_ptr(std::dynamic_pointer_cast<Property<glm::vec3>>(pProperty)->getRef()), -100.0f, 100.0f);
				break;
			}
			case VEC4:
			{
				ImGui::SliderFloat4(item.first.c_str(), glm::value_ptr(std::dynamic_pointer_cast<Property<glm::vec4>>(pProperty)->getRef()), - 100.0f, 100.0f);
				break;
			}
			case CAMERA_VIEW_TYPE:
			{
				//ImGuiStyle& style = ImGui::GetStyle();
				//float w = ImGui::CalcItemWidth();
				//float spacing = style.ItemInnerSpacing.x;
				//float button_sz = ImGui::GetFrameHeight();
				//ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
				//if (ImGui::BeginCombo(item.first.c_str(), nullptr)) // The second parameter is the label previewed before opening the combo.
				//{
				//	for (int n = 0; n < 2; n++)
				//	{
				//		bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
				//		if (ImGui::Selectable(items[n], is_selected))
				//		{
				//			current_item = items[n];
				//			std::dynamic_pointer_cast<Property<uint8_t>>(pProperty)->getRef() = cameraMap.at(current_item);
				//		}
				//		if (is_selected)
				//			ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				//	}
				//	ImGui::EndCombo();
				//}
				//break;
			}
			default:
			{
				//LOG_DEBUG("Not support type[%s]", item.second->getTypeInfo().name());
				break;
			}
		}
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void ImGuiShader::ShowDemo()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}