#include "entt/entt.hpp"
#include "editor.hpp"
#include "imgui/imgui_stdlib.h"
#include <iostream>
#include <map>

#ifndef INSPECTOR_HPP
#define INSPECTOR_HPP

// Credits go to: https://github.com/Green-Sky/imgui_entt_entity_editor
// For the license of the original code, check vendor/imgui/entt_imgui.hpp

void draw_inspector(entt::registry& registry, Editor& editor, ComponentRegistrar<entt::entity> cr) {
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, 900 / 2), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 900 / 2), ImGuiCond_Once);
	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (registry.valid(editor.current_entity)) {
		ImGui::PushID(static_cast<int>(entt::to_integral(editor.current_entity)));
		std::map<entt::id_type, ComponentInfo> has_not;
		auto e = editor.current_entity;
		auto name = registry.try_get<Name>(e);
	
		if (registry.try_get<Name>(e) != nullptr) {
			ImGui::Text("Name:");
			ImGui::SameLine();
			ImGui::InputText("", &name->name);
		}

		for (auto& [component_type_id, ci] : cr.component_infos) {
			if (cr.entityHasComponent(registry, editor.current_entity, component_type_id)) {
				ImGui::PushID(component_type_id);
				if (ImGui::Button("-")) {
					ci.destroy(registry, e);
					ImGui::PopID();
					continue; // early out to prevent access to deleted data
				}
				else {
					ImGui::SameLine();
				}

				if (ImGui::CollapsingHeader(ci.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Indent(30.f);
					ImGui::PushID("Widget");
					ci.widget(registry, e);
					ImGui::PopID();
					ImGui::Unindent(30.f);
				}
				ImGui::PopID();
			}
			else {
				has_not[component_type_id] = ci;
			}
		}

		if (!has_not.empty()) {
			if (ImGui::Button("+ Add Component")) {
				ImGui::OpenPopup("Add Component");
			}

			if (ImGui::BeginPopup("Add Component")) {
				ImGui::TextUnformatted("Available:");
				ImGui::Separator();

				for (auto& [component_type_id, ci] : has_not) {
					ImGui::PushID(component_type_id);
					if (ImGui::Selectable(ci.name.c_str())) {
						ci.create(registry, e);
					}
					ImGui::PopID();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::PopID();

	}
	else {
		ImGui::Text("Nothing to see here...");
	}

	ImGui::End();
}

#endif