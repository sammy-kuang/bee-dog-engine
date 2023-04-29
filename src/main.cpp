#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include "prefabs.hpp"
#include "systems.hpp"
#include "singletons.hpp"
#include "engine.hpp"
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
    vector<System> ui_systems;

    // register singletons
    register_singletons(registry);

    // initialize raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    InitAudioDevice();
    SetTargetFPS(120);

    // append systems
    add_core_systems(systems);
    systems.push_back(debug_rendering);

    // create the camera
    add_camera(registry);

    // create some entities
    auto player = create_sprite_entity(registry, ASSETS_PATH "gura.png", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, -1.f);
    registry.emplace<Player>(player);
    auto &vel = registry.get<Velocity>(player);


    for (int i = 0; i < 10; i++)
    {
        auto entity = create_sprite_entity(registry, ASSETS_PATH "gura.png", SCREEN_WIDTH / 2 - 400 + i * 100.1, SCREEN_HEIGHT / 2 + 200 + GetRandomValue(-10, 10));
        registry.emplace<Floor>(entity);
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto &system : systems)
        {
            system(registry);
        }

        EndMode2D(); // only has any effect if a camera exists

        DrawFPS(0, 0);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}