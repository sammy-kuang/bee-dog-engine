#include "raylib.h"
#include "entt/entt.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"
#include "resources.hpp"
#include <vector>
#include <string>

using std::vector, std::string;

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

struct BDTransform
{
	float x = 0;
	float y = 0;
	float z = 0;
	float rotation = 0;
	float scale = 1;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(x, y, z, rotation, scale);
	}
};

struct Sprite
{
	std::string path{};

	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(path);
	}
};

struct Name {
	std::string name;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(name);
	}
};

struct Velocity
{
	float x = 0;
	float y = 0;
	bool cancel_x = false;
	bool cancel_y = false;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(x, y, cancel_x, cancel_y);
	}
};

static int COLLISION_THRESHOLD = 5;

struct BoxCollider
{
	Rectangle box;

	void move(int x, int y)
	{
		box.x = x - box.width / 2;
		box.y = y - box.height / 2;
	}

	void rebase_on_sprite(entt::registry& registry, entt::entity entity) {
		auto sprite = registry.try_get<Sprite>(entity);

		if (sprite != nullptr) {
			auto tex = registry.ctx().get<TextureCache>().load_resource(sprite->path);
			box.width = (float)tex.width;
			box.height = (float)tex.height;
		}
	}

	Rectangle create_x_box(int nx, int ny)
	{
		return Rectangle{ (float)(nx - box.width / 2), (float)(ny + COLLISION_THRESHOLD - box.height / 2), box.width, box.height - COLLISION_THRESHOLD * 2 };
	}

	Rectangle create_y_box(int nx, int ny)
	{
		return Rectangle{ (float)(nx + COLLISION_THRESHOLD - box.width / 2), (float)(ny - box.height / 2), box.width - COLLISION_THRESHOLD * 2, box.height };
	}

	template<class Archive>
	void serialize(Archive& archive) {
		archive(box);
	}
};

struct BoxArea
{
	Rectangle box;

	void move(int x, int y)
	{
		box.x = x - (box.width + get_margin()) / 2;
		box.y = y - (box.height + get_margin()) / 2;
	}

	float get_margin()
	{
		return COLLISION_THRESHOLD * 1.6f;
	}

	void rebase_on_sprite(entt::registry& registry, entt::entity entity) {
		auto sprite = registry.try_get<Sprite>(entity);

		if (sprite != nullptr) {
			auto tex = registry.ctx().get<TextureCache>().load_resource(sprite->path);
			box.width = (float)tex.width;
			box.height = (float)tex.height;
		}
	}

	Rectangle create_area_rectangle()
	{

		return Rectangle{ box.x, box.y, box.width + get_margin(), box.height + get_margin() };
	}

	vector<entt::entity> get_colliding_entities(entt::registry& registry)
	{
		vector<entt::entity> list;

		auto view = registry.view<BoxCollider>();

		for (auto& entity : view)
		{
			if (&registry.get<BoxArea>(entity) == this)
				continue;

			BoxCollider& b = registry.get<BoxCollider>(entity);
			auto b_box = b.box;

			if (CheckCollisionRecs(create_area_rectangle(), b_box))
				list.push_back(entity);
		}

		return list;
	}

	template<class Archive>
	void serialize(Archive& archive) {
		archive(box);
	}

};

struct Player
{
	int id = 0;
	template<class Archive>
	void serialize(Archive& archive) {
		archive(id);
	}
};

struct Floor
{
	int id = 0;
};

// component to indicate that an entity should always been updated
struct AlwaysRender {
	int id;

	template<class Archive>
	void serialize(Archive& archive) {
		archive(id);
	}

};

// component to indicate that an entity should not be updated
struct Invisible {
	int id;
};

#endif