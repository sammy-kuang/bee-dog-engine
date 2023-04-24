#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include <vector>

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

typedef void (*System)(entt::registry&);

// all systems should follow the pattern
// void system(entt::registry& register)

// system to draw sprites given a transform
void draw_sprites(entt::registry& registry) {
    auto view = registry.view<BDTransform, Sprite>();

    view.each([](BDTransform &transform, Sprite &sprite) {
        int width = sprite.texture.width;
        int height = sprite.texture.height;

        Vector2 draw_pos = Vector2{transform.x - width / 2.f, transform.y - height / 2.f};
        // Vector2 size = sprite.texture;

        DrawTextureEx(sprite.texture, draw_pos, transform.rotation, transform.scale, WHITE);
    });
}

// system to apply velocity
void apply_velocity(entt::registry& registry) {
    auto view = registry.view<BDTransform, Velocity>();

    view.each([](BDTransform &transform, Velocity &velocity) {
        auto frame_time = GetFrameTime();
        float delta_x = velocity.x * frame_time;
        float delta_y = velocity.y * frame_time;

        if (!velocity.cancel_tick) {
            transform.x += delta_x;
            transform.y += delta_y;
        }

        velocity.x -= delta_x;
        velocity.y -= delta_y;
    });
}

void player_controller(entt::registry& registry) {
    auto view = registry.view<Player, Velocity>();

    view.each([](auto &player, auto &v) {
        if (IsKeyDown(KEY_W)) {
            v.y = -250;
        } else if (IsKeyDown(KEY_S)) {
            v.y = 250;
        } else {
            v.y = 0;
        }

        if (IsKeyDown(KEY_A)) {
            v.x = -250;
        } else if (IsKeyDown(KEY_D)) {
            v.x = 250;
        } else {
            v.x = 0;
        }
    });
}

#endif