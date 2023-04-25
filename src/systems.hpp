#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include "singletons.hpp"
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
    view.each([](auto &player, auto &v, BDTransform &t)
              {
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
                  DrawCircle(t.x, t.y, 1.0f, RED); });
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
            if (entity == other || e_vel.cancel_x || e_vel.cancel_y) // we can't collide with ourselves
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

// system to enable debug rendering (collisions primarily)
void debug_rendering(entt::registry &registry)
{
    auto view = registry.view<BoxCollider, BDTransform, Velocity>();

    view.each([](BoxCollider &bd, BDTransform &tr, Velocity &vel)
              {
        auto nx = tr.x + vel.x * GetFrameTime();
        auto ny = tr.y + vel.y * GetFrameTime();

        auto h = bd.create_x_box(nx, ny);
        auto v = bd.create_y_box(nx, ny);

        DrawRectangleLinesEx(h, 2.f, GREEN);
        DrawRectangleLinesEx(v, 2.f, BLUE);
        DrawRectangleLinesEx(bd.box, 2.f, RED); });

    DrawFPS(0, 0);
}

// add "core" systems, such as sprite rendering, collision, velocity
void add_core_systems(std::vector<System> &systems)
{
    systems.push_back(draw_sprites);
    systems.push_back(apply_velocity);
    systems.push_back(player_controller);
    systems.push_back(handle_box_collisions);
}

#endif