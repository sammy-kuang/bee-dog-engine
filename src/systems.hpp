#include "raylib.h"
#include "raymath.h"
#include "entt.hpp"
#include "components.hpp"
#include "singletons.hpp"
#include "engine.hpp"
#include <vector>
#include <iostream>
#include <cmath>

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

typedef void (*System)(entt::registry &);

// all systems should follow the pattern
// void system(entt::registry& register)

// system to draw sprites given a transform
void draw_sprites(entt::registry &registry)
{
    auto view = registry.view<BDTransform, Sprite>();

    view.each([](BDTransform &transform, Sprite &sprite)
              {
        int width = sprite.texture.width * transform.scale;
        int height = sprite.texture.height * transform.scale;

        // rotation calculations
        // thanks jack
        // https://github.com/j-ackyao
        float r = sqrt(pow((width / 2.f), 2) + pow((height / 2.f), 2));
        float x_0 = r * cos(PI / 4);
        float y_0 = r * sin(PI / 4);

        float theta = (transform.rotation * PI / 180.0) + PI / 4;
        float x_1 = r * cos(theta);
        float y_1 = r * sin (theta);

        float x_f = -x_1 + x_0 + transform.x - width / 2;
        float y_f = -y_1 + y_0 + transform.y - height / 2; 

        Vector2 draw_pos{x_f, y_f};

        DrawTextureEx(sprite.texture, draw_pos, transform.rotation, transform.scale, WHITE); });
}

// system to apply velocity
void apply_velocity(entt::registry &registry)
{
    auto view = registry.view<BDTransform, Velocity>();

    view.each([](BDTransform &transform, Velocity &velocity)
              {
        auto frame_time = GetFrameTime();
        float delta_x = velocity.x * frame_time;
        float delta_y = velocity.y * frame_time;

        if (!velocity.cancel_x) transform.x += delta_x;
        if (!velocity.cancel_y) transform.y += delta_y;

        velocity.x -= delta_x;
        velocity.y -= delta_y; });
}

void sort_sprites_by_z(entt::registry &registry)
{
    registry.sort<BDTransform>([](const auto &l, const auto &r)
                               { return l.z < r.z; });
}

void player_controller(entt::registry &registry)
{
    auto view = registry.view<Player, Velocity, BDTransform>();
    auto camera_view = registry.view<Camera2D>();

    for (auto &player : view)
    {
        BDTransform &t = registry.get<BDTransform>(player);
        Velocity &v = registry.get<Velocity>(player);
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

        Camera2D &c = registry.ctx().get<Camera2D>();

        Vector2 m_pos = GetScreenToWorld2D(GetMousePosition(), c);
        float dst = Vector2Distance(Vector2{t.x, t.y}, m_pos);
        Vector2 dir = Vector2Normalize(Vector2Subtract(m_pos, Vector2{t.x, t.y}));
        Vector2 hit_pos = Vector2{0, 0};
        auto entity = fire_raycast<Floor>(registry, Vector2{t.x, t.y}, dir, dst, hit_pos);

        if (entity != entt::null)
        {
            DrawLine(t.x, t.y, hit_pos.x, hit_pos.y, GREEN);

            BDTransform &b = registry.get<BDTransform>(entity);
            if (hit_pos.x < b.x)
                b.rotation -= 50 * GetFrameTime();
            else
                b.rotation += 50 * GetFrameTime();
        }
        else
        {
            DrawLine(t.x, t.y, m_pos.x, m_pos.y, RED);
        }

        // update the camera
        c.target = Vector2{t.x - GetScreenWidth() / 2, t.y - GetScreenHeight() / 2};


        // get the box area
        BoxArea &ba = registry.get<BoxArea>(player);

        for (auto& e : ba.get_colliding_entities(registry)) {
            registry.destroy(e);
        }
    }
}

// system to handle box collisions for moving objects
// thanks jack
void handle_box_collisions(entt::registry &registry)
{
    auto view = registry.view<BDTransform, Velocity, BoxCollider>();
    auto coll = registry.view<BDTransform, BoxCollider>();

    for (auto &entity : view)
    {
        BDTransform &e_tr = registry.get<BDTransform>(entity);
        Velocity &e_vel = registry.get<Velocity>(entity);
        BoxCollider &e_bc = registry.get<BoxCollider>(entity);

        e_bc.move(e_tr.x, e_tr.y); // make the collision box follow the transform

        auto nx = e_tr.x + e_vel.x * GetFrameTime();
        auto ny = e_tr.y + e_vel.y * GetFrameTime();

        auto h = e_bc.create_x_box(nx, ny);
        auto v = e_bc.create_y_box(nx, ny);

        e_vel.cancel_x = false;
        e_vel.cancel_y = false;

        for (auto &other : coll)
        {
            if (entity == other || (e_vel.cancel_x && e_vel.cancel_y)) // we can't collide with ourselves
                continue;

            // check if entity is colliding with other
            // if so, cancel current velocity
            BDTransform &o_tr = registry.get<BDTransform>(other);
            BoxCollider &o_bc = registry.get<BoxCollider>(other);

            bool horizontal = CheckCollisionRecs(h, o_bc.box);
            bool vertical = CheckCollisionRecs(v, o_bc.box);

            if (!horizontal && !vertical)
            {
                continue;
            }

            if (horizontal)
            {
                if (e_vel.x > 0 && o_tr.x > e_tr.x)
                    e_vel.cancel_x = true;
                else if (e_vel.x < 0 && o_tr.x < e_tr.x)
                    e_vel.cancel_x = true;
                else
                    e_vel.cancel_x = false;
            }
            else
            {
                if (e_vel.y > 0 && o_tr.y > e_tr.y)
                    e_vel.cancel_y = true;
                else if (e_vel.y < 0 && o_tr.y < e_tr.y)
                    e_vel.cancel_y = true;
                else
                    e_vel.cancel_y = false;
            }
        }
    }
}

// move the box areas in a similar manner to box collisions
void move_box_areas(entt::registry &registry) {
    auto view = registry.view<BDTransform, BoxArea>();

    for (auto &entity : view) {
        BDTransform &t = registry.get<BDTransform>(entity);
        BoxArea &b = registry.get<BoxArea>(entity);
        b.move(t.x, t.y);
    }
}

// system to draw the screen relative to the camera
// there should only be one camera in the registry
void camera_system(entt::registry &registry)
{
    BeginMode2D(registry.ctx().get<Camera2D>());
}

// system to enable debug rendering (collisions primarily)
void debug_rendering(entt::registry &registry)
{
    auto view = registry.view<BoxCollider, BoxArea, BDTransform, Velocity>();

    view.each([](BoxCollider &bd, BoxArea &ba, BDTransform &tr, Velocity &vel)
              {
        auto nx = tr.x + vel.x * GetFrameTime();
        auto ny = tr.y + vel.y * GetFrameTime();

        auto h = bd.create_x_box(nx, ny);
        auto v = bd.create_y_box(nx, ny);

        DrawRectangleLinesEx(h, 2.f, GREEN);
        DrawRectangleLinesEx(v, 2.f, BLUE);
        DrawRectangleLinesEx(bd.box, 2.f, RED); 
        DrawRectangleRec(ba.box, Color{102, 191, 255, 100});
        
        });
        
}

// add "core" systems, such as sprite rendering, collision, velocity
void add_core_systems(std::vector<System> &systems)
{
    systems.push_back(camera_system);
    systems.push_back(draw_sprites);
    systems.push_back(handle_box_collisions);
    systems.push_back(move_box_areas);
    systems.push_back(apply_velocity);
    systems.push_back(player_controller);
}

#endif