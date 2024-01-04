#include "raylib.h"
#include "entt/entt.hpp"
#include "components.hpp"
#include <string>

#ifndef PREFABS_HPP
#define PREFABS_HPP

// Create an entity with a transform, sprite, box collider, box area, and a velocity.
entt::entity create_sprite_entity(entt::registry &registry, const char* resource_name = "", float x = 0, float y = 0, float z = 0);

// Create and add the camera to the registry's context
Camera2D& add_camera(entt::registry &registry);

typedef void (*ParticleDrawFunction)(int x, int y);

struct Particle {
    ParticleDrawFunction draw;
    float lifetime;
};

// Basic 2D particle system
struct ParticleSystem {
    float lifetime; // how long each particle lasts in seconds
    int count; // how many particles?
    Vector2 direction; // normalized direction vector (will self normalize)
    float velocity; // how fast do the particles travel?
    float spread;  // in radians, how much should the particles spread from the original direction
    ParticleDrawFunction draw; // what function represents the particle's visuals?
};

void spawn_particle_system(entt::registry &registry, ParticleSystem part_system, Vector2 position);

void draw_red_circle(int x, int y);
const ParticleSystem basic_particle_system = {5.f, 12, Vector2{0, -1}, 500, 25 * PI / 180, draw_red_circle};

#endif