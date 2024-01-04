#include "entt/entt.hpp"

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

typedef void (*System)(entt::registry &);

// all systems should follow the pattern
// void system(entt::registry& register)

// system to draw sprites given a transform
void draw_sprites(entt::registry &registry);

// system to apply velocity
void apply_velocity(entt::registry &registry);

void sort_sprites_by_z(entt::registry &registry);

// move collision boxes to the transform position
void move_box_collisions(entt::registry &registry);

// system to handle box collisions for moving objects
// thanks jack
void handle_box_collisions(entt::registry &registry);

// move the box areas in a similar manner to box collisions
void move_box_areas(entt::registry &registry);

// system to draw the screen relative to the camera
// there should only be one camera in the registry
void camera_system(entt::registry &registry);

// particle system manager
void particle_system(entt::registry &registry);

// system to enable debug rendering (collisions primarily)
void debug_rendering(entt::registry &registry);

void add_move_systems(std::vector<System> &systems);

// add "core" systems, such as sprite rendering, collision, velocity
void add_core_systems(std::vector<System> &systems);


#endif