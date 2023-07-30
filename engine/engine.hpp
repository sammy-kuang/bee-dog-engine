#include "raylib.h"
#include "entt/entt.hpp"

#ifndef ENGINE_HPP
#define ENGINE_HPP

void add_ctx(entt::registry &registry);

// Fire a raycast in direction with length distance
// Returns the first entity hit by the raycast that has component T into out
template <typename T, typename... Ts>
entt::entity fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2 &hit_position);

entt::entity fire_raycast(entt::registry &registry, Vector2 position, Vector2 direction, float distance, Vector2 &hit_position);

// Fire a raycast in direction with length distance
// Returns a list of entities hit by the raycast that has component T
template <typename T, typename... Ts>
std::vector<entt::entity> fire_raycast_mult(entt::registry &registry, Vector2 position, Vector2 direction, float distance);


#endif