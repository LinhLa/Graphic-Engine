#include "stdafx.h"
#include "ImGuiShader.h"
#include "log.h"

#include "GLProperty.h"
#include "Model.h"
#include "Library.h"
#include "Scene.h"

#include "SignalDefine.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <NodeMesh.h>

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static void createTreeItem(UIObjectPtr pObject, ImGuiTreeNodeFlags base_flags, std::string selection, UIObjectPtr& node_clicked, bool test_drag_and_drop, int index = 0)
{
	static glm::u32 textureID = Library::GetInstance()->get<GLTexture>("node")->getID();
	if (!pObject)
	{
		return;
	}

	if (index == 0)
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);

	ImGui::Image((void*)textureID, ImVec2(20.0f, 20.0f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
	ImGui::SameLine();
	if (ImGui::TreeNode((void*)(intptr_t)index, pObject->getName().c_str()))
	{
		//Draw parent node
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		ImGuiTreeNodeFlags node_flags = base_flags;
		const bool is_selected = (selection == pObject->getName());
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::IsItemClicked())
			node_clicked = pObject;

		if (test_drag_and_drop && ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
			ImGui::Text("This is a drag and drop source");
			ImGui::EndDragDropSource();
		}

		//Draw child node 
		UIObjectPtr child = nullptr;
		int i = 0;
		while (child = pObject->getChild<UIObject>(i))
		{
			createTreeItem(child, base_flags, selection, node_clicked, test_drag_and_drop, i++);
		}
		ImGui::TreePop();
	}
}

bool ImGuiShader::DrawImageControl(std::string title, glm::u32& selected_texture_unit)
{
	bool bSelected = false;
	static std::map<std::string, std::string> selected_map;
	if (selected_map.find(title) == selected_map.end())
	{
		selected_map[title] = std::string("...");
	}

	// Expose flags as checkbox for the demo
	static ImGuiComboFlags flags = 0;
	//ImGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", &flags, ImGuiComboFlags_PopupAlignLeft);
	//ImGui::SameLine(); HelpMarker("Only makes a difference if the popup is larger than the combo");
	//if (ImGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", &flags, ImGuiComboFlags_NoArrowButton))
	//	flags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
	//if (ImGui::CheckboxFlags("ImGuiComboFlags_NoPreview", &flags, ImGuiComboFlags_NoPreview))
	//	flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both

	if (ImGui::BeginCombo(title.c_str(), selected_map[title].c_str(), flags))
	{
		for (auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
		{
			const bool is_selected = (selected_map[title] == *itr);
			if (ImGui::Selectable(itr->c_str(), is_selected))
			{
				selected_map[title] = *itr;
				//<handle selected item
				auto pTexture = Library::GetInstance()->get<GLTexture>(*itr);
				selected_texture_unit = pTexture->getID();
				bSelected = true;
			}
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Image((void*)(selected_texture_unit), ImVec2(80.0f, 60.0f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
	return bSelected;
}

void ImGuiShader::DrawImControl(std::unordered_map<std::string, IPropertyPtr> propertyTable)
{
	for (auto& item : propertyTable)
	{
		auto pProperty = item.second;
		switch (pProperty->GetType())
		{
		case GL_BOOL:
		{
			auto property = std::dynamic_pointer_cast<Property<bool>>(pProperty);
			if (ImGui::Checkbox(item.first.c_str(), property->getPointer()))
			{
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_INT:
		{
			auto property = std::dynamic_pointer_cast<Property<glm::i32>>(pProperty);
			if (ImGui::SliderInt(item.first.c_str(), property->getPointer(), 0, 100))
			{				
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_FLOAT:
		{
			auto property = std::dynamic_pointer_cast<Property<glm::f32>>(pProperty);
			if (ImGui::SliderFloat(item.first.c_str(), property->getPointer(), -1000.0f, 1000.0f))
			{
				// Edit 1 float using a slider from 0.0f to 1.000f
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_FLOAT_VEC2:
		{
			auto property = std::dynamic_pointer_cast<Property<glm::vec2>>(pProperty);
			if (ImGui::SliderFloat2(item.first.c_str(), glm::value_ptr(property->getRef()), -1000.0f, 1000.0f))            // Edit 1 float using a slider from 0.0f to 1.000f
			{
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_FLOAT_VEC4:
		{
			auto property = std::dynamic_pointer_cast<Property<glm::vec4>>(pProperty);
			if (ImGui::ColorEdit4(item.first.c_str(), glm::value_ptr(property->getRef()))) // Edit 4 floats representing a color
			{
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_FLOAT_VEC3:
		{
			auto property = std::dynamic_pointer_cast<Property<glm::vec3>>(pProperty);
			if (ImGui::SliderFloat3(item.first.c_str(), glm::value_ptr(property->getRef()), -1000.0f, 1000.0f)) // Edit 3 floats representing a color
			{
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_INT_VEC4:
		{
			auto property = std::dynamic_pointer_cast<Property<glm::i32vec4>>(pProperty);
			if (ImGui::SliderInt4(item.first.c_str(), glm::value_ptr(property->getRef()), -10000, 10000))
			{
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_SAMPLER_2D:
		{
			auto property = std::dynamic_pointer_cast<Property<glm::u32>>(pProperty);
			if (DrawImageControl(item.first, property->getRef()))
			{
				property->m_pSignalValueChange->onSignal(property->GetValue());
			}
			break;
		}
		case GL_FLOAT_MAT4:
		{
			break;
		}
		default:
		{
			//LOG_DEBUG("Not support type[%s]", item.second->getTypeInfo().name());
			break;
		}
		}
	}
}

ImGuiShader::ImGuiShader()
{
	auto library = Library::GetInstance();
	library->getSignalTable<Material>()->bind(ON_UPDATE_SIGNAL, this, &ImGuiShader::onMaterialUpdate);
	library->getSignalTable<ShaderProgram>()->bind(ON_UPDATE_SIGNAL, this, &ImGuiShader::onShaderUpdate);
	library->getSignalTable<GLTexture>()->bind(ON_UPDATE_SIGNAL, this, &ImGuiShader::onTextureUpdate);
	library->getSignalTable<Model>()->bind(ON_UPDATE_SIGNAL, this, &ImGuiShader::onModelUpdate);

	//<Get info list
	onMaterialUpdate({});
	onShaderUpdate({});
	onTextureUpdate({});
	onModelUpdate({});
}

ImGuiShader::~ImGuiShader() {}

void ImGuiShader::onMaterialUpdate(VoidType&&)
{
	m_materials = std::move(Library::GetInstance()->get<Material>());
}

void ImGuiShader::onShaderUpdate(VoidType&&)
{
	m_shaders = std::move(Library::GetInstance()->get<ShaderProgram>());
}

void ImGuiShader::onTextureUpdate(VoidType&&)
{
	m_textures = std::move(Library::GetInstance()->get<GLTexture>());
}

void ImGuiShader::onModelUpdate(VoidType&&)
{
	m_models = std::move(Library::GetInstance()->get<Model>());
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

void ImGuiShader::ShowPropertyWindow()
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
	std::string title = "Property window";
	if (!ImGui::Begin(title.c_str(), &m_show_property_window))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	

	MaterialPtr pMaterial = nullptr;
	if (m_node_clicked && std::dynamic_pointer_cast<NodeMesh>(m_node_clicked))
	{
		pMaterial = std::dynamic_pointer_cast<NodeMesh>(m_node_clicked)->getMaterial();
	}
	else
	{
		pMaterial = Library::GetInstance()->get<Material>(m_material_selected);
	}
	
	title = "Material [" + (pMaterial ? pMaterial->getName() : std::string("...")) + "]";
	if (ImGui::TreeNode(title.c_str()))
	{
		if (pMaterial) DrawImControl(std::dynamic_pointer_cast<PropertyTable>(pMaterial)->m_propertyTable);
			ImGui::TreePop();
	}

	title = "Node [" + (m_node_clicked ? m_node_clicked->getName() : std::string("...")) + "]";
	if (ImGui::TreeNode(title.c_str()))
	{
		if (m_node_clicked) DrawImControl(std::dynamic_pointer_cast<PropertyTable>(m_node_clicked)->m_propertyTable);
		ImGui::TreePop();
	}
	
	ImGui::End();
}

void ImGuiShader::Show()
{
	// Our state
	static bool open = false;
	static bool show_demo_window = true;
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)			ImGui::ShowDemoWindow(&show_demo_window);
	if (m_show_property_window)		ShowPropertyWindow();

	ImGui::Begin("Library");                          // Create a window

	if (ImGui::TreeNode("Material"))
	{
		if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (auto itr = m_materials.begin(); itr < m_materials.end(); ++itr)
			{
				const bool is_selected = (m_material_selected == *itr);
				if (ImGui::Selectable(itr->c_str(), is_selected))
					m_material_selected = *itr;
					
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Shader"))
	{
		if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (auto itr = m_shaders.begin(); itr != m_shaders.end(); ++itr)
			{
				const bool is_selected = (m_shader_selected == *itr);
				if (ImGui::Selectable(itr->c_str(), is_selected))
					m_shader_selected = *itr;
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Texture"))
	{
		if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
			{
				const bool is_selected = (m_texture_selected == *itr);
				if (ImGui::Selectable(itr->c_str(), is_selected))
					m_texture_selected = *itr;
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		auto pTexture = Library::GetInstance()->get<GLTexture>(m_texture_selected);
		if (pTexture)
		{
			auto textureID = pTexture->getID();
			ImGui::Image((void*)(textureID), ImVec2(80.0f, 60.0f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Model"))
	{
		if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (auto itr = m_models.begin(); itr != m_models.end(); ++itr)
			{
				const bool is_selected = (m_model_selected == *itr);
				if (ImGui::Selectable(itr->c_str(), is_selected))
					m_model_selected = *itr;
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Resource"))
	{
		m_resources = std::move(Scene::GetInstance()->GetResource());
		if (ImGui::BeginListBox("", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (auto itr = m_resources.begin(); itr != m_resources.end(); ++itr)
			{
				const bool is_selected = (m_resource_selected == *itr);
				if (ImGui::Selectable(itr->c_str(), is_selected))
					m_resource_selected = *itr;
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Scene Graph"))
	{
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static bool align_label_with_current_x_position = false;
		static bool test_drag_and_drop = false;

		HelpMarker(
			"This is a more typical looking tree with selectable nodes.\n"
			"Click to select, CTRL+Click to toggle, click on arrows or double-click to open.");

		ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", &base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
		ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", &base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
		ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", &base_flags, ImGuiTreeNodeFlags_SpanAvailWidth); ImGui::SameLine(); HelpMarker("Extend hit area to all available width instead of allowing more items to be laid out after the node.");
		ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
		ImGui::Checkbox("Align label with current X position", &align_label_with_current_x_position);
		ImGui::Checkbox("Test tree node as drag source", &test_drag_and_drop);
		ImGui::Text("Hello!");
		if (align_label_with_current_x_position)
			ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

		// 'selection_mask' is dumb representation of what may be user-side selection state.
		//  You may retain selection state inside or outside your objects in whatever format you see fit.
		// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
		/// of the loop. May be a pointer to your own node type, etc.
		std::string selection = m_node_clicked ? m_node_clicked->getName() : "";

		auto root = Scene::GetInstance()->GetRoot();
		createTreeItem(root, base_flags, selection, m_node_clicked, test_drag_and_drop);


		if (!m_node_clicked)	//do some thing;

		if (align_label_with_current_x_position)
			ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

		ImGui::TreePop();
	}

	ImGui::Text("Open Window");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Property Window", &m_show_property_window);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}