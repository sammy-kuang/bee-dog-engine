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

#endif