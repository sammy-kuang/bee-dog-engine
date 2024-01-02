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

	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(x, y, z, rotation, scale);
	}
};


struct Sprite
{
	std::string path{};
	Texture2D *handle = nullptr;

	friend class cereal::access;

	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(path, nullptr);
	}
};

struct Name
{
	std::string name;

	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(name);
	}
};

struct Velocity
{
	float x = 0;
	float y = 0;
	bool cancel_x = false;
	bool cancel_y = false;

	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(x, y, cancel_x, cancel_y);
	}
};

static int COLLISION_THRESHOLD = 5;

struct BoxCollider
{
	Rectangle box;
	bool spatially_initialized = false;

	void move(int x, int y);

	void rebase_on_sprite(entt::registry &registry, entt::entity entity);

	Rectangle create_x_box(int nx, int ny);

	Rectangle create_y_box(int nx, int ny);

	template <class Archive>
	void serialize(Archive &archive) {
		archive(box, false);
	}
};

struct BoxArea
{
	Rectangle box;

	void move(int x, int y);

	float get_margin();

	void rebase_on_sprite(entt::registry &registry, entt::entity entity);

	Rectangle create_area_rectangle();

	vector<entt::entity> get_colliding_entities(entt::registry &registry);

	template <class Archive>
	void serialize(Archive &archive) {
		archive(box);
	}
};

struct Player
{
	int id = 0;
	template <class Archive>
	void serialize(Archive &archive) {
		archive(id);
	}
};

struct Floor
{
	int id = 0;
};

// component to indicate that an entity should always been updated
struct AlwaysRender
{
	int id;

	template <class Archive>
	void serialize(Archive &archive) {
		archive(id);
	}
};

// component to indicate that an entity should not be updated
struct Invisible
{
	int id;
};

#endif