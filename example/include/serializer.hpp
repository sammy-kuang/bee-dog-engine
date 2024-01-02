#include "raylib.h"
#include "entt/entt.hpp"
#include "components.hpp"
#include <string>

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#define SERIALIZABLE_COMPONENTS BDTransform, Sprite, BoxCollider, BoxArea, Velocity, Player, Name

void serialize_level(entt::registry& registry, std::string level_resource) ;

void load_level(entt::registry& registry, std::string level_resource) ;

template<class Archive>
void serialize(Archive& archive, Rectangle& rect) ;

#endif