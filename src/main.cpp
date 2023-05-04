#include <vector>

#include "raylib.h"
#include "entt/entt.hpp"
#include "components.hpp"
#include "prefabs.hpp"
#include "systems.hpp"
#include "singletons.hpp"
#include "engine.hpp"
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

    Texture2D tex = LoadTexture(ASSETS_PATH "gura.png");

    // create some entities
    auto player = create_sprite_entity(registry, tex, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, -1.f);
    registry.emplace<Player>(player);
    auto &vel = registry.get<Velocity>(player);

    for (int i = 0; i < 100; i++)
    {
        auto entity = create_sprite_entity(registry, tex, SCREEN_WIDTH / 2 - 400 + i * 100.1, SCREEN_HEIGHT / 2 + 200 + GetRandomValue(-10, 10));
        registry.emplace<Floor>(entity);
    }

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