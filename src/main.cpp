#include <vector>

#include "raylib.h"
#include "entt/entt.hpp"
#include "engine.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "resources.hpp"
#include "prefabs.hpp"
#include "serializer.hpp"
#include "spatial_hash.hpp"
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

using std::vector;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define WINDOW_TITLE "bee dog engine"

int main(void)
{
	// ecs related variables
	entt::registry registry;
	vector<System> initialization_systems;
	vector<System> systems;
	vector<System> ui_systems;

	// initialize raylib
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	InitAudioDevice();
	SetTargetFPS(240);

	// append systems
	add_core_systems(systems);
	systems.push_back(debug_rendering);

	// create the camera
	add_camera(registry);

	// initialize resource caches
	add_ctx(registry);

	// registry.on_construct<BoxCollider>().connect<&update_global_spatial_maps>(registry);

	// load level
	if (FileExists(get_asset_path("save.json").c_str())) {
		load_level(registry, "save.json");
	}

	// it is not soooo fast....
	// for (int i = 0; i < 5000; i++) {
	// 	create_sprite_entity(registry, "test.png", i*100, 200);
	// }

	update_global_spatial_maps(registry);

	// add player
	auto p = create_sprite_entity(registry, "test.png");
	registry.emplace<Player>(p);

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		rlImGuiBegin();

		for (auto &system : systems)
		{
			system(registry);
		}

		for (auto &ui_system : ui_systems)
		{
			ui_system(registry);
		}

		EndMode2D(); // only has any effect if a camera exists
		DrawFPS(0, 0);

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}