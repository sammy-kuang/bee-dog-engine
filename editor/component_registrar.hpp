#pragma once
#include "entt/entt.hpp"
#include "imgui/entt_imgui.hpp"
#include "components.hpp"

template <>
void ComponentEditorWidget<BDTransform>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<BDTransform>(e);
	ImGui::DragFloat("x", &t.x);
	ImGui::DragFloat("y", &t.y);
	ImGui::DragFloat("z", &t.z);
	ImGui::DragFloat("Rotation", &t.rotation, 0.5f, 0.f, 360.f);
	ImGui::DragFloat("Scale", &t.scale, 0.1f);
}

template <>
void ComponentEditorWidget<Sprite>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<Sprite>(e);
	ImGui::InputText("Path", &t.path);
}

template <>
void ComponentEditorWidget<BoxCollider>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<BoxCollider>(e);
	ImGui::DragFloat("Width", &t.box.width);
	ImGui::DragFloat("Height", &t.box.height);
	if (ImGui::Button("Rebase on Sprite")) t.rebase_on_sprite(reg, e);
}

template <>
void ComponentEditorWidget<BoxArea>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<BoxArea>(e);
	ImGui::DragFloat("Width", &t.box.width);
	ImGui::DragFloat("Height", &t.box.height);
	if (ImGui::Button("Rebase on Sprite")) t.rebase_on_sprite(reg, e);
}

template <>
void ComponentEditorWidget<AlwaysRender>(entt::registry& reg, entt::registry::entity_type e)
{
}


void register_components(ComponentRegistrar<entt::entity>& cr) {
	cr.registerComponent<BDTransform>("Transform");
	cr.registerComponent<Sprite>("Sprite");
	cr.registerComponent<BoxCollider>("Box Collider");
	cr.registerComponent<BoxArea>("Box Area");
	cr.registerComponent<AlwaysRender>("Always Render");
}

ComponentRegistrar<entt::entity>& add_registrar(entt::registry& reg) {
	reg.ctx().emplace<ComponentRegistrar<entt::entity>>();
	auto& cr = reg.ctx().get<ComponentRegistrar<entt::entity>>();
	register_components(cr);
	return cr;
}