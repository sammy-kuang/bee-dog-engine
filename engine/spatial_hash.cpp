#include "spatial_hash.hpp"
#include <algorithm>

size_t HashIntTuple::operator()(const std::tuple<int, int> &t) const
{
    return std::get<0>(t) ^ std::get<1>(t);
}

int round_map_size(int a)
{
    return (int)(a / SPATIAL_HASH_SIZE) * SPATIAL_HASH_SIZE;
}

std::tuple<int, int> pos_to_tuple(int x, int y)
{
    return std::make_tuple(round_map_size(x), round_map_size(y));
}

std::vector<entt::entity> *get_spatial_map(entt::registry &registry, std::tuple<int, int> position_key)
{
    auto &sp = registry.ctx().get<SpatialHash>();

    if (sp.map.find(position_key) == sp.map.end())
    {
        sp.map[position_key] = new vector<entt::entity>();
    }

    return sp.map[position_key];
}

void remove_from_spatial_map(entt::registry &registry, entt::entity entity, std::tuple<int, int> key)
{
    auto &tr = registry.get<BDTransform>(entity);
    auto *map = get_spatial_map(registry, key);

    auto position = std::find(map->begin(), map->end(), entity);

    if (position != map->end())
    {
        map->erase(position);
        std::cout << "Removed from spatial map! [" << std::get<0>(key) << ',' << std::get<1>(key) << "]\n";
        std::cout << "New size of the previous map is: " << map->size() << '\n';
    }
}

void add_to_spatial_map(entt::registry &registry, entt::entity entity, std::tuple<int, int> position_key)
{
    auto *map = get_spatial_map(registry, position_key);
    map->push_back(entity);

    std::cout << "Added to spatial map...[" << std::get<0>(position_key) << ',' << std::get<1>(position_key) << "]\n";
}

void update_global_spatial_maps(entt::registry &registry)
{
    auto view = registry.view<BoxCollider>();

    for (auto &entity : view)
    {
        auto &bc = registry.get<BoxCollider>(entity);
        if (!bc.spatially_initialized)
        {
            add_to_spatial_map(registry, entity, pos_to_tuple(bc.box.x, bc.box.y));
            bc.spatially_initialized = true;
        }
    }
}

std::vector<entt::entity> get_surrounding_maps(entt::registry &registry, std::tuple<int, int> position_key)
{
    vector<entt::entity> output;
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            auto other_key = pos_to_tuple(std::get<0>(position_key) + (x * SPATIAL_HASH_SIZE), std::get<1>(position_key) + (y * SPATIAL_HASH_SIZE));
            auto vec = get_spatial_map(registry, other_key);
            output.insert(output.end(), vec->begin(), vec->end());
        }
    }
    return output;
}