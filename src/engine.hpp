#include "raylib.h"
#include "raymath.h"
#include "entt.hpp"
#include "components.hpp"

#ifndef ENGINE_HPP
#define ENGINE_HPP


// Fire a raycast from position in direction, where direction is a normalized vector, at a maximum distance of distance
// The hit position, or the maximum distance from the position in the direction will be returned in hit_position
bool fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2& hit_position) {
    float travelled = 0.0f;
    bool hit = false;
    
    auto view = registry.view<BoxCollider>();

    while (travelled < distance && !hit) {
        for (auto &entity : view) {
            BoxCollider &b = view.get<BoxCollider>(entity);

            if (CheckCollisionPointRec(position, b.box)) {
                hit = true;
            }
        }

        if (!hit) {
            position = Vector2Add(position, direction);
            travelled += Vector2Length(direction);
        }

        hit_position = position;
    }

    return hit;
} 

#endif