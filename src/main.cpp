#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include "prefabs.hpp"
#include "systems.hpp"
#include "singletons.hpp"
#include <vector>
#include <iostream>
using std::vector;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define WINDOW_TITLE "bd-engine"

int main(void)
{
    // ecs related variables
    entt::registry registry;
    vector<System> systems;

    // register singletons
    register_singletons(registry);

    // initialize raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    // append systems
    add_core_systems(systems);
    systems.push_back(debug_rendering);

    // create some entities
    // auto one = create_sprite_entity(registry, ASSETS_PATH"gura.png", SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2);
    auto two = create_sprite_entity(registry, ASSETS_PATH"gura.png", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2);
    registry.emplace<Player>(two);

    auto view = registry.view<BDTransform>();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto &system : systems) {
            system(registry);
        }

        view.each([](BDTransform &transform) {
            transform.rotation += 360 * GetFrameTime();
        });

        EndDrawing();
    }

    CloseWindow();

    return 0;
}