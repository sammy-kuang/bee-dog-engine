#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include <string>

#ifndef PREFABS_HPP
#define PREFABS_HPP

entt::entity create_sprite_entity(entt::registry &registry, float x,  float y, std::string path) {
    auto entity = registry.create();

    registry.emplace<BDTransform>(entity, x, y, 0.f);
    registry.emplace<Sprite>(entity, LoadTexture(path.c_str()));

    return entity;
}

#endif