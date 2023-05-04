#include "raylib.h"
#include "raymath.h"
#include "entt.hpp"
#include "components.hpp"
#include <vector>
#include <algorithm>

#ifndef ENGINE_HPP
#define ENGINE_HPP

// Fire a raycast in direction with length distance
// Returns the first entity hit by the raycast that has component T into out
template <typename T, typename...Ts>
entt::entity fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2 &hit_position)
{
    auto view = registry.view<BoxCollider, T, Ts...>(entt::exclude<Invisible>);
    float travelled = 0.f;

    while (travelled < distance)
    {
        for (auto &entity : view)
        {
            BoxCollider& b = registry.get<BoxCollider>(entity);

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

entt::entity fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2 &hit_position) {
    return fire_raycast<BDTransform>(registry, position, direction, distance, hit_position);
}

// Fire a raycast in direction with length distance
// Returns a list of entities hit by the raycast that has component T
template <typename T, typename...Ts>
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

// Create and add the camera to the registry's context
void add_camera(entt::registry &registry) {
    registry.ctx().emplace<Camera2D>(Vector2{0, 0}, Vector2{0, 0}, 0.f, 1.f);
}


#endif