#include "raylib.h"
#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "entt/entt.hpp"
#include "components.hpp"
#include <fstream>

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#define SERIALIZABLE_COMPONENTS BDTransform, Sprite, BoxCollider, BoxArea, Velocity, Player

void debug_serialize(entt::registry& registry, const char* path) {
	std::ofstream os(path, std::ios::binary);
	cereal::JSONOutputArchive archive{ os };
	entt::snapshot{ registry }.entities(archive).component<SERIALIZABLE_COMPONENTS>(archive);
}

void debug_load(entt::registry& registry, const char* path) {
	std::ifstream is(path, std::ios::binary);
	cereal::JSONInputArchive archive{ is };
	entt::snapshot_loader{ registry }.entities(archive).component<SERIALIZABLE_COMPONENTS>(archive);
}

template<class Archive>
void serialize(Archive& archive, Rectangle& rect) {
	archive(rect.x, rect.y, rect.width, rect.height);
}
#endif