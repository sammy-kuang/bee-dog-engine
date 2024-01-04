#include "entt/entt.hpp"
#include "engine.hpp"
#include "raylib.h"
#include "raymath.h"
#include "resources.hpp"
#include "serializer.hpp"
#include "systems.hpp"
#include "prefabs.hpp"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

void init(entt::registry& registry) {
	// load our demo level
	if (FileExists(get_asset_path("save.json").c_str())) {
		load_level(registry, "save.json");
	}

	// add the player
	auto p = create_sprite_entity(registry, "test.png");
	registry.emplace<Player>(p);
}

void player_controller(entt::registry &registry)
{
	auto view = registry.view<Player, Velocity, BDTransform>();
	auto camera_view = registry.view<Camera2D>();

	for (auto &player : view)
	{
		int speed = 250;
		BDTransform &t = registry.get<BDTransform>(player);
		Velocity &v = registry.get<Velocity>(player);
		if (IsKeyDown(KEY_W))
		{
			v.y = -speed;
		}
		else if (IsKeyDown(KEY_S))
		{
			v.y = speed;
		}
		else
		{
			v.y = 0;
		}

		if (IsKeyDown(KEY_A))
		{
			v.x = -speed;
		}
		else if (IsKeyDown(KEY_D))
		{
			v.x = speed;
		}
		else
		{
			v.x = 0;
		}

		Camera2D &c = registry.ctx().get<Camera2D>();
		// update the camera
		c.target = Vector2{t.x - GetScreenWidth() / 2, t.y - GetScreenHeight() / 2};

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			spawn_particle_system(registry, basic_particle_system, Vector2{t.x, t.y});
		}
	}
}

int main(void)
{
	Engine engine = init_engine(init, "bee dog engine", SCREEN_WIDTH, SCREEN_HEIGHT);
	add_system(engine, player_controller);
	add_system(engine, particle_system);
	run_engine(engine);
	return 0;
}