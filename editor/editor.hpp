#include <string>
#include "entt/entt.hpp"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "resources.hpp"
#include "serializer.hpp"
#include "raylib.h"
#include "imgui/entt_imgui.hpp"

#ifndef EDITOR_HPP
#define EDITOR_HPP

struct Editor;
const int WINDOW_WIDTH = 250;

typedef void (*LoaderCallback) (entt::registry&, Editor&, std::string);

struct Editor {
	entt::entity current_entity = entt::null;
	std::string current_file = "";
	bool loader_open = false;

	std::string hold_cache = "";
	const char* loader_title = "";
	const char* loader_button_title = "";
	LoaderCallback current_callback;

	bool right_arrow_grab = false;
	bool up_arrow_grab = false;
};

void set_current_file(Editor& editor, std::string save_resource) {
	editor.current_file = save_resource;
	auto s = std::string("bee dog editor - ") + save_resource;
	SetWindowTitle(s.c_str());
}

void empty(entt::registry& registry, Editor& editor, std::string save_resource) {
	registry.clear();
	set_current_file(editor, save_resource);
}

void load(entt::registry& registry, Editor& editor, std::string save_resource) {
	if (!FileExists(get_asset_path(save_resource).c_str())) {
		std::cout << "Failed to find file: " << get_asset_path(save_resource) << ", continuing...\n";
		return;
	}

	registry.clear();
	load_level(registry, save_resource);
	set_current_file(editor, save_resource);
}

void save(entt::registry& registry, Editor& editor, std::string save_resource) {
	serialize_level(registry, save_resource);
	set_current_file(editor, save_resource);
}

void center_camera(Camera2D& cam) {
	cam.offset = Vector2{1600/2,900/2};
	cam.target = Vector2{0,0};
}
#endif