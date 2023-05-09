#include <vector>

#include "raylib.h"
#include "entt/entt.hpp"
#include "components.hpp"
#include "systems.hpp"
#include "resources.hpp"
#include "prefabs.hpp"
#include "serializer.hpp"
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
	SetTargetFPS(60);

	// append systems
	add_core_systems(systems);
	systems.push_back(debug_rendering);

	// create the camera
	add_camera(registry);

	// initialize resource caches
	add_ctx(registry);

	auto save_file = ASSETS_PATH "save.level";

	if (!FileExists(save_file)) {
		//create some entities
		auto player = create_sprite_entity(registry, "gura.png", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, -1.f);
		registry.emplace<Player>(player);
		auto& vel = registry.get<Velocity>(player);

		int rand = GetRandomValue(1, 10);
		std::cout << "Generating " << rand << " entities\n";
		for (int i = 0; i < rand; i++)
		{
			auto entity = create_sprite_entity(registry, "gura.png", (float)(SCREEN_WIDTH / 2 - 400 + i * 100.1), (float)(SCREEN_HEIGHT / 2 + 200 + GetRandomValue(-10, 10)));
			registry.emplace<Floor>(entity);
		}
	}

	for (auto& system : initialization_systems)
	{
		system(registry);
	}

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		rlImGuiBegin();

		for (auto& system : systems)
		{
			system(registry);
		}

		for (auto& ui_system : ui_systems)
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