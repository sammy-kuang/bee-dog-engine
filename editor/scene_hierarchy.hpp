#include "entt/entt.hpp"
#include "components.hpp"
#include "prefabs.hpp"
#include "editor.hpp"
#include <map>
#include <iostream>

#ifndef SCENE_HIERARCHY_HPP
#define SCENE_HIERARCHY_HPP


void draw_scene_hierarchy(entt::registry& registry, Editor& editor) {
	// get all entities
	auto entities = registry.view<Name>();

	ImGui::SetNextWindowSize(ImVec2(250, 800), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
	ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar);
	
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New")) {
				// create a new .level file
			}

			if (ImGui::MenuItem("Save")) {
				// save
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::ListBoxHeader("", ImVec2(ImGui::GetWindowWidth()-20, ImGui::GetWindowHeight()-100));

	for (auto& entity : entities) {
		auto& name = registry.get<Name>(entity);
		//auto& transform = registry.get<BDTransform>(entity);
		
		bool selected = editor.current_entity == entity;

		if (ImGui::Selectable(name.name.c_str(), &selected)) {
			editor.current_entity = entity;
			std::cout << "Selected: " << name.name << "\n";
		}

	}
	ImGui::ListBoxFooter();

	if (ImGui::Button("Create Entity")) {
		create_sprite_entity(registry, "gura.png");
	}
	if (ImGui::Button("Clear")) {
		registry.clear();
	}

	ImGui::End();
}

#endif