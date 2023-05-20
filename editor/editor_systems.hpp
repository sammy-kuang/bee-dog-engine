#pragma once
#include "editor.hpp"
#include <iostream>
#include <vector>

// Directly taken from:
// https://github.com/raylib-extras/examples-c/blob/main/mouse_zoom/mouse_zoom.c
// It just works so well that theres nothing I need to change
void camera_movement(entt::registry& registry) {
	auto &cam = registry.ctx().get<Camera2D>();

	// translate based on right click
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		Vector2 delta = GetMouseDelta();
		delta = Vector2Scale(delta, -1.0f / cam.zoom);

		cam.target = Vector2Add(cam.target, delta);
	}

	// zoom based on wheel
	float wheel = GetMouseWheelMove();
	if (wheel != 0)
	{
		// get the world point that is under the mouse
		Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);

		// set the offset to where the mouse is
		cam.offset = GetMousePosition();

		// set the target to match, so that the camera maps the world space point under the cursor to the screen space point under the cursor at any zoom
		cam.target = mouseWorldPos;

		// zoom
		cam.zoom += wheel * 0.125f;
		if (cam.zoom < 0.125f)
			cam.zoom = 0.125f;
	}
}

#define ARROW_LENGTH 125
#define ARROW_WIDTH 7

void editor_entity_controls(entt::registry& registry) {
	auto& editor = registry.ctx().get<Editor>();
	auto& camera = registry.ctx().get<Camera2D>();

	if (registry.valid(editor.current_entity)) {
		auto *t = registry.try_get<BDTransform>(editor.current_entity);
		if (t == nullptr) return;

		// draw our arrows
		auto arrow_length = ARROW_LENGTH * (0.8f / camera.zoom);
		auto arrow_width = ARROW_WIDTH * (0.8f / camera.zoom);
		auto square_size = 25 * (0.8f / camera.zoom);

		auto right_arrow = Rectangle{ t->x, t->y - arrow_width / 2.f, arrow_length, arrow_width };
		auto up_arrow = Rectangle{ t->x - arrow_width, t->y - arrow_length, arrow_width, arrow_length };
		auto square = Rectangle{ t->x, t->y - arrow_width / 2.f - square_size, square_size, square_size };

		DrawRectangleRec(right_arrow, RED);
		DrawRectangleRec(up_arrow, BLUE);
		DrawRectangleRec(square, Color{0, 255, 0, 200});

		auto mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);

		if (CheckCollisionPointRec(mouse_pos, right_arrow) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !editor.up_arrow_grab) {
			editor.right_arrow_grab = true;
		}
		else if (CheckCollisionPointRec(mouse_pos, up_arrow) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !editor.right_arrow_grab) {
			editor.up_arrow_grab = true;
		}
		else if (CheckCollisionPointRec(mouse_pos, square) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (!editor.right_arrow_grab && !editor.up_arrow_grab)) {
			editor.right_arrow_grab = true;
			editor.up_arrow_grab = true;
		}

		auto delta = Vector2Scale(GetMouseDelta(), 1 / camera.zoom);

		if (editor.right_arrow_grab) {
			t->x += delta.x;
		}

		if (editor.up_arrow_grab) {
			t->y += delta.y;
		}

		if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
			editor.right_arrow_grab = false;
			editor.up_arrow_grab = false;
		}
	}

	if (!editor.right_arrow_grab && !editor.up_arrow_grab && GetMouseX() > WINDOW_WIDTH && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		auto clickable = registry.view<BoxCollider>();

		for (auto& entity : clickable) {
			auto& coll = registry.get<BoxCollider>(entity);
			if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), coll.box)) {
				editor.current_entity = entity;
			}
		}
	}
}

void add_editor_systems(std::vector<System> &systems) {
	systems.push_back(camera_movement);
	systems.push_back(editor_entity_controls);
}
