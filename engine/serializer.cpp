#include "serializer.hpp"

#include "cereal/archives/json.hpp"
#include "cereal/archives/binary.hpp"
#include "resources.hpp"
#include <fstream>

void serialize_level(entt::registry& registry, std::string level_resource) {
	auto path = get_asset_path(level_resource);
	std::ofstream os(path, std::ios::binary);
	cereal::JSONOutputArchive archive{ os };
	entt::snapshot{ registry }.entities(archive).component<SERIALIZABLE_COMPONENTS>(archive);
}

void load_level(entt::registry& registry, std::string level_resource) {
	auto path = get_asset_path(level_resource);
	std::ifstream is(path, std::ios::binary);
	cereal::JSONInputArchive archive{ is };
	entt::snapshot_loader{ registry }.entities(archive).component<SERIALIZABLE_COMPONENTS>(archive);
}

template<class Archive>
void serialize(Archive& archive, Rectangle& rect) {
	archive(rect.x, rect.y, rect.width, rect.height);
}