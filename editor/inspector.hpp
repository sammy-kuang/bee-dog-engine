#include "entt/entt.hpp"
#include "editor.hpp"
#include "imgui/imgui_stdlib.h"
#include <iostream>

#ifndef INSPECTOR_HPP
#define INSPECTOR_HPP

void draw_inspector(entt::registry& registry, Editor& editor) {
	ImGui::SetNextWindowSize(ImVec2(250, 900/2), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 900/2), ImGuiCond_Once);
	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	if (editor.current_entity != entt::null && editor.current_entity != entt::tombstone) {
		auto entity = editor.current_entity;
		auto transform = registry.try_get<BDTransform>(entity);
		auto sprite = registry.try_get<Sprite>(entity);
		auto box_collider = registry.try_get<BoxCollider>(entity);
		auto box_area = registry.try_get<BoxArea>(entity);
		auto name = registry.try_get<Name>(entity);

		if (name != nullptr) {
			ImGui::SeparatorText("Name");
			ImGui::InputText("", &name->name, ImGuiInputTextFlags_EnterReturnsTrue);
		}

		if (transform != nullptr) {
			ImGui::SeparatorText("Transform");
			ImGui::InputFloat("x", &transform->x);
			ImGui::InputFloat("y", &transform->y);
			ImGui::InputFloat("z", &transform->z);
			ImGui::InputFloat("Rotation", &transform->rotation);
			ImGui::InputFloat("Scale", &transform->scale);
		}

		if (sprite != nullptr) {
			ImGui::SeparatorText("Sprite");
			ImGui::InputText("Resource", &sprite->path, ImGuiInputTextFlags_EnterReturnsTrue);
		}

		if (box_collider != nullptr) {
			ImGui::SeparatorText("Box Collider");
			ImGui::PushID("BoxCollider");
			ImGui::InputFloat("Width", &box_collider->box.width);
			ImGui::InputFloat("Height", &box_collider->box.height);
			if (ImGui::Button("Rebase on Sprite")) box_collider->rebase_on_sprite(registry, entity);
			ImGui::PopID();
		}

		if (box_area != nullptr) {
			ImGui::SeparatorText("Box Area");
			ImGui::PushID("BoxArea");
			ImGui::InputFloat("Width", &box_area->box.width);
			ImGui::InputFloat("Height", &box_area->box.height);
			if (ImGui::Button("Rebase on Sprite")) box_area->rebase_on_sprite(registry, entity);
			ImGui::PopID();
		}
	}
	else {
		ImGui::Text("Nothing to see here...");
	}

	ImGui::End();
}

#endif