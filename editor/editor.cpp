#include "raylib.h"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

int main(void)
{
	const int screenWidth = 1600;
	const int screenHeight = 900;
	Vector2 cameraPos;

	InitWindow(screenWidth, screenHeight, "bd-engine editor");

	SetTargetFPS(60);

	rlImGuiSetup(true);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);
		rlImGuiBegin();
	
		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}