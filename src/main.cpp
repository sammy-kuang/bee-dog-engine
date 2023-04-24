#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include "prefabs.hpp"
#include "systems.hpp"
#include <vector>
#include <iostream>
using std::vector;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define WINDOW_TITLE "bd-engine"

int main(void)
{
    // ecs related variables
    entt::registry registry;
    vector<System> systems;

    // initialize raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    // append systems
    systems.push_back(draw_sprites);
    systems.push_back(apply_velocity);
    systems.push_back(player_controller);
    auto entity = create_sprite_entity(registry, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ASSETS_PATH"test.png");
    registry.emplace<Velocity>(entity, 0.f, 0.f);
    registry.emplace<Player>(entity);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto &system : systems) {
            system(registry);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
