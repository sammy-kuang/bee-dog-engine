#include "raylib.h"
#include "entt.hpp"
#include "components.hpp"
#include <string>

#ifndef PREFABS_HPP
#define PREFABS_HPP

entt::entity create_sprite_entity(entt::registry &registry, std::string path, float x = 0, float y = 0, float z = 0) {
    auto entity = registry.create();

    Texture2D tex = LoadTexture(path.c_str());
    registry.emplace<BDTransform>(entity, x, y, z);
    registry.emplace<Sprite>(entity, tex);
    registry.emplace<Velocity>(entity);
    registry.emplace<BoxCollider>(entity, Rectangle{(float)(x - tex.width / 2), (float)(y - tex.height / 2), (float)tex.width, (float)tex.height});

    return entity;
}

#endif