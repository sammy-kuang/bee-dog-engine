#include "entt/entt.hpp"
#include "engine.hpp"
#include "raylib.h"
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

int main(void)
{
	Engine engine = init_engine(init, "bee dog engine", SCREEN_WIDTH, SCREEN_HEIGHT);
	run_engine(engine);
	return 0;
}