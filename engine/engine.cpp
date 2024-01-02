#include "components.hpp"
#include "engine.hpp"
#include "entt/entt.hpp"
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "raylib.h"
#include "raymath.h"
#include "resources.hpp"
#include "serializer.hpp"
#include "spatial_hash.hpp"
#include "prefabs.hpp"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

Engine init_engine(GameInit init, const char *title, int width, int height)
{
	return Engine{init, {}, {}, {}, title, width, height};
}

void add_system(Engine &e, System s)
{
	e.systems.push_back(s);
}

void add_ui_system(Engine &e, System s)
{
	e.ui_systems.push_back(s);
}

void update_loop(Engine &engine)
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	rlImGuiBegin();

	for (auto &system : engine.systems)
	{
		system(engine.registry);
	}

	EndMode2D(); // only has any effect if a camera exists

	for (auto &ui_system : engine.ui_systems)
	{
		ui_system(engine.registry);
	}

	DrawFPS(0, 0);
	rlImGuiEnd();
	EndDrawing();
}

void web_loop(void* data) {
	Engine* e = static_cast<Engine*>(data);
	update_loop(*e);
}

void run_engine(Engine& engine)
{
	// initialize raylib
	InitWindow(engine.window_width, engine.window_height, engine.title);
	InitAudioDevice();
	SetTargetFPS(60);

	// append systems
	add_core_systems(engine.systems);
	add_system(engine, debug_rendering);

	// create the camera
	add_camera(engine.registry);

	// initialize resource caches
	add_ctx(engine.registry);

	// registry.on_construct<BoxCollider>().connect<&update_global_spatial_maps>(registry);
	engine.init(engine.registry);
	update_global_spatial_maps(engine.registry);

	rlImGuiSetup(true);

	#ifndef PLATFORM_WEB
	while (!WindowShouldClose())
	{
		update_loop(engine);
	}
	#endif
	#ifdef PLATFORM_WEB
	emscripten_set_main_loop_arg(&web_loop, &engine, 0, 1);
	#endif

	rlImGuiShutdown();

	CloseWindow();
}



void add_ctx(entt::registry &registry)
{
	// initialize resource caches
	registry.ctx().emplace<TextureCache>();
	registry.ctx().emplace<SpatialHash>();
}

// Fire a raycast in direction with length distance
// Returns the first entity hit by the raycast that has component T into out
template <typename T, typename... Ts>
entt::entity fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2 &hit_position)
{
	auto view = registry.view<BoxCollider, T, Ts...>(entt::exclude<Invisible>);
	float travelled = 0.f;

	while (travelled < distance)
	{
		for (auto &entity : view)
		{
			BoxCollider &b = registry.get<BoxCollider>(entity);

			if (CheckCollisionPointRec(position, b.box))
			{
				hit_position = position;
				return entity;
			}
		}

		position = Vector2Add(position, direction);
		travelled += Vector2Length(direction);
	}
	hit_position = position;
	return entt::null;
}

entt::entity fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2 &hit_position)
{
	return fire_raycast<BDTransform>(registry, position, direction, distance, hit_position);
}

// Fire a raycast in direction with length distance
// Returns a list of entities hit by the raycast that has component T
template <typename T, typename... Ts>
std::vector<entt::entity> fire_raycast_mult(entt::registry &registry, Vector2 position, Vector2 direction, float distance)
{
	auto view = registry.view<BoxCollider, T, Ts...>(entt::exclude<Invisible>);
	float travelled = 0.f;
	std::vector<entt::entity> entities;

	while (travelled < distance)
	{
		for (auto &entity : view)
		{
			BoxCollider &b = registry.get<BoxCollider>(entity);
			if (CheckCollisionPointRec(position, b.box) && std::find(entities.begin(), entities.end(), entity) == entities.end())
			{
				entities.push_back(entity);
			}
		}
		position = Vector2Add(position, direction);
		travelled += Vector2Length(direction);
	}
	return entities;
}
