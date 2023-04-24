#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include "prefabs.hpp"
#include "systems.hpp"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define WINDOW_TITLE "bd-engine"

int main(void)
{
    entt::registry registry;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    create_sprite_entity(registry, 0, 0, ASSETS_PATH "test.png");

    auto view = registry.view<BDTransform, Sprite>();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        view.each([](auto &transform, auto sprite) {
            DrawTexture(sprite.texture, (int)transform.x, (int)transform.y, WHITE);
        });

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
