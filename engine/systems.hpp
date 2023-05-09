#include "raylib.h"
#include "raymath.h"
#include "entt/entt.hpp"
#include "components.hpp"
#include "resources.hpp"
#include "engine.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <string>

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

typedef void (*System)(entt::registry&);

// all systems should follow the pattern
// void system(entt::registry& register)

// system to draw sprites given a transform
void draw_sprites(entt::registry& registry)
{
	auto view = registry.view<BDTransform, Sprite>(entt::exclude<Invisible>);

	for (auto& entity : view) {
		BDTransform& transform = registry.get<BDTransform>(entity);
		Sprite& sprite = registry.get<Sprite>(entity);
		auto texture = registry.ctx().get<TextureCache>().load_resource(sprite.path);

		int width = (int)(texture.width * transform.scale);
		int height = (int)(texture.height * transform.scale);

		// rotation calculations
		// thanks jack
		// https://github.com/j-ackyao

		if (transform.rotation != 0) {
			float r = sqrt(pow((width / 2.f), 2) + pow((height / 2.f), 2));
			float x_0 = r * cos(PI / 4);
			float y_0 = r * sin(PI / 4);

			float theta = (transform.rotation * PI / 180.0) + PI / 4;
			float x_1 = r * cos(theta);
			float y_1 = r * sin(theta);

			float x_f = -x_1 + x_0 + transform.x - width / 2;
			float y_f = -y_1 + y_0 + transform.y - height / 2;

			Vector2 draw_pos{ x_f, y_f };
			DrawTextureEx(texture, draw_pos, transform.rotation, transform.scale, WHITE);
		}
		else {
			Vector2 draw_pos{ transform.x - width / 2, transform.y - height / 2 };
			DrawTextureEx(texture, draw_pos, transform.rotation, transform.scale, WHITE);
		}
	}


}

// system to apply velocity
void apply_velocity(entt::registry& registry)
{
	auto view = registry.view<BDTransform, Velocity>();

	view.each([](BDTransform& transform, Velocity& velocity)
		{
			auto frame_time = GetFrameTime();
			float delta_x = velocity.x * frame_time;
			float delta_y = velocity.y * frame_time;

			if (!velocity.cancel_x) transform.x += delta_x;
			if (!velocity.cancel_y) transform.y += delta_y;

			velocity.x -= delta_x;
			velocity.y -= delta_y; });
}

void sort_sprites_by_z(entt::registry& registry)
{
	registry.sort<BDTransform>([](const auto& l, const auto& r)
		{ return l.z < r.z; });
}

void player_controller(entt::registry& registry)
{
	auto view = registry.view<Player, Velocity, BDTransform>();
	auto camera_view = registry.view<Camera2D>();

	for (auto& player : view)
	{
		BDTransform& t = registry.get<BDTransform>(player);
		Velocity& v = registry.get<Velocity>(player);
		if (IsKeyDown(KEY_W))
		{
			v.y = -250;
		}
		else if (IsKeyDown(KEY_S))
		{
			v.y = 250;
		}
		else
		{
			v.y = 0;
		}

		if (IsKeyDown(KEY_A))
		{
			v.x = -250;
		}
		else if (IsKeyDown(KEY_D))
		{
			v.x = 250;
		}
		else
		{
			v.x = 0;
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			t.rotation += 5;
		}
		else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			t.rotation -= 5;
		}
		DrawCircle(t.x, t.y, 1.0f, RED);

		Camera2D& c = registry.ctx().get<Camera2D>();
		// update the camera
		c.target = Vector2{ t.x - GetScreenWidth() / 2, t.y - GetScreenHeight() / 2 };

		Vector2 down_hit, up_hit, left_hit, right_hit;
		Vector2 cur = Vector2{ t.x, t.y };

		auto down = fire_raycast<Floor>(registry, cur, Vector2{ 0, 1 }, 100.f, down_hit);
		auto up = fire_raycast<Floor>(registry, cur, Vector2{ 0, -1 }, 100.f, up_hit);
		auto left = fire_raycast<Floor>(registry, cur, Vector2{ -1, 0 }, 100.f, left_hit);
		auto right = fire_raycast<Floor>(registry, cur, Vector2{ 1, 0 }, 100.f, right_hit);

		DrawLineV(cur, down_hit, down == entt::null ? RED : GREEN);
		DrawLineV(cur, up_hit, up == entt::null ? RED : GREEN);
		DrawLineV(cur, left_hit, left == entt::null ? RED : GREEN);
		DrawLineV(cur, right_hit, right == entt::null ? RED : GREEN);
	}
}

// move collision boxes to the transform position
void move_box_collisions(entt::registry& registry) {
	auto view = registry.view<BDTransform, BoxCollider>();

	for (auto& entity : view) {
		BDTransform& tr = registry.get<BDTransform>(entity);
		BoxCollider& bd = registry.get<BoxCollider>(entity);

		bd.move(tr.x, tr.y);
	}
}

// system to handle box collisions for moving objects
// thanks jack
void handle_box_collisions(entt::registry& registry)
{
	auto view = registry.view<BDTransform, Velocity, BoxCollider>();
	auto coll = registry.view<BDTransform, BoxCollider>();

	for (auto& entity : view)
	{
		BDTransform& e_tr = registry.get<BDTransform>(entity);
		Velocity& e_vel = registry.get<Velocity>(entity);
		BoxCollider& e_bc = registry.get<BoxCollider>(entity);

		auto nx = e_tr.x + e_vel.x * GetFrameTime();
		auto ny = e_tr.y + e_vel.y * GetFrameTime();

		auto h = e_bc.create_x_box(nx, ny);
		auto v = e_bc.create_y_box(nx, ny);

		e_vel.cancel_x = false;
		e_vel.cancel_y = false;

		for (auto& other : coll)
		{
			if (entity == other || (e_vel.cancel_x && e_vel.cancel_y)) // we can't collide with ourselves
				continue;

			// check if entity is colliding with other
			// if so, cancel current velocity
			BDTransform& o_tr = registry.get<BDTransform>(other);
			BoxCollider& o_bc = registry.get<BoxCollider>(other);

			bool horizontal = CheckCollisionRecs(h, o_bc.box);
			bool vertical = CheckCollisionRecs(v, o_bc.box);

			if (!horizontal && !vertical)
			{
				continue;
			}

			if (horizontal)
			{
				e_vel.cancel_x = (e_vel.x > 0 && o_tr.x > e_tr.x) || (e_vel.x < 0 && o_tr.x < e_tr.x);
			}
			else
			{
				e_vel.cancel_y = (e_vel.y > 0 && o_tr.y > e_tr.y) || (e_vel.y < 0 && o_tr.y < e_tr.y);
			}
		}
	}
}

// move the box areas in a similar manner to box collisions
void move_box_areas(entt::registry& registry) {
	auto view = registry.view<BDTransform, BoxArea>();

	for (auto& entity : view) {
		BDTransform& t = registry.get<BDTransform>(entity);
		BoxArea& b = registry.get<BoxArea>(entity);
		b.move(t.x, t.y);
	}
}

// system to draw the screen relative to the camera
// there should only be one camera in the registry
void camera_system(entt::registry& registry)
{
	auto camera = registry.ctx().get<Camera2D>();
	auto camera_box = Rectangle{ camera.target.x, camera.target.y, (float)GetScreenWidth(), (float)GetScreenHeight() };
	BeginMode2D(camera);
	auto view = registry.view<BDTransform>(entt::exclude<AlwaysRender>);

	for (auto& entity : view) {
		BDTransform& transform = registry.get<BDTransform>(entity);
		Rectangle comparison = Rectangle{ 0, 0, 0, 0 };
		if (registry.try_get<Sprite>(entity) != nullptr) {
			auto s = registry.get<Sprite>(entity);
			auto texture = registry.ctx().get<TextureCache>().load_resource(s.path);
			comparison = Rectangle{ transform.x - texture.width / 2.f, transform.y - texture.width / 2.f, (float)texture.width, (float)texture.height };
		}
		else if (registry.try_get<BoxCollider>(entity) != nullptr) {
			auto s = registry.get<BoxCollider>(entity).box;
			comparison = Rectangle{ transform.x, transform.y, (float)s.width, (float)s.height };
		}

		bool visible = registry.try_get<Invisible>(entity) == nullptr;
		bool in_view = (comparison.width <= 0 || comparison.height <= 0) ? CheckCollisionPointRec(Vector2{ transform.x, transform.y }, camera_box) : CheckCollisionRecs(comparison, camera_box);

		if (!in_view && visible)
			registry.emplace<Invisible>(entity);
		else if (in_view && !visible)
			registry.remove<Invisible>(entity);

	}
}

// system to enable debug rendering (collisions primarily)
void debug_rendering(entt::registry& registry)
{
	auto collisions = registry.view<BoxCollider>(entt::exclude<Invisible>);
	auto vel_box = registry.view<BoxCollider, Velocity, BDTransform>(entt::exclude<Invisible>);
	auto areas = registry.view<BoxArea>(entt::exclude<Invisible>);

	collisions.each([](BoxCollider& bd)
		{
			DrawRectangleLinesEx(bd.box, 1.f, RED);

		});

	vel_box.each([](BoxCollider& bd, Velocity& vel, BDTransform& tr)
		{
			auto nx = (int)(tr.x + vel.x * GetFrameTime());
			auto ny = (int)(tr.y + vel.y * GetFrameTime());

			auto h = bd.create_x_box(nx, ny);
			auto v = bd.create_y_box(nx, ny);

			DrawRectangleLinesEx(h, 1.f, GREEN);
			DrawRectangleLinesEx(v, 1.f, BLUE);

		});

	areas.each([](BoxArea& ba) {
		DrawRectangleRec(ba.create_area_rectangle(), Color{ 102, 191, 255, 125 });
		});

}


void add_move_systems(std::vector<System>& systems) {
	systems.push_back(move_box_collisions);
	systems.push_back(move_box_areas);
}

// add "core" systems, such as sprite rendering, collision, velocity
void add_core_systems(std::vector<System>& systems)
{
	add_move_systems(systems);
	systems.push_back(camera_system);
	systems.push_back(draw_sprites);
	systems.push_back(handle_box_collisions);
	systems.push_back(apply_velocity);
	systems.push_back(player_controller);
}

#endif