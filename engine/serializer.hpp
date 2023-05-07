#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "entt/entt.hpp"
#include "components.hpp"
#include <fstream>

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

//void debug_serialize(entt::registry& registry, const char* path) {
//	std::ofstream os(path, std::ios::binary);
//	cereal::BinaryOutputArchive archive{os};
//	entt::snapshot{ registry }.entities(archive).component<BDTransform, Sprite, BoxCollider>(archive);
//}
//
//void debug_load(entt::registry& registry, const char* path) {
//	std::ifstream is(path, std::ios::binary);
//	cereal::BinaryInputArchive archive{ is };
//	entt::snapshot_loader{ registry }.entities(archive).component<BDTransform, Sprite, BoxCollider>(archive);
//}

#endif