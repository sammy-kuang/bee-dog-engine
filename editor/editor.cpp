#include "raylib.h"

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;
    Vector2 cameraPos;

    InitWindow(screenWidth, screenHeight, "bd-engine editor");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}