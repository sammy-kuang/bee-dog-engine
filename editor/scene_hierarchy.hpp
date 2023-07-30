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

void open_loader(Editor& editor, LoaderCallback callback, const char* title, const char* button) {
	editor.current_callback = callback;
	editor.loader_button_title = button;
	editor.loader_title = title;
	editor.loader_open = true;
}

void draw_scene_hierarchy(entt::registry& registry, Editor& editor) {
	// get all entities
	auto entities = registry.view<Name>();

	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, 900 / 2), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New")) {
				open_loader(editor, empty, "New", "Save new level as...");
			}

			if (ImGui::MenuItem("Save")) {
				if (editor.current_file.empty()) {
					open_loader(editor, save, "New", "Save level as...");
				}
				else
					save(registry, editor, editor.current_file);
			}

			if (ImGui::MenuItem("Load")) {
				// We should add some verfication prompt
				// instead of saving for them

				/*if (!editor.current_file.empty())
					save(registry, editor, editor.current_file);*/

				open_loader(editor, load, "Load", "Load...");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Recenter Camera")) {
				center_camera(registry.ctx().get<Camera2D>());
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::ListBoxHeader("##", ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() / 2));

	for (auto& entity : entities) {
		auto& name = registry.get<Name>(entity);
		bool selected = editor.current_entity == entity;

		if (ImGui::Selectable(name.name.c_str(), &selected)) {
			editor.current_entity = entity;
			std::cout << "Selected: " << name.name << "\n";
		}

	}
	ImGui::ListBoxFooter();

	if (ImGui::Button("Create Entity")) {
		auto e = create_sprite_entity(registry, "test.png");
		editor.current_entity = e;
	}

	if (ImGui::Button("Clear")) {
		registry.clear();
		editor.current_entity = entt::null;
	}

	if (editor.current_entity != entt::null && ImGui::Button("Delete")) {
		registry.destroy(editor.current_entity);
		editor.current_entity = entt::null;
	}

	if (registry.valid(editor.current_entity) && ImGui::Button("Deselect")) {
		editor.current_entity = entt::null;
	}

	ImGui::End();

	if (editor.loader_open) {
		ImGui::SetNextWindowPos(ImVec2((float)GetScreenWidth() / 2.f, (float)GetScreenHeight() / 2.f), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);
		ImGui::Begin(editor.loader_title, NULL);

		if (ImGui::InputText("##", &editor.hold_cache, ImGuiInputTextFlags_EnterReturnsTrue) ||
			ImGui::Button(editor.loader_button_title)) {
			editor.current_callback(registry, editor, editor.hold_cache);
			close_loader(editor);
		}

		ImGui::SameLine();

		if (ImGui::Button("Close")) {
			close_loader(editor);
		}

		ImGui::End();
	}
}
#endif