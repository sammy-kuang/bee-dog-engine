#include "raylib.h"
#include "raymath.h"
#include "entt.hpp"
#include "components.hpp"
#include <vector>
#include <algorithm>

#ifndef ENGINE_HPP
#define ENGINE_HPP

// Fire a raycast from position in direction, where direction is a normalized vector, at a maximum distance of distance
// The hit position, or the maximum distance from the position in the direction will be returned in hit_position
// When the raycast hits ANY object, it will immediately return true and a hit_position
bool fire_raycast_first(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2 &hit_position)
{
    float travelled = 0.0f;
    bool hit = false;

    auto view = registry.view<BoxCollider>();

    while (travelled < distance && !hit)
    {
        for (auto &entity : view)
        {
            BoxCollider &b = view.get<BoxCollider>(entity);

            if (CheckCollisionPointRec(position, b.box))
            {
                hit = true;
            }
        }

        if (!hit)
        {
            position = Vector2Add(position, direction);
            travelled += Vector2Length(direction);
        }

        hit_position = position;
    }

    return hit;
}

// Fire a raycast in direction with length distance
// Returns the first entity hit by the raycast that has component T into out
template <typename T, typename...Ts>
bool fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, entt::entity &out)
{
    auto view = registry.view<BoxCollider, T, Ts...>();
    float travelled = 0.f;

    while (travelled < distance)
    {
        for (auto &entity : view)
        {
            BoxCollider &b = registry.get<BoxCollider>(entity);

            if (CheckCollisionPointRec(position, b.box))
            {
                out = entity;
                return true;
            }
        }

        position = Vector2Add(position, direction);
        travelled += Vector2Length(direction);
    }

    out = entt::null;
    return false;
}

// Fire a raycast in direction with length distance
// Returns a list of entities hit by the raycast that has component T
template <typename T, typename...Ts>
std::vector<entt::entity> fire_raycast_mult(entt::registry &registry, Vector2 position, Vector2 direction, float distance)
{
    auto view = registry.view<BoxCollider, T, Ts...>();
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
#endif