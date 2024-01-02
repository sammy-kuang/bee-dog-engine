#include "raylib.h"
#include "entt/entt.hpp"
#include "systems.hpp"
#include <vector>

using std::vector;

#ifndef ENGINE_HPP
#define ENGINE_HPP

typedef void (*GameInit)(entt::registry &);

struct Engine {
    GameInit init;
    vector<System> systems;
	vector<System> ui_systems;
    const char* title;
    int window_width;
    int window_height;
};

Engine init_engine(GameInit, const char*, int, int);
void add_system(Engine&, System);
void add_ui_system(Engine&, System);
void run_engine(Engine engine);

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