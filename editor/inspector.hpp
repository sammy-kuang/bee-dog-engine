#include "entt/entt.hpp"
#include "editor.hpp"

#ifndef INSPECTOR_HPP
#define INSPECTOR_HPP

void draw_inspector(entt::registry& registry, Editor& editor) {
	ImGui::SetNextWindowSize(ImVec2(250, 800), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(1600-250-10, 10), ImGuiCond_Once);
	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_MenuBar);


	ImGui::End();
}

#endif