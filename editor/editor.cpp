#include "editor.hpp"
#include "scene_hierarchy.hpp"
#include "inspector.hpp"
#include "systems.hpp"
#include "component_registrar.hpp"
#include "editor_systems.hpp"
#include <vector>

using std::vector, std::string;

int main(void)
{
	entt::registry registry;
	vector<System> systems;
	const int screen_width = 1600;
	const int screen_height = 900;

	auto &editor = registry.ctx().emplace<Editor>();
	add_ctx(registry);
	add_move_systems(systems);
	add_editor_systems(systems);

	auto& cr = add_registrar(registry);

	InitWindow(screen_width, screen_height, "bee dog editor");
	SetTargetFPS(60);

	auto &camera = add_camera(registry);
	Vector2 camera_pos = Vector2{ -screen_width / 2, -screen_height / 2 };
	camera.target = camera_pos;

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(camera);

		draw_sprites(registry);
		debug_rendering(registry);

		for (auto& system : systems) {
			system(registry);
		}

		EndMode2D();
		rlImGuiBegin();

		draw_scene_hierarchy(registry, editor);
		draw_inspector(registry, editor, cr);

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}