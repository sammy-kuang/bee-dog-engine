#include "entt/entt.hpp"
#include "editor.hpp"

#ifndef SCENE_HIERARCHY_HPP
#define SCENE_HIERARCHY_HPP


void draw_scene_hierarchy(entt::registry& registry, Editor& editor) {
	// get all entities
	ImGui::SetNextWindowSize(ImVec2(300, 700), ImGuiCond_Once);
	ImGui::Begin("Scene");
	ImGui::Button("Create Entity");
	ImGui::End();
}

#endif