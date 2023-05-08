#include "raylib.h"

#include "editor.hpp"
#include "scene_hierarchy.hpp"
#include "inspector.hpp"
#include "systems.hpp"

#include <vector>
#include <string>

using std::vector, std::string;

int main(void)
{
	entt::registry registry;
	Editor editor;
	const int screen_width = 1600;
	const int screen_height = 900;

	//create_temp_entitites(registry);
	add_ctx(registry);

	InitWindow(screen_width, screen_height, "bee dog editor");
	SetTargetFPS(60);

	Vector2 camera_pos = Vector2{ -screen_width / 2, -screen_height / 2 };
	Camera2D camera = { Vector2{0, 0}, Vector2{0, 0}, 0.f, 1.f };

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);

		camera.target = camera_pos;
		draw_sprites(registry);
		debug_rendering(registry);

		EndMode2D();

		rlImGuiBegin();

		draw_scene_hierarchy(registry, editor);
		draw_inspector(registry, editor);

		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}