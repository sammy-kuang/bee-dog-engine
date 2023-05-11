#include "entt/entt.hpp"
#include "components.hpp"
#include "prefabs.hpp"
#include "editor.hpp"
#include "imgui/imgui_stdlib.h"
#include <map>
#include <iostream>

#ifndef SCENE_HIERARCHY_HPP
#define SCENE_HIERARCHY_HPP

void close_loader(Editor& editor) {
	editor.loader_open = false;
	editor.hold_cache = "";
	editor.loader_title = "";
	editor.loader_button_title = "";
	editor.current_callback = nullptr;
}

void draw_scene_hierarchy(entt::registry& registry, Editor& editor) {
	// get all entities
	auto entities = registry.view<Name>();

	ImGui::SetNextWindowSize(ImVec2(250, 900 / 2), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save")) {
				if (editor.current_file.empty()) {
					editor.current_callback = save;
					editor.loader_button_title = "Save";
					editor.loader_title = "Save level as...";
					editor.loader_open = true;
				}
				else
					save(registry, editor, editor.current_file);
			}

			if (ImGui::MenuItem("Load")) {
				if (!editor.current_file.empty())
					save(registry, editor, editor.current_file);

				editor.current_callback = load;
				editor.loader_button_title = "Load";
				editor.loader_title = "Load";
				editor.loader_open = true;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::ListBoxHeader("", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() / 2));

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
		editor.current_entity = entt::null;
	}

	if (editor.current_entity != entt::null && ImGui::Button("Delete")) {
		registry.destroy(editor.current_entity);
		editor.current_entity = entt::null;
	}

	ImGui::End();

	if (editor.loader_open) {
		ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() / 2, GetScreenHeight() / 2));
		ImGui::Begin(editor.loader_title, NULL);

		if (ImGui::InputText("", &editor.hold_cache, ImGuiInputTextFlags_EnterReturnsTrue) ||
			ImGui::Button(editor.loader_button_title)) {
			editor.current_callback(registry, editor, editor.hold_cache);
			close_loader(editor);
		}

		ImGui::SameLine();

		if (ImGui::Button("Close")) {
			close_loader(editor);
		}
	}
}
#endif