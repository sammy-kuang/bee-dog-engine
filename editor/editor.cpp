#include "raylib.h"

#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

#include "editor.hpp"
#include "scene_hierarchy.hpp"
#include "systems.hpp"

#include <vector>
#include <string>

using std::vector, std::string;

void create_temp_entitites(entt::registry& registry) {
	auto entity = registry.create();
	registry.emplace<Name>(registry.create(), "Dog");
	registry.emplace<Name>(registry.create(), "Cat");
	registry.emplace<Name>(registry.create(), "Frog");
	registry.emplace<Name>(registry.create(), "Bird");
}

int main(void)
{
	entt::registry registry;
	Editor editor;
	const int screen_width = 1600;
	const int screen_height = 900;

	create_temp_entitites(registry);

	InitWindow(screen_width, screen_height, "bee dog editor");
	SetTargetFPS(60);

	Vector2 camera_pos = Vector2{ 0, 0 };
	Camera2D camera = { Vector2{0, 0}, Vector2{0, 0}, 0.f, 1.f };

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);

		camera.target = camera_pos;
		draw_sprites(registry);

		EndMode2D();

		rlImGuiBegin();

		draw_scene_hierarchy(registry, editor);

		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}