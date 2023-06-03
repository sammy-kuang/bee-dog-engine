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
	registry.clear();
	load_level(registry, save_resource);
	set_current_file(editor, save_resource);
}

void save(entt::registry& registry, Editor& editor, std::string save_resource) {
	serialize_level(registry, save_resource);
	set_current_file(editor, save_resource);
}

void center_camera(Camera2D& cam) {

	// get the world point that is under the mouse
	Vector2 mouseWorldPos = Vector2{-1600 / 2, - 900 / 2};

	// set the offset to where the mouse is
	cam.offset = GetWorldToScreen2D(mouseWorldPos, cam);

	// set the target to match, so that the camera maps the world space point under the cursor to the screen space point under the cursor at any zoom
	cam.target = mouseWorldPos;

	std::cout << "Invoked center camera\n";

}
#endif