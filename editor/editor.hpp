#include <string>
#include "entt/entt.hpp"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "resources.hpp"
#include "serializer.hpp"
#include "raylib.h"

#ifndef EDITOR_HPP
#define EDITOR_HPP

struct Editor;

typedef void (*LoaderCallback) (entt::registry&, Editor&, std::string);

struct Editor {
	entt::entity current_entity = entt::null;
	std::string current_file = "";
	bool loader_open = false;

	std::string hold_cache = "";
	const char* loader_title = "";
	const char* loader_button_title = "";
	LoaderCallback current_callback;
};

void load(entt::registry& registry, Editor& editor, std::string save_resource) {
	registry.clear();
	load_level(registry, save_resource);
	editor.current_file = save_resource;
	auto s = std::string("bee dog editor - ") + save_resource;
	SetWindowTitle(s.c_str());
}

void save(entt::registry& registry, Editor& editor, std::string save_resource) {
	serialize_level(registry, save_resource);
	editor.current_file = save_resource;
	auto s = std::string("bee dog editor - ") + save_resource;
	SetWindowTitle(s.c_str());

}

#endif